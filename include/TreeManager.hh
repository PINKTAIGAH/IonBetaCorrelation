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

// // Define class constants
// namespace TreeManagerConstants{

//   // Event gate values
//   const Double_t implantTimeDiff = 5e3;
//   const Double_t implantEnergyMin = 3.5e3;
//   const Double_t implantEnergyMax = 7e3;
//   const Double_t implantEnegyDiff = 1e3;
//   const Int_t implantClusterSizeMax = 3;
//   const Double_t decayTimeDiff = 5e3;
//   const Double_t decayEnergyMin = 150.;
//   const Double_t decayEnergyMax = 1000.;
//   const Double_t decayEnergyDiff = 168.;
//   const Int_t decayClusterSizeMax = 3;
//   const Double_t germaniumEnergyMin = 20.;

// }

struct EventMaps{
  std::multimap<ULong64_t, ImplantEvent>* implant;
  std::multimap<ULong64_t, ImplantEvent>* gatedImplant;
  std::multimap<ULong64_t, DecayEvent>* decay;
  std::multimap<ULong64_t, GermaniumEvent>* germanium;
};

class TreeManager{
  private:
  
    // Event Constraints
    Double_t implantTimeDiff = ConfigReader::Instance().GetImplantTimeDiff();
    Double_t implantEnergyMin = ConfigReader::Instance().GetImplantEnergyMin();
    Double_t implantEnergyMax = ConfigReader::Instance().GetImplantEnergyMax();
    Double_t implantEnergyDiff = ConfigReader::Instance().GetImplantEnergyDiff();
    Int_t implantClusterSizeMax = ConfigReader::Instance().GetImplantClusterSizeMax();
    Double_t decayTimeDiff = ConfigReader::Instance().GetDecayTimeDiff();
    Double_t decayEnergyMin = ConfigReader::Instance().GetDecayEnergyMin();
    Double_t decayEnergyMax = ConfigReader::Instance().GetDecayEnergyMax();
    Double_t decayEnergyDiff = ConfigReader::Instance().GetDecayEnergyDiff();
    Int_t decayClusterSizeMax = ConfigReader::Instance().GetDecayClusterSizeMax();
    Double_t germaniumEnergyMin = ConfigReader::Instance().GetGermaniumEnergyMin();
  
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