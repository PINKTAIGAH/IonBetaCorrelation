#include "GlobalConstants.hh"
#include "CorrelationManager.hh"

// *************************************************************************************
// *********************** BETA CANDIDATE STRUCT ****************************
// *************************************************************************************

void BetaCandidateEvent::SetGammaParameters(Double_t gammaEnergy){
  delayedGammaEnergy = gammaEnergy;
  delayedGammaMatch = true;
}

bool BetaCandidateEvent::HasMatchedGammaInRange(std::set<Double_t> knownGammaSet, Double_t gammaRange){
  if (!delayedGammaMatch) return false;
  return knownGammaSet.lower_bound(delayedGammaEnergy-gammaRange) != knownGammaSet.upper_bound(delayedGammaEnergy-gammaRange);
}

// *************************************************************************************
// *********************** CORRELATION MANAGER *******************************
// *************************************************************************************

// Constructor

CorrelationManager::CorrelationManager(EventMaps eventMaps, HistogramManager* histogramManager){

  // Assign the unpacked event maps
  implantEventMap = eventMaps.implant;
  gatedImplantEventMap = eventMaps.gatedImplant;
  decayEventMap = eventMaps.decay;
  germaniumEventMap = eventMaps.germanium;

  // Assign the histogram manager
  this->histogramManager = histogramManager;

  // Initialise and fill deadtime window
  InitialiseDeadtimeWindowManager();
  TagImplants();

}

// Private Methods

void CorrelationManager::InitialiseDeadtimeWindowManager(){
  deadtimeWindowManager = std::make_unique<TimeRangeManagerLocal>(Global::localDeadTimePositionWindow); 
}

void CorrelationManager::TagImplants(){

  /// !!!! HAVEN'T ADDED INTERRUPTED VETO YET

  // Loop over all gated implants
  for (auto implantItr = implantEventMap->begin(); implantItr != implantEventMap->end(); ++implantItr ){
    
    auto& implantEvent = implantItr->second;    

    deadtimeWindowManager->AddRange(
      implantEvent.time,
      implantEvent.time + Global::implantDeadTime,
      implantEvent.x,
      implantEvent.y
    );

  }

}

