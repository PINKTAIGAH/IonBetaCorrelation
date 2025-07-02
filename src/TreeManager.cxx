#include "TMath.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "TreeManager.hh"
#include "AnatreeEventStructs.hh"

// Constructor

TreeManager::TreeManager(TFile* inputFile){
  
  this->inputFile = inputFile;

}

TreeManager::~TreeManager(){

  // Clean up heap allocated readers
  implantReader.reset();
  gatedImplantReader.reset();
  decayReader.reset();
  germaniumReader.reset();

}


// Private Methods

void TreeManager::InitialiseReaders(){

  // // Initialise readers
  implantReader = std::make_unique<TTreeReader>(TreeManagerConstants::implantTreeName.c_str(), inputFile);
  gatedImplantReader = std::make_unique<TTreeReader>(TreeManagerConstants::gatedImplantTreeName.c_str(), inputFile);
  decayReader = std::make_unique<TTreeReader>(TreeManagerConstants::decayTreeName.c_str(), inputFile);
  germaniumReader = std::make_unique<TTreeReader>(TreeManagerConstants::germaniumTreeName.c_str(), inputFile);


}

void TreeManager::FillMaps(){

  // Clear multimaps in case this method is ran twice
  implantEventMap.clear();
  gatedImplantEventMap.clear();
  decayEventMap.clear();
  germaniumEventMap.clear();

  // Initialise TTreeReaderValues
  TTreeReaderValue<ULong64_t> implantTime(*implantReader, "implant.time");
  TTreeReaderValue<ULong64_t> implantTimeX(*implantReader, "implant.time_x");
  TTreeReaderValue<ULong64_t> implantTimeY(*implantReader, "implant.time_y");
  TTreeReaderValue<Double_t> implantX(*implantReader, "implant.x");
  TTreeReaderValue<Double_t> implantY(*implantReader, "implant.y");
  TTreeReaderValue<Double_t> implantEnergy(*implantReader, "implant.e");
  TTreeReaderValue<Double_t> implantEnergyX(*implantReader, "implant.ex");
  TTreeReaderValue<Double_t> implantEnergyY(*implantReader, "implant.ey");
  TTreeReaderValue<Int_t> implantClusterSizeX(*implantReader, "implant.csx");
  TTreeReaderValue<Int_t> implantClusterSizeY(*implantReader, "implant.csy");
  TTreeReaderValue<Int_t> implantClusterMinX(*implantReader, "implant.cminx");
  TTreeReaderValue<Int_t> implantClusterMinY(*implantReader, "implant.cminy");
  TTreeReaderValue<Int_t> implantClusterMaxX(*implantReader, "implant.cmaxx");
  TTreeReaderValue<Int_t> implantClusterMaxY(*implantReader, "implant.cmaxy");
  TTreeReaderValue<Int_t> implantDssd(*implantReader, "implant.dssd");
  TTreeReaderValue<Int_t> implantSpill(*implantReader, "implant.sp");

  TTreeReaderValue<ULong64_t> gatedImplantTime(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".time" ).c_str() );
  TTreeReaderValue<ULong64_t> gatedImplantTimeX(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".time_x" ).c_str() );
  TTreeReaderValue<ULong64_t> gatedImplantTimeY(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".time_y" ).c_str() );
  TTreeReaderValue<Double_t> gatedImplantX(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".x" ).c_str() );
  TTreeReaderValue<Double_t> gatedImplantY(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".y" ).c_str() );
  TTreeReaderValue<Double_t> gatedImplantEnergy(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".e" ).c_str() );
  TTreeReaderValue<Double_t> gatedImplantEnergyX(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".ex" ).c_str() );
  TTreeReaderValue<Double_t> gatedImplantEnergyY(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".ey" ).c_str() );
  TTreeReaderValue<Int_t> gatedImplantClusterSizeX(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".csx" ).c_str() );
  TTreeReaderValue<Int_t> gatedImplantClusterSizeY(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".csy" ).c_str() );
  TTreeReaderValue<Int_t> gatedImplantClusterMinX(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cminx" ).c_str() );
  TTreeReaderValue<Int_t> gatedImplantClusterMinY(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cminy" ).c_str() );
  TTreeReaderValue<Int_t> gatedImplantClusterMaxX(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cmaxx" ).c_str() );
  TTreeReaderValue<Int_t> gatedImplantClusterMaxY(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cmaxy" ).c_str() );
  TTreeReaderValue<Int_t> gatedImplantDssd(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".dssd" ).c_str() );
  TTreeReaderValue<Int_t> gatedImplantSpill(*gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".sp" ).c_str() );

  TTreeReaderValue<ULong64_t> decayTime(*decayReader, "decay.time");
  TTreeReaderValue<ULong64_t> decayTimeX(*decayReader, "decay.time_x");
  TTreeReaderValue<ULong64_t> decayTimeY(*decayReader, "decay.time_y");
  TTreeReaderValue<Double_t> decayX(*decayReader, "decay.x");
  TTreeReaderValue<Double_t> decayY(*decayReader, "decay.y");
  TTreeReaderValue<Double_t> decayEnergy(*decayReader, "decay.e");
  TTreeReaderValue<Double_t> decayEnergyX(*decayReader, "decay.ex");
  TTreeReaderValue<Double_t> decayEnergyY(*decayReader, "decay.ey");
  TTreeReaderValue<Int_t> decayClusterSizeX(*decayReader, "decay.csx");
  TTreeReaderValue<Int_t> decayClusterSizeY(*decayReader, "decay.csy");
  TTreeReaderValue<Int_t> decayClusterMinX(*decayReader, "decay.cminx");
  TTreeReaderValue<Int_t> decayClusterMinY(*decayReader, "decay.cminy");
  TTreeReaderValue<Int_t> decayClusterMaxX(*decayReader, "decay.cmaxx");
  TTreeReaderValue<Int_t> decayClusterMaxY(*decayReader, "decay.cmaxy");
  TTreeReaderValue<Int_t> decayDssd(*decayReader, "decay.dssd");
  TTreeReaderValue<Int_t> decaySpill(*decayReader, "decay.sp");

  TTreeReaderValue<ULong64_t> germaniumTime(*germaniumReader, "germanium.time");
  TTreeReaderValue<Double_t> germaniumEnergy(*germaniumReader, "germanium.energy");
  TTreeReaderValue<Int_t> germaniumSpill(*germaniumReader, "germanium.sp");

  // Load implant map
  while (implantReader->Next()){
    if ( *implantDssd==TreeManagerConstants::dssd && TMath::Abs((Long64_t)*implantTimeX-(Long64_t)*implantTimeY) < TreeManagerConstants::implantTimeDiff ){

      ImplantEvent tempEvent = {
        *implantTime, *implantTimeX, *implantTimeY, *implantX, *implantY, *implantEnergy, *implantEnergyX, *implantEnergyY, *implantClusterSizeX, *implantClusterSizeY,
        *implantClusterMinX, *implantClusterMaxX+1, *implantClusterMinY, *implantClusterMaxY+1, *implantDssd, *implantSpill 
      }; // We add 1 to max because the max and min are the same values

      implantEventMap.emplace(*implantTime, tempEvent);

    }
  }

  // Load gated implant map
  while (gatedImplantReader->Next()){
    if ( *implantDssd==TreeManagerConstants::dssd && TMath::Abs((Long64_t)*implantTimeX-(Long64_t)*implantTimeY) < TreeManagerConstants::implantTimeDiff ){

      ImplantEvent tempEvent = {
        *gatedImplantTime, *gatedImplantTimeX, *gatedImplantTimeY, *gatedImplantX, *gatedImplantY, *gatedImplantEnergy, *gatedImplantEnergyX, *gatedImplantEnergyY, *gatedImplantClusterSizeX, *gatedImplantClusterSizeY,
        *gatedImplantClusterMinX, *gatedImplantClusterMaxX+1, *gatedImplantClusterMinY, *gatedImplantClusterMaxY+1, *gatedImplantDssd, *gatedImplantSpill 
      }; // We add 1 to max because the max and min are the same values

      gatedImplantEventMap.emplace(*gatedImplantTime, tempEvent);

    }
  }

  // Load decay map
  while (decayReader->Next()){
    if ( *decayDssd == TreeManagerConstants::dssd && TMath::Abs((Long64_t)*decayTimeX-(Long64_t)*decayTimeY) < TreeManagerConstants::decayTimeDiff && TMath::Abs((Long64_t)*decayEnergyX-(Long64_t)*decayEnergyY) < TreeManagerConstants::decayEnergyDiff && *decayEnergy > TreeManagerConstants::decayEnergyMin && *decayEnergy < TreeManagerConstants::decayEnergyMax ){

      DecayEvent tempEvent = {
        *decayTime, *decayTimeX, *decayTimeY, *decayX, *decayY, *decayEnergy, *decayEnergyX, *decayEnergyY, *decayClusterSizeX, *decayClusterSizeY,
        *decayClusterMinX, *decayClusterMaxX+1, *decayClusterMinY, *decayClusterMaxY+1, *decayDssd, *decaySpill 
      }; // We add 1 to max because the max and min are the same values

      decayEventMap.emplace(*decayTime, tempEvent);

    }
  }

  // Load germanium map
  while (germaniumReader->Next()){
    if ( *germaniumEnergy > TreeManagerConstants::germaniumEnergyMin ){

      GermaniumEvent tempEvent = { *germaniumTime, *germaniumEnergy, *germaniumSpill };

      germaniumEventMap.emplace(*germaniumTime, tempEvent);

    }
  }


}

// Public Methods

void TreeManager::LoadEvents(){
  
  std::cout << "Initialising TTreeReaders..." << std::endl;
  InitialiseReaders();
  std::cout << "Filling maps with events..." << std::endl;
  FillMaps();
  std::cout << "Events succesfully loaded!" << std::endl;

}

EventMaps TreeManager::GetEventMaps(){

  EventMaps tempEventMaps {&implantEventMap, &gatedImplantEventMap, &decayEventMap, &germaniumEventMap};
  return tempEventMaps;

}