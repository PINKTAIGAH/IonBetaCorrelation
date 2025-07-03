#include "Logger.hh"
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

  // Assign number of gated implants
  gatedImplantCounter = gatedImplantEventMap->size();
  Logger::Log("Number of " + Global::isotopeName + "in file: " + std::to_string(gatedImplantCounter));

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

  Logger::ScopedTimer localTimer("Correlating implants to decays"); // Time method

  // Forward declaration of vector containing BetaCandidateEnvents
  std::vector<BetaCandidateEvent> betaCandidateEventVector;

  // Initialise beta candidate counter
  betaCandidateCounter = 0;

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
      if ( !AreClustersOverlapping( {{gatedImplantEvent.cminX, gatedImplantEvent.cmaxX}, {gatedImplantEvent.cminY, gatedImplantEvent.cmaxY}}, {{decayEvent.cminX, decayEvent.cmaxX}, {decayEvent.cminY, decayEvent.cmaxY}}, Global::allowAjacentCusters ) ) continue;

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
      ++betaCandidateCounter;
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
      if ( !AreClustersOverlapping( {{gatedImplantEvent.cminX, gatedImplantEvent.cmaxX}, {gatedImplantEvent.cminY, gatedImplantEvent.cmaxY}}, {{decayEvent.cminX, decayEvent.cmaxX}, {decayEvent.cminY, decayEvent.cmaxY}}, Global::allowAjacentCusters ) ) continue;

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
      switch (implantDecayMultiplicityBackward){
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

  Logger::ScopedTimer localTimer("Filling correlated implant decay histograms"); // Time method

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
        histogramManager->h2_strip_dt_forward->Fill(betaCandidateEvent.decayPositionY+400, betaCandidateEvent.timediff);; // Add 400 to skip to part of histo containing y strips

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

  Logger::ScopedTimer localTimer("Correlating matched decays to gammas"); // Time method

  // Initialise matched gamma counter
  matchedGammaCounter = 0;

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
      auto germaniumItrStart = germaniumEventMap->lower_bound(betaCandidateEvent.decayTime - Global::decayGammaWindow.start - 10e3); // 10e3 before start of gamma window
      auto germaniumItrEnd = germaniumEventMap->upper_bound(betaCandidateEvent.decayTime - Global::decayGammaWindow.end + 10e3); // 10e3 after end of gamma window
      
      // Loop over all germaniums in bounds
      for (auto germaniumItr = germaniumItrStart; germaniumItr != germaniumItrEnd; ++germaniumItr){

        // Unpack germanium event & find timediff
        auto& germaniumEvent = germaniumItr->second;
        Long64_t betaGammaTimediff =  (Long64_t)germaniumEvent.time - (Long64_t)betaCandidateEvent.decayTime;

        // Fill histos
        histogramManager->h2_gatedimplantbetagamma_betagamma_dt_vs_implantbeta_dt->Fill(betaCandidateEvent.timediff, betaGammaTimediff);
        if (betaCandidateEvent.correlationType == CorrelationType::FORWARDS){
          histogramManager->h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_forwardmatch->Fill(betaGammaTimediff);
          histogramManager->h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_forwardmatch->Fill(betaGammaTimediff, germaniumEvent.e);
        }
        if (betaCandidateEvent.correlationType == CorrelationType::BACKWARDS){
          histogramManager->h1_gatedimplantbetagamma_spectrum_after_ionbeta_dt_backwardmatch->Fill(betaGammaTimediff);
          histogramManager->h2_gatedimplantbetagamma_spectrum_after_ionbeta_dt_energy_backwardmatch->Fill(betaGammaTimediff, germaniumEvent.e);
        }

        // Check if germanium event is in prompt window
        if ( ! (betaGammaTimediff > Global::decayGammaWindow.start && betaGammaTimediff < Global::decayGammaWindow.end) ) continue;

        // Forward Correlation
        if (betaCandidateEvent.correlationType == CorrelationType::FORWARDS){
        
          // Assign gamma to betaCandidate event
          betaCandidateEvent.SetGammaParameters(germaniumEvent.e);
          ++matchedGammaCounter;
        
          // Fill histos
          histogramManager->h1_gatedimplantbetagamma_spectrum_after_ionbeta_forwardmatch->Fill(germaniumEvent.e);
          histogramManager->h2_gatedimplantbetagamma_energy_vs_decaystrip_forwardmatch->Fill(betaCandidateEvent.decayPositionX, germaniumEvent.e);
          histogramManager->h2_gatedimplantbetagamma_energy_vs_decaystrip_forwardmatch->Fill(betaCandidateEvent.decayPositionY + 400, germaniumEvent.e); // Add 400 to skip to part of histo containing y strips

          // Define lower bound and upper bounds for second germanium loop
          auto otherGermaniumItrStart = germaniumEventMap->lower_bound(betaCandidateEvent.decayTime - Global::decayGammaWindow.start - 10e3); // 10e3 before start of gamma window
          auto otherGermaniumItrEnd = germaniumEventMap->upper_bound(betaCandidateEvent.decayTime - Global::decayGammaWindow.end + 10e3); // 10e3 after end of gamma window

          // Perform Gamma-Gamma Correlation
          for (auto otherGermaniumItr = otherGermaniumItrStart; otherGermaniumItr != otherGermaniumItrEnd; ++otherGermaniumItr){
            
            // Skip if both germanium events are the same gamma
            if (germaniumItr == otherGermaniumItr) continue;

            // Unapack other germanium event
            auto& otherGermaniumEvent = otherGermaniumItr->second;
            Long64_t gammaGammaTimediff = (Long64_t)otherGermaniumEvent.time - (Long64_t)germaniumEvent.time;

            histogramManager->h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_forwardmatch->Fill(germaniumEvent.e, gammaGammaTimediff);

            // Fill GammaGamma Matrix if inside time window
            if (gammaGammaTimediff > Global::gammaGammaWindow.start && gammaGammaTimediff < Global::gammaGammaWindow.end) histogramManager->h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_forwardmatch->Fill(germaniumEvent.e, otherGermaniumEvent.e);

          }
        
        }

        // Backward Correlation
        if (betaCandidateEvent.correlationType == CorrelationType::BACKWARDS){
        
          // Fill histos
          histogramManager->h1_gatedimplantbetagamma_spectrum_after_ionbeta_backwardmatch->Fill(germaniumEvent.e);
          histogramManager->h2_gatedimplantbetagamma_energy_vs_decaystrip_backwardmatch->Fill(betaCandidateEvent.decayPositionX, germaniumEvent.e);
          histogramManager->h2_gatedimplantbetagamma_energy_vs_decaystrip_backwardmatch->Fill(betaCandidateEvent.decayPositionY + 400, germaniumEvent.e); // Add 400 to skip to part of histo containing y strips

          // Define lower bound and upper bounds for second germanium loop
          auto otherGermaniumItrStart = germaniumEventMap->lower_bound(betaCandidateEvent.decayTime - Global::decayGammaWindow.start - 10e3); // 10e3 before start of gamma window
          auto otherGermaniumItrEnd = germaniumEventMap->upper_bound(betaCandidateEvent.decayTime - Global::decayGammaWindow.end + 10e3); // 10e3 after end of gamma window

          // Perform Gamma-Gamma Correlation
          for (auto otherGermaniumItr = otherGermaniumItrStart; otherGermaniumItr != otherGermaniumItrEnd; ++otherGermaniumItr){
            
            // Skip if both germanium events are the same gamma
            if (germaniumItr == otherGermaniumItr) continue;

            // Unapack other germanium event
            auto& otherGermaniumEvent = otherGermaniumItr->second;
            Long64_t gammaGammaTimediff = (Long64_t)otherGermaniumEvent.time - (Long64_t)germaniumEvent.time;

            histogramManager->h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_time_backwardmatch->Fill(germaniumEvent.e, gammaGammaTimediff);

            // Fill GammaGamma Matrix if inside time window
            if (gammaGammaTimediff > Global::gammaGammaWindow.start && gammaGammaTimediff < Global::gammaGammaWindow.end) histogramManager->h2_gatedimplantbetagamma_spectrum_after_ionbeta_square_backwardmatch->Fill(germaniumEvent.e, otherGermaniumEvent.e);

          }

        }

      }

    }

  }

}


// Public Methods
void CorrelationManager::RunImplantDecayCorrelation(){

  Logger::Log("Correlating implants to decays");
  CorrelateImplantDecays();
  Logger::Log("Number of beta candidates identified: " + std::to_string(betaCandidateCounter));
  Logger::Log("Filling implant decay histograms");
  FillImplantDecayHistograms();

}

void CorrelationManager::RunDecayGermaniumCorrelation(){

  Logger::Log("Correlating matched decays to germaniums");
  CorrelateDecayGermaniums();
  Logger::Log("Number of matched gammas identified: " + std::to_string(matchedGammaCounter));

}