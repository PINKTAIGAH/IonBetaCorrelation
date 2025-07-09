#include <mutex>

#include "TMath.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include <ROOT/TTreeProcessorMT.hxx>

#include "Logger.hh"
#include "TreeManager.hh"
#include "AnatreeEventStructs.hh"

// Constructor

TreeManager::TreeManager(std::unordered_map<std::string, TChain*>& chains){
  this->chainMap = chains;
}

// Private Methods


void TreeManager::FillMaps(){

  Logger::ScopedTimer localTimer("Filling maps"); // Time method

  // Clear multimaps in case this method is ran twice
  implantEventMap.clear();
  gatedImplantEventMap.clear();
  decayEventMap.clear();
  germaniumEventMap.clear();

  std::mutex implantMutex;
  std::mutex gatedImplantMutex;
  std::mutex decayMutex;
  std::mutex germaniumMutex;

  // Define thread process as  lambdas
  auto FillImplantMapProcess = [&](TTreeReader& reader) {

    // Define thread local map
    std::multimap<ULong64_t, ImplantEvent> localMap;

    // Initialise TTreeReaderValues
    TTreeReaderValue<ULong64_t> implantTime(reader, "implant.time");
    TTreeReaderValue<ULong64_t> implantTimeX(reader, "implant.time_x");
    TTreeReaderValue<ULong64_t> implantTimeY(reader, "implant.time_y");
    TTreeReaderValue<Double_t> implantX(reader, "implant.x");
    TTreeReaderValue<Double_t> implantY(reader, "implant.y");
    TTreeReaderValue<Double_t> implantEnergy(reader, "implant.e");
    TTreeReaderValue<Double_t> implantEnergyX(reader, "implant.ex");
    TTreeReaderValue<Double_t> implantEnergyY(reader, "implant.ey");
    TTreeReaderValue<Int_t> implantClusterSizeX(reader, "implant.csx");
    TTreeReaderValue<Int_t> implantClusterSizeY(reader, "implant.csy");
    TTreeReaderValue<Int_t> implantClusterMinX(reader, "implant.cminx");
    TTreeReaderValue<Int_t> implantClusterMinY(reader, "implant.cminy");
    TTreeReaderValue<Int_t> implantClusterMaxX(reader, "implant.cmaxx");
    TTreeReaderValue<Int_t> implantClusterMaxY(reader, "implant.cmaxy");
    TTreeReaderValue<Int_t> implantDssd(reader, "implant.dssd");
    TTreeReaderValue<Int_t> implantSpill(reader, "implant.sp");

    // Load implant map
    while (reader.Next()){
      if ( *implantDssd==TreeManagerConstants::dssd && TMath::Abs((Long64_t)*implantTimeX-(Long64_t)*implantTimeY) < TreeManagerConstants::implantTimeDiff ){

        ImplantEvent tempEvent = {
          *implantTime, *implantTimeX, *implantTimeY, *implantX, *implantY, *implantEnergy, *implantEnergyX, *implantEnergyY, *implantClusterSizeX, *implantClusterSizeY,
          *implantClusterMinX, *implantClusterMaxX+1, *implantClusterMinY, *implantClusterMaxY+1, *implantDssd, *implantSpill 
        }; // We add 1 to max because the max and min are the same values

        localMap.emplace(*implantTime, tempEvent);

      }
    }

    // Lock and fill implant map
    {
      std::lock_guard<std::mutex> lock(implantMutex);
      for (const auto& [key, value] : localMap){ implantEventMap.emplace(key, value); }
    }

  };

  auto FillGatedImplantMapProcess = [&](TTreeReader& reader){

    // Define thread local map
    std::multimap<ULong64_t, ImplantEvent> localMap;

    // Initialise TTreeReaderValues
    TTreeReaderValue<ULong64_t> gatedImplantTime(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".time" ).c_str() );
    TTreeReaderValue<ULong64_t> gatedImplantTimeX(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".time_x" ).c_str() );
    TTreeReaderValue<ULong64_t> gatedImplantTimeY(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".time_y" ).c_str() );
    TTreeReaderValue<Double_t> gatedImplantX(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".x" ).c_str() );
    TTreeReaderValue<Double_t> gatedImplantY(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".y" ).c_str() );
    TTreeReaderValue<Double_t> gatedImplantEnergy(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".e" ).c_str() );
    TTreeReaderValue<Double_t> gatedImplantEnergyX(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".ex" ).c_str() );
    TTreeReaderValue<Double_t> gatedImplantEnergyY(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".ey" ).c_str() );
    TTreeReaderValue<Int_t> gatedImplantClusterSizeX(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".csx" ).c_str() );
    TTreeReaderValue<Int_t> gatedImplantClusterSizeY(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".csy" ).c_str() );
    TTreeReaderValue<Int_t> gatedImplantClusterMinX(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cminx" ).c_str() );
    TTreeReaderValue<Int_t> gatedImplantClusterMinY(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cminy" ).c_str() );
    TTreeReaderValue<Int_t> gatedImplantClusterMaxX(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cmaxx" ).c_str() );
    TTreeReaderValue<Int_t> gatedImplantClusterMaxY(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".cmaxy" ).c_str() );
    TTreeReaderValue<Int_t> gatedImplantDssd(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".dssd" ).c_str() );
    TTreeReaderValue<Int_t> gatedImplantSpill(reader, ( TreeManagerConstants::gatedImplantBranchName + (std::string)".sp" ).c_str() );

    // Load gated implant map
    while (reader.Next()){
      if ( *gatedImplantDssd==TreeManagerConstants::dssd && TMath::Abs((Long64_t)*gatedImplantTimeX-(Long64_t)*gatedImplantTimeY) < TreeManagerConstants::implantTimeDiff ){

        ImplantEvent tempEvent = {
          *gatedImplantTime, *gatedImplantTimeX, *gatedImplantTimeY, *gatedImplantX, *gatedImplantY, *gatedImplantEnergy, *gatedImplantEnergyX, *gatedImplantEnergyY, *gatedImplantClusterSizeX, *gatedImplantClusterSizeY,
          *gatedImplantClusterMinX, *gatedImplantClusterMaxX+1, *gatedImplantClusterMinY, *gatedImplantClusterMaxY+1, *gatedImplantDssd, *gatedImplantSpill 
        }; // We add 1 to max because the max and min are the same values

        localMap.emplace(*gatedImplantTime, tempEvent);

      }
    }

    // Lock and fill gated implant map
    {
      std::lock_guard<std::mutex> lock(gatedImplantMutex);
      for (const auto& [key, value] : localMap){ gatedImplantEventMap.emplace(key, value); }
    }

  };

  auto FillDecaysMapProcess = [&](TTreeReader& reader){

    // Define thread local map
    std::multimap<ULong64_t, DecayEvent> localMap;

    // Initialise TTreeReaderValues
    TTreeReaderValue<ULong64_t> decayTime(reader, "decay.time");
    TTreeReaderValue<ULong64_t> decayTimeX(reader, "decay.time_x");
    TTreeReaderValue<ULong64_t> decayTimeY(reader, "decay.time_y");
    TTreeReaderValue<Double_t> decayX(reader, "decay.x");
    TTreeReaderValue<Double_t> decayY(reader, "decay.y");
    TTreeReaderValue<Double_t> decayEnergy(reader, "decay.e");
    TTreeReaderValue<Double_t> decayEnergyX(reader, "decay.ex");
    TTreeReaderValue<Double_t> decayEnergyY(reader, "decay.ey");
    TTreeReaderValue<Int_t> decayClusterSizeX(reader, "decay.csx");
    TTreeReaderValue<Int_t> decayClusterSizeY(reader, "decay.csy");
    TTreeReaderValue<Int_t> decayClusterMinX(reader, "decay.cminx");
    TTreeReaderValue<Int_t> decayClusterMinY(reader, "decay.cminy");
    TTreeReaderValue<Int_t> decayClusterMaxX(reader, "decay.cmaxx");
    TTreeReaderValue<Int_t> decayClusterMaxY(reader, "decay.cmaxy");
    TTreeReaderValue<Int_t> decayDssd(reader, "decay.dssd");
    TTreeReaderValue<Int_t> decaySpill(reader, "decay.sp");

    // Load decay map
    while (reader.Next()){
      if ( *decayDssd == TreeManagerConstants::dssd && TMath::Abs((Long64_t)*decayTimeX-(Long64_t)*decayTimeY) < TreeManagerConstants::decayTimeDiff && TMath::Abs((Long64_t)*decayEnergyX-(Long64_t)*decayEnergyY) < TreeManagerConstants::decayEnergyDiff && *decayEnergy > TreeManagerConstants::decayEnergyMin && *decayEnergy < TreeManagerConstants::decayEnergyMax ){

        DecayEvent tempEvent = {
          *decayTime, *decayTimeX, *decayTimeY, *decayX, *decayY, *decayEnergy, *decayEnergyX, *decayEnergyY, *decayClusterSizeX, *decayClusterSizeY,
          *decayClusterMinX, *decayClusterMaxX+1, *decayClusterMinY, *decayClusterMaxY+1, *decayDssd, *decaySpill 
        }; // We add 1 to max because the max and min are the same values

        localMap.emplace(*decayTime, tempEvent);

      }
    }

    // Lock and fill decay map
    {
      std::lock_guard<std::mutex> lock(decayMutex);
      for (const auto& [key, value] : localMap){ decayEventMap.emplace(key, value); }
    }

  };

  auto FillGermaniumMapProcess = [&](TTreeReader& reader){

    // Define thread local map
    std::multimap<ULong64_t,GermaniumEvent> localMap;

    TTreeReaderValue<ULong64_t> germaniumTime(reader, "germanium.time");
    TTreeReaderValue<Double_t> germaniumEnergy(reader, "germanium.energy");
    TTreeReaderValue<Int_t> germaniumSpill(reader, "germanium.sp");

    // Load germanium map
    while (reader.Next()){
      if ( *germaniumEnergy > TreeManagerConstants::germaniumEnergyMin ){

        GermaniumEvent tempEvent = { *germaniumTime, *germaniumEnergy, *germaniumSpill };

        localMap.emplace(*germaniumTime, tempEvent);

      }
    }

    // Lock and fill germanium map
    {
      std::lock_guard<std::mutex> lock(germaniumMutex);
      for (const auto& [key, value] : localMap){ germaniumEventMap.emplace(key, value); }
    }

  };

  // Enable implicit multithreading 
  ROOT::EnableImplicitMT();

  // Process Implant Tree
  ROOT::TTreeProcessorMT implantProcessor(*chainMap["implant"]);
  implantProcessor.Process(FillImplantMapProcess);
  
  // Process gated implant Tree
  ROOT::TTreeProcessorMT gatedImplantProcessor(*chainMap["gatedImplant"]);
  gatedImplantProcessor.Process(FillGatedImplantMapProcess);

  // Process decay Tree
  ROOT::TTreeProcessorMT decayProcessor(*chainMap["decay"]);
  decayProcessor.Process(FillDecaysMapProcess);

  // Process decay Tree
  ROOT::TTreeProcessorMT germaniumProcessor(*chainMap["germanium"]);
  germaniumProcessor.Process(FillGermaniumMapProcess);

}

// Public Methods

void TreeManager::LoadEvents(){
  
  Logger::Log("Filling maps with events...");
  FillMaps();
  Logger::Log("Events succesfully loaded!");

}

EventMaps TreeManager::GetEventMaps(){

  EventMaps tempEventMaps {&implantEventMap, &gatedImplantEventMap, &decayEventMap, &germaniumEventMap};
  return tempEventMaps;

}