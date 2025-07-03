#ifndef CORRELATION_MANAGER_H
#define CORRELATION_MANAGER_H

#include <set>
#include <memory>

#include "TObject.h"

#include "utils.hh"
#include "TreeManager.hh"
#include "HistogramManager.hh"
#include "TimeRangeManager.hh"

// *************************************************************************************
// ****************************** DEFINE CUSTOM ENUMS *****************************
// *************************************************************************************

enum class EventType { GATEDIMPLANT, IMPLANT, DECAY }; // Tags for event type
enum class CorrelationType { FORWARDS, BACKWARDS }; // Tags is ionbeta match is a borwards or backwards time correlations
enum class BetaType { CANDIDATE, MATCH, SECOND_MATCH, THIRD_MATCH, TENTH_MATCH }; // Tags for candidate betas (Have they been matched to an implant or not)
enum class InterruptedCoincidenceType { NOT_INTERRUPTED, INTERRUPTED }; // Tags to define if an implant has beenn interrupted in a time window

// *************************************************************************************
// ****************************** DEFINE STRUCTS *****************************
// *************************************************************************************

struct BetaCandidateEvent{

  CorrelationType correlationType;
  BetaType betaType;
  ULong64_t implantTime;
  ULong64_t decayTime;
  Double_t implantPositionX;
  Double_t implantPositionY;
  Double_t implantEnergy;
  ClusterRange implantClusterX;
  ClusterRange implantClusterY;
  Double_t decayPositionX;
  Double_t decayPositionY;
  Double_t decayEnergy;
  ClusterRange decayClusterX;
  ClusterRange decayClusterY;
  Int_t decaySpill;
  Long64_t timediff;

  bool delayedGammaMatch = false;
  Double_t delayedGammaEnergy;

  void SetGammaParameters(Double_t gammaEnergy);
  bool HasMatchedGammaInRange(std::set<Double_t> knownGammaSet, Double_t gammaRange);

};


struct ImplantDecayCorrelationEvent{
  ForwardBackwardPair implantDecayMatchMultiplicity;
  std::vector<BetaCandidateEvent> betaCandidateEventVector;
};


// *************************************************************************************
// ****************************** DEFINE CLASS *****************************
// *************************************************************************************

class CorrelationManager {

  private:
    // Class constructor arguments
    HistogramManager* histogramManager;
    std::multimap<ULong64_t, ImplantEvent>* implantEventMap;
    std::multimap<ULong64_t, ImplantEvent>* gatedImplantEventMap;
    std::multimap<ULong64_t, DecayEvent>* decayEventMap;
    std::multimap<ULong64_t, GermaniumEvent>* germaniumEventMap;
    std::unique_ptr<TimeRangeManagerLocal> deadtimeWindowManager;
    Int_t gatedImplantCounter;

    // Class Members
    std::multimap<ULong64_t, ImplantDecayCorrelationEvent> implantDecayCorrelatedEventMap;
    Int_t betaCandidateCounter;
    Int_t matchedGammaCounter;

    // Private Methods
    void InitialiseDeadtimeWindowManager();
    void TagImplants();
    void CorrelateImplantDecays();
    void FillImplantDecayHistograms();
    void CorrelateDecayGermaniums();


  public:

    CorrelationManager(EventMaps eventMaps, HistogramManager* histogramManager);
    ~CorrelationManager() = default;
    void RunImplantDecayCorrelation();
    void RunDecayGermaniumCorrelation();


};


#endif