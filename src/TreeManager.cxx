#include "TMath.h"

#include "TreeManager.hh"

// Constructor

TreeManager::TreeManager(TFile* inputFile){
  
  this->inputFile = inputFile;

}



// Private Methods

void TreeManager::ExtractTrees(){
  
  // Extract the trees form the input file
  implantTree = (TTree*)inputFile->Get( TreeManagerConstants::implantTreeName.c_str() );
  gatedImplantTree = (TTree*)inputFile->Get( TreeManagerConstants::gatedImplantTreeName.c_str() );
  decayTree = (TTree*)inputFile->Get( TreeManagerConstants::decayTreeName.c_str() );
  germaniumTree = (TTree*)inputFile->Get( TreeManagerConstants::germaniumTreeName.c_str() );

}

void TreeManager::InitialiseReaders(){

  // Initialise readers
  implantReader = TTreeReader(implantTree);
  gatedImplantReader = TTreeReader(gatedImplantTree);
  decayReader = TTreeReader(decayTree);
  germaniumReader = TTreeReader(germaniumTree);

  // Initialise TTreeReaderValues
  implantTime = TTreeReaderValue(implantReader, "implant.time");
  implantTimeX = TTreeReaderValue(implantReader, "implant.time_x");
  implantTimeY = TTreeReaderValue(implantReader, "implant.time_y");
  implantX = TTreeReaderValue(implantReader, "implant.x");
  implantY = TTreeReaderValue(implantReader, "implant.y");
  implantEnergy = TTreeReaderValue(implantReader, "implant.e");
  implantEnergyX = TTreeReaderValue(implantReader, "implant.ex");
  implantEnergyY = TTreeReaderValue(implantReader, "implant.ey");
  implantClusterSizeX = TTreeReaderValue(implantReader, "implant.csx");
  implantClusterSizeY = TTreeReaderValue(implantReader, "implant.csy");
  implantClusterMinX = TTreeReaderValue(implantReader, "implant.cminx");
  implantClusterMinY = TTreeReaderValue(implantReader, "implant.cminy");
  implantClusterMaxX = TTreeReaderValue(implantReader, "implant.cmaxx");
  implantClusterMaxY = TTreeReaderValue(implantReader, "implant.cmaxy");
  implantDssd = TTreeReaderValue(implantReader, "implant.dssd");
  implantSpill = TTreeReaderValue(implantReader, "implant.sp");

  gatedImplantTime = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".time" ).c_str() );
  gatedImplantTimeX = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".time_x" ).c_str() );
  gatedImplantTimeY = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".time_y" ).c_str() );
  gatedImplantX = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".x" ).c_str() );
  gatedImplantY = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".y" ).c_str() );
  gatedImplantEnergy = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".e" ).c_str() );
  gatedImplantEnergyX = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".ex" ).c_str() );
  gatedImplantEnergyY = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".ey" ).c_str() );
  gatedImplantClusterSizeX = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".csx" ).c_str() );
  gatedImplantClusterSizeY = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".csy" ).c_str() );
  gatedImplantClusterMinX = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cminx" ).c_str() );
  gatedImplantClusterMinY = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cminy" ).c_str() );
  gatedImplantClusterMaxX = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cmaxx" ).c_str() );
  gatedImplantClusterMaxY = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cmaxy" ).c_str() );
  gatedImplantDssd = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".dssd" ).c_str() );
  gatedImplantSpill = TTreeReaderValue(gatedImplantReader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".sp" ).c_str() );

  decayTime = TTreeReaderValue(decayReader, "decay.time");
  decayTimeX = TTreeReaderValue(decayReader, "decay.time_x");
  decayTimeY = TTreeReaderValue(decayReader, "decay.time_y");
  decayX = TTreeReaderValue(decayReader, "decay.x");
  decayY = TTreeReaderValue(decayReader, "decay.y");
  decayEnergy = TTreeReaderValue(decayReader, "decay.e");
  decayEnergyX = TTreeReaderValue(decayReader, "decay.ex");
  decayEnergyY = TTreeReaderValue(decayReader, "decay.ey");
  decayClusterSizeX = TTreeReaderValue(decayReader, "decay.csx");
  decayClusterSizeY = TTreeReaderValue(decayReader, "decay.csy");
  decayClusterMinX = TTreeReaderValue(decayReader, "decay.cminx");
  decayClusterMinY = TTreeReaderValue(decayReader, "decay.cminy");
  decayClusterMaxX = TTreeReaderValue(decayReader, "decay.cmaxx");
  decayClusterMaxY = TTreeReaderValue(decayReader, "decay.cmaxy");
  decayDssd = TTreeReaderValue(decayReader, "decay.dssd");
  decaySpill = TTreeReaderValue(decayReader, "decay.sp");

  germaniumTime = TTreeReaderValue(germaniumReader, "germanium.time");
  germaniumEnergy = TTreeReaderValue(germaniumReader, "germanium.energy");
  germaniumSpill = TTreeReaderValue(germaniumReader, "germanium.sp");

}