void CorrelationManager::CorrelateImplantDecays(){

  // Forward declaration of vector containing BetaCandidateEnvents
  std::vector<BetaCandidateEvent> betaCandidateEventVector;

  // Loop over all gated implants
  for (auto gatedImplantItr = gatedImplantEventMap->begin(); gatedImplantItr != gatedImplantEventMap->end(); ++gatedImplantItr ){

    // clear beta candidate vector from last loop
    betaCandidateEventVector.clear();
    // Unpack gated implant event
    auto& gatedImplantEvent = gatedImplantItr->second;

    // Veto implant event if overlapping with noisy strip in HEC
    if ( IsNoisyStrip(Global::BROKEN_AIDA_X_STRIPS_IMPLANT, {gatedImplantEvent.cminX, gatedImplantEvent.cmaxX}) ) continue;
    if ( IsNoisyStrip(Global::BROKEN_AIDA_Y_STRIPS_IMPLANT, {gatedImplantEvent.cminY, gatedImplantEvent.cmaxY}) ) continue;

    // Fill histogram
    histogramManager->h2_aida_implant_xy->Fill(gatedImplantEvent.x, gatedImplantEvent.y); 

    // Initialise loop variables for nested loop
    Int_t implantDecayMultiplicityForward = 0;
    Int_t implantDecayMultiplicityBackward = 0;
    bool foundMatchForward = false;
    bool foundMatchBackward = false;

    // Loop over forward direction

    // Find Lower and upper bounds for decays
    auto  decayItrStart = decayEventMap->lower_bound((Long64_t)gatedImplantEvent.time);
    auto  decayItrEnd = decayEventMap->upper_bound((Long64_t)gatedImplantEvent.time + Global::timeThreshold);

    for (auto decayItr = decayItrStart; decayItr != decayItrEnd; ++decayItr){

      // Unpack gated implant event
      auto& decayEvent = decayItr->second;

      // Veto decay event if overlapping with noisy strip in HEC
      if ( IsNoisyStrip(Global::BROKEN_AIDA_X_STRIPS_DECAY, {decayEvent.cminX, decayEvent.cmaxX}) ) continue;
      if ( IsNoisyStrip(Global::BROKEN_AIDA_Y_STRIPS_DECAY, {decayEvent.cminY, decayEvent.cmaxY}) ) continue;

      // Check if we only analyse offspill
      if ( Global::onlyOffspillDecays && decayEvent.spill == 1) continue;

      // Skip if clusters aren't overlapping
      if ( !AreClustersOverlapping( {{gatedImplantEvent.cminX, gatedImplantEvent.cmaxX}, {gatedImplantEvent.cminY, gatedImplantEvent.cmaxY}}, {{gatedImplantEvent.cminX, gatedImplantEvent.cmaxX}, {gatedImplantEvent.cminY, gatedImplantEvent.cmaxY}}, false ) ) continue;

      // Determine timedifference between implant and decay
      Long64_t timediff = (Long64_t)decayEvent.time - (Long64_t)gatedImplantEvent.time;

      // Skip if implant and decay are not in time window (Shouldn't trigger due to lower and upper bounds)
      if (timediff < 0 || timediff > Global::timeThreshold) continue;

      // Check if decay is in a deadtime
      if (deadtimeWindowManager->Contains(decayEvent.time, decayEvent.x, decayEvent.y)){
        histogramManager->h1_deadtime_implant_vetoed_decay_candidates_time->Fill(decayEvent.time);
        continue;
      }

      // Increase forward match counter
      ++implantDecayMultiplicityForward;
      // Define tags for match
      BetaType betaType = BetaType::CANDIDATE;
      CorrelationType correlationType  = CorrelationType::FORWARDS;

      // Override beta type if a specific match number
      switch (implantDecayMultiplicityForward){
        case 1: {
          // First match 
          foundMatchForward = true;
          betaType = BetaType::MATCH;
          break;
        }
        case 2: {
          betaType = BetaType::SECOND_MATCH;
          break;
        }
        case 3: {
          betaType = BetaType::THIRD_MATCH;
          break;
        }
        case 10: {
          betaType = BetaType::TENTH_MATCH;
          break;
        }
        default: break;
      }

      // Build Matched event and add to vector
      BetaCandidateEvent matchedCandidateEvent = {
        correlationType, betaType, gatedImplantEvent.time, decayEvent.time, gatedImplantEvent.x, gatedImplantEvent.y, gatedImplantEvent.e, 
        {gatedImplantEvent.cminX, gatedImplantEvent.cmaxX}, {gatedImplantEvent.cminY, gatedImplantEvent.cmaxY}, decayEvent.x, decayEvent.y,
        decayEvent.e, {decayEvent.cminX, decayEvent.cmaxX}, {decayEvent.cminY, decayEvent.cmaxY}, decayEvent.spill, timediff
      };
      betaCandidateEventVector.emplace_back(matchedCandidateEvent);

    }


    // Loop over backward direction

    // Find Lower and upper bounds for decays
    decayItrEnd = decayEventMap->lower_bound((Long64_t)gatedImplantEvent.time);
    decayItrStart = decayEventMap->lower_bound((Long64_t)gatedImplantEvent.time - Global::timeThreshold);

    auto rbegin = std::make_reverse_iterator(decayItrStart);
    auto rend = std::make_reverse_iterator(decayItrEnd);

    for (auto decayItr = decayItrStart; decayItr != decayItrEnd; ++decayItr){

      // Unpack gated implant event
      auto& decayEvent = decayItr->second;

      // Veto decay event if overlapping with noisy strip in HEC
      if ( IsNoisyStrip(Global::BROKEN_AIDA_X_STRIPS_DECAY, {decayEvent.cminX, decayEvent.cmaxX}) ) continue;
      if ( IsNoisyStrip(Global::BROKEN_AIDA_Y_STRIPS_DECAY, {decayEvent.cminY, decayEvent.cmaxY}) ) continue;

      // Check if we only analyse offspill
      if ( Global::onlyOffspillDecays && decayEvent.spill == 1) continue;

      // Skip if clusters aren't overlapping
      if ( !AreClustersOverlapping( {{gatedImplantEvent.cminX, gatedImplantEvent.cmaxX}, {gatedImplantEvent.cminY, gatedImplantEvent.cmaxY}}, {{gatedImplantEvent.cminX, gatedImplantEvent.cmaxX}, {gatedImplantEvent.cminY, gatedImplantEvent.cmaxY}}, false ) ) continue;

      // Determine timedifference between implant and decay
      Long64_t timediff = (Long64_t)decayEvent.time - (Long64_t)gatedImplantEvent.time;

      // Skip if implant and decay are not in time window (Shouldn't trigger due to lower and upper bounds)
      if (timediff > 0 || -timediff > Global::timeThreshold) continue;

      // Check if decay is in a deadtime
      if (deadtimeWindowManager->Contains(decayEvent.time, decayEvent.x, decayEvent.y)){
        histogramManager->h1_deadtime_implant_vetoed_decay_candidates_time->Fill(decayEvent.time);
        continue;
      }

      // Increase backward match counter
      ++implantDecayMultiplicityBackward;
      // Define tags for match
      BetaType betaType = BetaType::CANDIDATE;
      CorrelationType correlationType  = CorrelationType::BACKWARDS;

      // Override beta type if a specific match number
      switch (implantDecayMultiplicityForward){
        case 1: {
          // First match 
          foundMatchBackward = true;
          betaType = BetaType::MATCH;
          break;
        }
        case 2: {
          betaType = BetaType::SECOND_MATCH;
          break;
        }
        case 3: {
          betaType = BetaType::THIRD_MATCH;
          break;
        }
        case 10: {
          betaType = BetaType::TENTH_MATCH;
          break;
        }
        default: break;
      }

      // Build Matched event and add to vector
      BetaCandidateEvent matchedCandidateEvent = {
        correlationType, betaType, gatedImplantEvent.time, decayEvent.time, gatedImplantEvent.x, gatedImplantEvent.y, gatedImplantEvent.e, 
        {gatedImplantEvent.cminX, gatedImplantEvent.cmaxX}, {gatedImplantEvent.cminY, gatedImplantEvent.cmaxY}, decayEvent.x, decayEvent.y,
        decayEvent.e, {decayEvent.cminX, decayEvent.cmaxX}, {decayEvent.cminY, decayEvent.cmaxY}, decayEvent.spill, timediff
      };
      betaCandidateEventVector.emplace_back(matchedCandidateEvent);

    }
    
    // Make ImplantDecayCorrelationEvent
    ImplantDecayCorrelationEvent implantDecayCorrelatedEvent = {
      {implantDecayMultiplicityForward, implantDecayMultiplicityBackward},
      betaCandidateEventVector
    };
    implantDecayCorrelatedEventMap.emplace(gatedImplantEvent.time, implantDecayCorrelatedEvent);

  }

}

