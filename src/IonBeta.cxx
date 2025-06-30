#include <iostream>

#include "TFile.h"

#include "TreeManager.hh"


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
  std::cout << "File loaded: "<< inputFile->GetName() << '\n';

  // Open output file
  TFile* outputFile = new TFile(outputFileName, "RECREATE");
  if (!outputFile){
    delete outputFile;
    std::cerr << "Error: Could not create output file " << std::endl;
    std::exit(1);
  }

  // *************************************************************************************
  // ******************************** EXTRACT DATA FROM ANATREES *************************
  // *************************************************************************************

  TreeManager* treeManager = new TreeManager(inputFile); 
  treeManager->LoadEvents();
  EventMaps eventMaps = treeManager->GetEventMaps();

  // *************************************************************************************
  // ************************************** CLEANUP **************************************
  // *************************************************************************************

  delete inputFile;
  delete outputFile;

  treeManager->Cleanup();
  delete treeManager;
      
}