void TreeManager::FillMaps(){

  // Load implant map
  while (implantReader.Next()){
    if ( *implantDssd==TreeManagerConstants::dssd && TMath::Abs((Long64_t)*implantTimeX-(Long64_t)*implantTimeY) < TreeManagerConstants::implantTimeDiff ){

      ImplantEvent tempEvent = {
        *implantTime, *implantTimeX, *implantTimeY, *implantX, *implantY, *implantEnergy, *implantEnergyX, *implantEnergyY, *implantClusterSizeX, *implantClusterSizeY,
        *implantClusterMinX, *implantClusterMaxX, *implantClusterMinY, *implantClusterMaxY, *implantDssd, *implantSpill 
      };

      implantEventMap.emplace(*implantTime, tempEvent);

    }
  }

  // Load gated implant map
  while (gatedImplantReader.Next()){
    if ( *implantDssd==TreeManagerConstants::dssd && TMath::Abs((Long64_t)*implantTimeX-(Long64_t)*implantTimeY) < TreeManagerConstants::implantTimeDiff ){

      ImplantEvent tempEvent = {
        *gatedImplantTime, *gatedImplantTimeX, *gatedImplantTimeY, *gatedImplantX, *gatedImplantY, *gatedImplantEnergy, *gatedImplantEnergyX, *gatedImplantEnergyY, *gatedImplantClusterSizeX, *gatedImplantClusterSizeY,
        *gatedImplantClusterMinX, *gatedImplantClusterMaxX, *gatedImplantClusterMinY, *gatedImplantClusterMaxY, *gatedImplantDssd, *gatedImplantSpill 
      };

      gatedImplantEventMap.emplace(*gatedImplantTime, tempEvent);

    }
  }

  // Load decay map
  while (decayReader.Next()){
    if ( *decayDssd == TreeManagerConstants::dssd && TMath::Abs((Long64_t)*decayTimeX-(Long64_t)*decayTimeY) < TreeManagerConstants::decayTimeDiff && TMath::Abs((Long64_t)*decayEnergyX-(Long64_t)*decayEnergyY) < TreeManagerConstants::decayEnergyDiff && *decayEnergy > TreeManagerConstants::decayEnergyMin && *decayEnergy < TreeManagerConstants::decayEnergyMax ){

      DecayEvent tempEvent = {
        *decayTime, *decayTimeX, *decayTimeY, *decayX, *decayY, *decayEnergy, *decayEnergyX, *decayEnergyY, *decayClusterSizeX, *decayClusterSizeY,
        *decayClusterMinX, *decayClusterMaxX, *decayClusterMinY, *decayClusterMaxY, *decayDssd, *decaySpill 
      };

      decayEventMap.emplace(*decayTime, tempEvent);

    }
  }

  // Load germanium map
  while (germaniumReader.Next()){
    if ( *germaniumEnergy > TreeManagerConstants::germaniumEnergyMin ){

      GermaniumEvent tempEvent = { *germaniumTime, *germaniumEnergy, *germaniumSpill };

      germaniumEventMap.emplace(*germaniumTime, tempEvent);

    }
  }


}

// Public Methods

void TreeManager::LoadEvents(){
  
  std::cout << "Extracting trees from anatree..." << std::endl;
  ExtractTrees();
  std::cout << "Initialising TTreeReaders..." << std::endl;
  InitialiseReaders();
  std::cout << "Filling maps with events..." << std::endl;
  FillMaps();
  std::cout << "Events succesfully loaded!" << std::endl;

}

EventMaps TreeManager::GetEventMaps(){

  EventMaps extractedEvents = {&implantEventMap, &gatedImplantEventMap, &decayEventMap, &germaniumEventMap};
  return extractedEvents;

}

void TreeManager::Cleanup(){
  // Clear heap
  delete implantTree;
  delete gatedImplantTree;
  delete decayTree;
  delete germaniumTree;
}