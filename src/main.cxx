#include<iostream>

#include "Logger.hh"
#include "IonBeta.hh"
#include "ConfigReader.hh"

int main (int argc, const char** argv){

  Logger::ScopedTimer programTimer("Program IonBeta");

  if ( argc != 3 ){
    Logger::Log("Program call must follow the following structure: ./ionbeta <Input File> <Output File> \n", Logger::Level::FATAL);
    return 1;
  }

  ConfigReader* config = new ConfigReader("../config/ionBeta.cfg");

  // Call IonBeta Function (Main action function)
  IonBeta(argv[1], argv[2]);

  delete config;

  Logger::Log("Program has run succesfully!");

  return 0;
}