void CorrelationManager::FillImplantDecayHistograms(){

  // Loop over all implant decay matches
  for (auto& implantDecayCorrelatedItr : implantDecayCorrelatedEventMap){
  
    // Unpack implant decay Correlated event
    auto& implantDecayMatchMultiplicity = implantDecayCorrelatedItr.second.implantDecayMatchMultiplicity;
    auto& betaCandidateEventVector = implantDecayCorrelatedItr.second.betaCandidateEventVector;

    // Fill Multiplicity histograms
    if (implantDecayMatchMultiplicity.forward > 0) histogramManager->h1_implantbeta_candidate_multiplicity_forwards->Fill(implantDecayMatchMultiplicity.forward);
    if (implantDecayMatchMultiplicity.backward > 0) histogramManager->h1_implantbeta_candidate_multiplicity_backwards->Fill(implantDecayMatchMultiplicity.backward);

    // Loop over all beta candidates
    for (auto& betaCandidateEvent : betaCandidateEventVector){
      
      if (betaCandidateEvent.correlationType == CorrelationType::FORWARDS  && betaCandidateEvent.betaType == BetaType::MATCH){
        histogramManager->h1_aida_implant_beta_dt->Fill(betaCandidateEvent.timediff);
        histogramManager->h1_aida_implant_beta_dt_forward->Fill(betaCandidateEvent.timediff);
        histogramManager->nt_aida_implant_beta_dt->Fill((double)betaCandidateEvent.timediff);
        histogramManager->h2_aida_matched_xy->Fill(betaCandidateEvent.decayPositionX, betaCandidateEvent.decayPositionY);
        histogramManager->h1_aida_wr_times->Fill(betaCandidateEvent.decayTime);
      }

      if (betaCandidateEvent.correlationType == CorrelationType::BACKWARDS && betaCandidateEvent.betaType == BetaType::MATCH){
        histogramManager->h1_aida_implant_beta_dt->Fill(betaCandidateEvent.timediff);
        histogramManager->h1_aida_implant_beta_dt_backward->Fill(-betaCandidateEvent.timediff);
        histogramManager->nt_aida_implant_beta_dt->Fill((double)betaCandidateEvent.timediff);
      }
      
      if (betaCandidateEvent.correlationType == CorrelationType::FORWARDS){
        histogramManager->h2_implant_energy_dt_forward->Fill(betaCandidateEvent.implantEnergy, betaCandidateEvent.timediff);
        histogramManager->h2_decay_energy_dt_forward->Fill(betaCandidateEvent.decayEnergy, betaCandidateEvent.timediff);
        histogramManager->h2_strip_dt_forward->Fill(betaCandidateEvent.decayPositionX, betaCandidateEvent.timediff);
        histogramManager->h2_strip_dt_forward->Fill(betaCandidateEvent.decayPositionY+400, betaCandidateEvent.timediff);

        histogramManager->nt_all_candidate_ionbeta_dt->Fill((double)betaCandidateEvent.timediff);
        histogramManager->h1_all_candidate_ionbeta_dt->Fill(betaCandidateEvent.timediff);

        histogramManager->h2_decayenergy_candidatemultiplicity_forwards->Fill(betaCandidateEvent.decayEnergy, implantDecayMatchMultiplicity.forward);
        histogramManager->h2_implantenergy_candidatemultiplicity_forwards->Fill(betaCandidateEvent.implantEnergy, implantDecayMatchMultiplicity.forward);

        double clusterSizeX = betaCandidateEvent.decayClusterX.max - betaCandidateEvent.decayClusterX.min;
        double clusterSizeY = betaCandidateEvent.decayClusterY.max - betaCandidateEvent.decayClusterY.min;
        histogramManager->h2_clustersize_candidatemultiplicity_forwards->Fill( TMath::Sqrt( (double)TMath::Sq(clusterSizeX) + (double)TMath::Sq(clusterSizeY) ), implantDecayMatchMultiplicity.forward);
      }

      if (betaCandidateEvent.correlationType == CorrelationType::BACKWARDS){
        histogramManager->h2_implant_energy_dt_backward->Fill(betaCandidateEvent.implantEnergy, betaCandidateEvent.timediff);
        histogramManager->h2_decay_energy_dt_backward->Fill(betaCandidateEvent.decayEnergy, betaCandidateEvent.timediff);
        histogramManager->h2_strip_dt_backward->Fill(betaCandidateEvent.decayPositionX, betaCandidateEvent.timediff);
        histogramManager->h2_strip_dt_backward->Fill(betaCandidateEvent.decayPositionY+400, betaCandidateEvent.timediff);

        histogramManager->nt_all_candidate_ionbeta_dt->Fill((double)betaCandidateEvent.timediff);
        histogramManager->h1_all_candidate_ionbeta_dt->Fill(betaCandidateEvent.timediff);

        histogramManager->h2_decayenergy_candidatemultiplicity_backwards->Fill(betaCandidateEvent.decayEnergy, implantDecayMatchMultiplicity.forward);
        histogramManager->h2_implantenergy_candidatemultiplicity_backwards->Fill(betaCandidateEvent.implantEnergy, implantDecayMatchMultiplicity.forward);

        double clusterSizeX = betaCandidateEvent.decayClusterX.max - betaCandidateEvent.decayClusterX.min;
        double clusterSizeY = betaCandidateEvent.decayClusterY.max - betaCandidateEvent.decayClusterY.min;
        histogramManager->h2_clustersize_candidatemultiplicity_backwards->Fill( TMath::Sqrt( (double)TMath::Sq(clusterSizeX) + (double)TMath::Sq(clusterSizeY) ), implantDecayMatchMultiplicity.forward);
      }

      // !!!!! ADD BETA CANDIDATADE CUTS AND OTHER CUTS !!!
      
    }

  }

}

