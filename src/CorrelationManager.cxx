#include "CorrelationManager.hh"

// Beta Candidate Struct

void BetaCandidateEvent::SetGammaParameters(Double_t gammaEnergy){
  delayedGammaEnergy = gammaEnergy;
  delayedGammaMatch = true;
}

bool BetaCandidateEvent::HasMatchedGammaInRange(std::set<Double_t> knownGammaSet, Double_t gammaRange){
  if (!delayedGammaMatch) return false;
  return knownGammaSet.lower_bound(delayedGammaEnergy-gammaRange) != knownGammaSet.upper_bound(delayedGammaEnergy-gammaRange);
}

// Correlation Manager

CorrelationManager::CorrelationManager(EventMaps eventMaps, HistogramManager* histogramManager){

  // Assign the unpacked event maps
  implantEventMap = eventMaps.implant;
  gatedImplantEventMap = eventMaps.gatedImplant;
  decayEventMap = eventMaps.decay;
  germaniumEventMap = eventMaps.germanium;

  // Assign the histogram manager
  this->histogramManager = histogramManager;

}