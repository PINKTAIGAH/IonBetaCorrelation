#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include <map>
#include <tuple>
#include <string>

#include "TFile.h"
#include "TObject.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "GlobalConstants.hh"
#include "AnatreeEventStructs.hh"

// Define class constants
namespace TreeManagerConstants{

  // Names of Trees
  const std::string implantTreeName = "aida_implant_tree";
  const std::string decayTreeName = "aida_decay_tree";
  const std::string germaniumTreeName = "germanium_tree";
  const std::string gatedImplantTreeName = "aida_gatedimplant_" + Global::isotopeName + "_tree";

  // Names of gated implant branch
  const std::string gatedImplantBranchName = "gatedimplant_" + Global::isotopeName;

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
    TFile* inputFile;   

    std::unique_ptr<TTreeReader> implantReader;
    std::unique_ptr<TTreeReader> gatedImplantReader;
    std::unique_ptr<TTreeReader> decayReader;
    std::unique_ptr<TTreeReader> germaniumReader;

    std::multimap<ULong64_t, ImplantEvent> implantEventMap;
    std::multimap<ULong64_t, ImplantEvent> gatedImplantEventMap;
    std::multimap<ULong64_t, DecayEvent> decayEventMap;
    std::multimap<ULong64_t, GermaniumEvent> germaniumEventMap;

    // Methods
    void InitialiseReaders();
    void FillMaps();

  public:

    TreeManager(TFile* inputFile);
    void LoadEvents();
    EventMaps GetEventMaps();
    void ClearHeap();
};



#endif