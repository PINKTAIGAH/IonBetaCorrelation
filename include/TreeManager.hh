#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include <map>
#include <tuple>
#include <string>

#include "TFile.h"
#include "TObject.h"

#include "AnatreeEventStructs.hh"

// Define class constants
namespace TreeManagerConstants{
  // Isotope name which is read
  constexpr std::string isotopeName = "82nb";

  // Names of Trees
  constexpr std::string implantTreeName = "aida_implant_tree";
  constexpr std::string decayTreeName = "aida_decay_tree";
  constexpr std::string germaniumTreeName = "germanium_tree";
  constexpr std::string gatedImplantTreeName = "aida_gatedimplant_" + isotopeName + "_tree";

  // Names of gated implant branch
  constexpr std::string gatedImplantBranchName = "aida_gatedimplant_" + isotopeName;

  // Event gate values
  constexpr Int_t dssd = 1;
  constexpr Double_t implantTimeDiff = 5e3;
  constexpr Double_t implantEnergyMin = 3.5e3;
  constexpr Double_t implantEnergyMax = 7e3;
  constexpr Double_t implantEnegyDiff = 1e3;
  constexpr Int_t implantClusterSizeMax = 3;
  constexpr Double_t decayTimeDiff = 5e3;
  constexpr Double_t decayEnergyMin = 150.;
  constexpr Double_t decayEnergyMax = 1000.;
  constexpr Double_t decayEnegyDiff = 168.;
  constexpr Int_t decayClusterSizeMax = 3;
  constexpr Double_t germaniumEnergyMin = 20.;

}

// Define struct to return event multimaps
struct EventMaps {
  std::multimap<ULong64_t, ImplantEvent>* implant;
  std::multimap<ULong64_t, ImplantEvent>* gatedImplant;
  std::multimap<ULong64_t, DecayEvent>* decay;
  std::multimap<ULong64_t, GermaniumEvent>* germanium;
};

class TreeManager{
  private:
  
    // Variables
    TFile* inputFile;   

    TTree* implantTree;
    TTree* gatedImplantTree;
    TTree* decayTree;
    TTree* germaniumTree;

    TTreeReader implantReader;
    TTreeReader gatedImplantReader;
    TTreeReader decayReader;
    TTreeReader germaniumReader;

    std::multimap<ULong64_t, ImplantEvent> implantEventMap;
    std::multimap<ULong64_t, ImplantEvent> gatedImplantEventMap;
    std::multimap<ULong64_t, DecayEvent> decayEventMap;
    std::multimap<ULong64_t, GermaniumEvent> germaniumEventMap;

    TTreeReaderValue<ULong64_t> implantTime;
    TTreeReaderValue<ULong64_t> implantTimeX;
    TTreeReaderValue<ULong64_t> implantTimeY;
    TTreeReaderValue<Double_t> implantX;
    TTreeReaderValue<Double_t> implantY;
    TTreeReaderValue<Double_t> implantEnergy;
    TTreeReaderValue<Double_t> implantEnergyX;
    TTreeReaderValue<Double_t> implantEnergyY;
    TTreeReaderValue<Double_t> implantClusterSizeX;
    TTreeReaderValue<Double_t> implantClusterSizeY;
    TTreeReaderValue<Int_t> implantClusterMinX;
    TTreeReaderValue<Int_t> implantClusterMinY;
    TTreeReaderValue<Int_t> implantClusterMaxX;
    TTreeReaderValue<Int_t> implantClusterMaxY;
    TTreeReaderValue<Int_t> implantDssd;
    TTreeReaderValue<Int_t> implantSpill;

    TTreeReaderValue<ULong64_t> gatedImplantTime;
    TTreeReaderValue<ULong64_t> gatedImplantTimeX;
    TTreeReaderValue<ULong64_t> gatedImplantTimeY;
    TTreeReaderValue<Double_t> gatedImplantX;
    TTreeReaderValue<Double_t> gatedImplantY;
    TTreeReaderValue<Double_t> gatedImplantEnergy;
    TTreeReaderValue<Double_t> gatedImplantEnergyX;
    TTreeReaderValue<Double_t> gatedImplantEnergyY;
    TTreeReaderValue<Double_t> gatedImplantClusterSizeX;
    TTreeReaderValue<Double_t> gatedImplantClusterSizeY;
    TTreeReaderValue<Int_t> gatedImplantClusterMinX;
    TTreeReaderValue<Int_t> gatedImplantClusterMinY;
    TTreeReaderValue<Int_t> gatedImplantClusterMaxX;
    TTreeReaderValue<Int_t> gatedImplantClusterMaxY;
    TTreeReaderValue<Int_t> gatedImplantDssd;
    TTreeReaderValue<Int_t> gatedImplantSpill;

    TTreeReaderValue<ULong64_t> decayTime;
    TTreeReaderValue<ULong64_t> decayTimeX;
    TTreeReaderValue<ULong64_t> decayTimeY;
    TTreeReaderValue<Double_t> decayX;
    TTreeReaderValue<Double_t> decayY;
    TTreeReaderValue<Double_t> decayEnergy;
    TTreeReaderValue<Double_t> decayEnergyX;
    TTreeReaderValue<Double_t> decayEnergyY;
    TTreeReaderValue<Double_t> decayClusterSizeX;
    TTreeReaderValue<Double_t> decayClusterSizeY;
    TTreeReaderValue<Int_t> decayClusterMinX;
    TTreeReaderValue<Int_t> decayClusterMinY;
    TTreeReaderValue<Int_t> decayClusterMaxX;
    TTreeReaderValue<Int_t> decayClusterMaxY;
    TTreeReaderValue<Int_t> decayDssd;
    TTreeReaderValue<Int_t> decaySpill;

    TTreeReaderValue<ULong64_t> germaniumTime;
    TTreeReaderValue<Double_t> germaniumEnergy;
    TTreeReaderValue<Int_t> germaniumSpill;

    // Methods
    void ExtractTrees();
    void InitialiseReaders();
    void FillMaps();

  public:

    TreeManager(TFile* inputFile);
    void LoadEvents();
    EventMaps GetEventMaps();
    void Cleanup();
};



#endif