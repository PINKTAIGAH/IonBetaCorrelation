#include <string>

#include "TFile.h"

#include "Logger.hh"
#include "TreeManager.hh"
#include "HistogramManager.hh"
#include "CorrelationManager.hh"


void IonBeta(const char* inputFileName, const char* outputFileName){

  // *************************************************************************************
  // ************************************* OPEN FILES ************************************
  // *************************************************************************************

  // Open input file
  TFile* inputFile = TFile::Open(inputFileName);
  if (!inputFile){
    delete inputFile; // Clean up
    Logger::Log("Could not open file ", Logger::Level::FATAL);
    std::exit(1);
  }
  Logger::Log( "File loaded: " + (std::string)inputFile->GetName() );

  // Open output file
  TFile* outputFile = new TFile(outputFileName, "RECREATE");
  if (!outputFile){
    delete outputFile;
    Logger::Log("Could not create output file ", Logger::Level::FATAL);
    std::exit(1);
  }

  // *************************************************************************************
  // ******************************** INITAILISE HISTOGRAMS ******************************
  // *************************************************************************************

  HistogramManager* histoManager = new HistogramManager(outputFile);

  // *************************************************************************************
  // ******************************** EXTRACT DATA FROM ANATREES *************************
  // *************************************************************************************

  TreeManager* treeManager = new TreeManager(inputFile); 
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
      
  inputFile->Close();
  outputFile->Close();

  delete inputFile;
  delete outputFile;
}