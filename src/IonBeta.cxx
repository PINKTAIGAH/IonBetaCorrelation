#include <iostream>

#include "TFile.h"

#include "TreeManager.hh"
#include "HistogramManager.hh"


void IonBeta(const char* inputFileName, const char* outputFileName){

  // *************************************************************************************
  // ************************************* OPEN FILES ************************************
  // *************************************************************************************

  // Open input file
  TFile* inputFile = TFile::Open(inputFileName);
  if (!inputFile){
    delete inputFile; // Clean up
    std::cerr << "Error: Could not open file " << std::endl;
   std::exit(1);
  }
  std::cout << "\nFile loaded: "<< inputFile->GetName() << '\n';

  // Open output file
  TFile* outputFile = new TFile(outputFileName, "RECREATE");
  if (!outputFile){
    delete outputFile;
    std::cerr << "Error: Could not create output file " << std::endl;
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
  // ************************************** DEBUG **************************************
  // *************************************************************************************

  for (auto itr : *eventMaps.gatedImplant){
    histoManager->h1_onspill_times->Fill(itr.first);
  }

  // *************************************************************************************
  // ***************************** WRITE HISTOGRAMS **************************************
  // *************************************************************************************

  histoManager->WriteHistograms();

  // *************************************************************************************
  // ************************************** CLEANUP **************************************
  // *************************************************************************************

  treeManager->ClearHeap();
  delete treeManager;

  histoManager->ClearHeap();
  delete histoManager;
      
  inputFile->Close();
  outputFile->Close();

  delete inputFile;
  delete outputFile;
}