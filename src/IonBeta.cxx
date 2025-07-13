#include <map>
#include <string>

#include "TFile.h"
#include "TChain.h"

#include "Logger.hh"
#include "IonBeta.hh"
#include "TreeManager.hh"
#include "HistogramManager.hh"
#include "CorrelationManager.hh"
#include "ConfigReader.hh"
#include "ArgumentParser.hh"



void IonBeta(){

  // *************************************************************************************
  // ************************************* OPEN FILES ************************************
  // *************************************************************************************

  // Make TChain Objects for each tree
  TChain* implantChain = new TChain("aida_implant_tree");
  TChain* gatedImplantChain = new TChain( ("aida_gatedimplant_" + ConfigReader::Instance().GetIsotopeName() + "_tree").c_str());
  TChain* decayChain = new TChain("aida_decay_tree");
  TChain* germaniumChain = new TChain("germanium_tree");

  std::unordered_map<std::string, TChain*> chainMap = { {"implant", implantChain}, {"gatedImplant", gatedImplantChain}, {"decay", decayChain}, {"germanium", germaniumChain} };

  // Fill chains with input files
  for (auto& itr : chainMap ){
    for ( auto& fileName : ArgumentParser::Instance().GetPositionalArgs() ){
      itr.second->Add(fileName.c_str());
    }
  }

  // Open output file
  TFile* outputFile = new TFile(ArgumentParser::Instance().GetValue("O").c_str(), "RECREATE");
  if (!outputFile){
    delete outputFile;
    Logger::Log("Could not create output file ", Logger::Level::FATAL);
    std::exit(1);
  }
  Logger::Log( "Output file loaded: " + (std::string)outputFile->GetName() );

  // *************************************************************************************
  // ******************************** INITAILISE HISTOGRAMS ******************************
  // *************************************************************************************

  HistogramManager* histoManager = new HistogramManager(outputFile);

  // *************************************************************************************
  // ******************************** EXTRACT DATA FROM ANATREES *************************
  // *************************************************************************************

  TreeManager* treeManager = new TreeManager(chainMap); 
  treeManager->LoadEvents();
  EventMaps eventMaps = treeManager->GetEventMaps();

  // *************************************************************************************
  // ********************************* MAKE CORRELATION **********************************
  // *************************************************************************************

  CorrelationManager* correlationManager = new CorrelationManager(eventMaps, histoManager);
  correlationManager->RunImplantDecayCorrelation();
  correlationManager->RunDecayGermaniumCorrelation();

  // *************************************************************************************
  // ***************************** WRITE HISTOGRAMS **************************************
  // *************************************************************************************

  histoManager->WriteHistograms();

  // *************************************************************************************
  // ************************************** CLEANUP **************************************
  // *************************************************************************************

  delete correlationManager;

  delete treeManager;

  delete histoManager;
      
  // inputFile->Close();
  outputFile->Close();

  // delete inputFile;
  delete outputFile;

  delete implantChain;
  delete gatedImplantChain;
  delete decayChain;
  delete germaniumChain;
}