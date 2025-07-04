#include<iostream>

#include "Logger.hh"
#include "IonBeta.hh"
#include "ConfigReader.hh"
#include "ArgumentParser.hh"

int main (int argc, char* argv[]){
  
  // Initialte scoped timer for program runtime
  Logger::ScopedTimer programTimer("Program ionbeta");

  // Define program flags
  ArgumentParser::Instance().AddFlag("I", "Input file path", true);
  ArgumentParser::Instance().AddFlag("C", "Config file", true);
  ArgumentParser::Instance().AddFlag("I2", "Secondary input file path", false, "input.root");
  ArgumentParser::Instance().AddFlag("O", "Output file path", false, "output.root");
  ArgumentParser::Instance().AddFlag("v", "Verbose mode", false);
  ArgumentParser::Instance().AddFlag("help", "Show help message", false);

  // Parse arguments
  if (!ArgumentParser::Instance().Parse(argc, argv)){
    ArgumentParser::Instance().PrintHelp(); 
    Logger::Log("Required parameters not provided to program", Logger::Level::FATAL);
    return 1;
  }

  // Check if help flag called 
  if (ArgumentParser::Instance().HasFlag("help")){
    ArgumentParser::Instance().PrintHelp(); 
    return 0;
  }

  ConfigReader::Instance().Initialise(ArgumentParser::Instance().GetValue("C"));
  if (ArgumentParser::Instance().HasFlag("v")) ConfigReader::Instance().PrintConfigValues();

  // Call IonBeta Function (Main action function)
  IonBeta();

  Logger::Log("Program has run succesfully!");

  return 0;
}