void CorrelationManager::CorrelateDecayGermaniums(){

  // Loop over matched implant decay events
  for ( auto& implantDecayCorrelatedItr : implantDecayCorrelatedEventMap ){

    // Define boolians to skip forward or backward matches
    bool skipForward = false;
    bool skipBackward = false;

    // Unpack implant decay Correlated event
    auto& implantDecayMatchMultiplicity = implantDecayCorrelatedItr.second.implantDecayMatchMultiplicity;
    auto& betaCandidateEventVector = implantDecayCorrelatedItr.second.betaCandidateEventVector;

    if ( implantDecayMatchMultiplicity.forward > Global::betaGammaCandidateCut ) skipForward = true;
    if ( implantDecayMatchMultiplicity.backward > Global::betaGammaCandidateCut ) skipBackward = true;

    // Loop over beta candidates
    for ( auto& betaCandidateEvent : betaCandidateEventVector ){

      // Skip candidate if correlation type is above the candidate cut
      if ( skipForward && betaCandidateEvent.correlationType == CorrelationType::FORWARDS ) continue; 
      if ( skipBackward && betaCandidateEvent.correlationType == CorrelationType::BACKWARDS ) continue; 

      // Check if we only analyse offspill
      if ( Global::onlyOffspillDecays && betaCandidateEvent.decaySpill == 1) continue;

      // Define lower bound and upper bounds for loops
      

    }


  }

}


// Public Methods

void CorrelationManager::RunImplantDecayCorrelation(){

  std::cout << "Correlating implants to decays" << std::endl;
  CorrelateImplantDecays();
  std::cout << "Filling implant decay histograms" << std::endl;
  FillImplantDecayHistograms();

}

void CorrelationManager::RunDecayGermaniumCorrelation(){

  std::cout << "Correlating matched decays to germaniums" << std::endl;
  CorrelateDecayGermaniums();
  // std::cout << "Filling decay germanium histograms" << std::endl;
  // FillImplantDecayHistograms();

}