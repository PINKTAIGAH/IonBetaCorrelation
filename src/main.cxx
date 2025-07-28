#include<iostream>

#include "Logger.hh"
#include "IonBeta.hh"
#include "ConfigReader.hh"
#include "ArgumentParser.hh"

int main (int argc, char* argv[]){
  
  // Initialte scoped timer for program runtime
  Logger::ScopedTimer programTimer("Program ionbeta");

  // Define program flags
  ArgumentParser::Instance().AddFlag("isotope", "Isotope name", true, "", "i");
  ArgumentParser::Instance().AddFlag("config", "Config file", true, "", "c");
  ArgumentParser::Instance().AddFlag("output", "Output file path", false, "output.root", "o");
  ArgumentParser::Instance().AddBooleanFlag("verbose", "Verbose mode", "v");
  ArgumentParser::Instance().AddBooleanFlag("help", "Show help message");
  ArgumentParser::Instance().AddBooleanFlag("noansi", "Dissable ANSI escape tags in console logs (for piping into a log file)");

  // Parse arguments
  if (!ArgumentParser::Instance().Parse(argc, argv)){
    return 1;
  }

  ConfigReader::Instance().Initialise( ArgumentParser::Instance().GetValue("config"), ArgumentParser::Instance().GetValue("isotope") );
  if (ArgumentParser::Instance().GetBoolValue("verbose")) ConfigReader::Instance().PrintConfigValues();

  if ( ArgumentParser::Instance().GetPositionalArg(0) == "" ){
    Logger::Log("No input files provided", Logger::Level::FATAL);
    ArgumentParser::Instance().PrintHelp(); 
    std::exit(1);
  }  

  for ( auto& fileName : ArgumentParser::Instance().GetPositionalArgs() ){ Logger::Log("Loaded input file: " + fileName); }

  // Call IonBeta Function (Main action function)
  IonBeta();

  Logger::Log("Program has run succesfully!");

  return 0;
}
