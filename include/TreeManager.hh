#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include <map>
#include <tuple>
#include <string>

#include "TFile.h"
#include "TChain.h"
#include "TObject.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "GlobalConstants.hh"
#include "AnatreeEventStructs.hh"
#include "ConfigReader.hh"

// Define class constants
namespace TreeManagerConstants{

  // Event gate values
  const Int_t dssd = 1;
  const Double_t implantTimeDiff = 5e3;
  const Double_t implantEnergyMin = 3.5e3;
  const Double_t implantEnergyMax = 7e3;
  const Double_t implantEnegyDiff = 1e3;
  const Int_t implantClusterSizeMax = 3;
  const Double_t decayTimeDiff = 5e3;
  const Double_t decayEnergyMin = 150.;
  const Double_t decayEnergyMax = 1000.;
  const Double_t decayEnergyDiff = 168.;
  const Int_t decayClusterSizeMax = 3;
  const Double_t germaniumEnergyMin = 20.;

}

struct EventMaps{
  std::multimap<ULong64_t, ImplantEvent>* implant;
  std::multimap<ULong64_t, ImplantEvent>* gatedImplant;
  std::multimap<ULong64_t, DecayEvent>* decay;
  std::multimap<ULong64_t, GermaniumEvent>* germanium;
};

class TreeManager{
  private:
  
    // Variables
    std::unordered_map<std::string, TChain*> chainMap;
    TFile* inputFile;   

    std::multimap<ULong64_t, ImplantEvent> implantEventMap;
    std::multimap<ULong64_t, ImplantEvent> gatedImplantEventMap;
    std::multimap<ULong64_t, DecayEvent> decayEventMap;
    std::multimap<ULong64_t, GermaniumEvent> germaniumEventMap;

    // Methods
    void FillMaps();

  public:

    TreeManager(std::unordered_map<std::string, TChain*>& chains);
    ~TreeManager() = default;

    void LoadEvents();
    EventMaps GetEventMaps();
};



#endif