#include <sstream>

#include "TObjArray.h"
#include "TObjString.h"

#include "Logger.hh"
#include "ConfigReader.hh"

// Constructor

ConfigReader::ConfigReader() {};

ConfigReader& ConfigReader::Instance(){
  static ConfigReader instance;
  return instance;
}

void ConfigReader::Initialise(const std::string configFile){
  if (initialised){
    Logger::Log("ConfigReader already initialised. Skipping", Logger::Level::WARNING);
    return;
  }
  this->configFile = configFile;
  LoadConfig();
  initialised = true;

  if (unsuccesfulConfigReading) {
    Logger::Log("Could not read config file succesfully", Logger::Level::FATAL);
    std::exit(1);
  }


}


// Private

void ConfigReader::LoadConfig(){

  unsuccesfulConfigReading = false;

  // Define TEnv
  env = std::make_unique<TEnv>();
  int status = env->ReadFile(configFile.c_str(), kEnvUser);

  // Check if logger was opened succesfully
  if (status!=0){
    Logger::Log("Problem reading config file: " + configFile, Logger::Level::FATAL);
    std::exit(1);
  }

  SetIsotopeName();
  SetTimeScale();
  SetTimeThreshold();
  SetDecayGammaWindow();
  SetGammaGammaWindow();
  SetImplantDeadTime();
  SetLocalDeadTimePositionWindow();
  SetVetoInterruptedImplants();
  SetOnlyOffspillDecays();
  SetAllowAjacentClusters();
  SetBetaGammaCandidateCut();
  SetBrokenAidaStripsImplantX();
  SetBrokenAidaStripsImplantY();
  SetBrokenAidaStripsDecayX();
  SetBrokenAidaStripsDecayY();

}

void ConfigReader::KeyExists(const TString& key){
  if (env->Lookup(key) != nullptr) return;
  Logger::Log("Config parameter not recognised: " + std::string(key.Data()), Logger::Level::ERROR);
  unsuccesfulConfigReading = true;
}

std::vector<Int_t> ConfigReader::ParseStringToVector(TString& csvString){
  std::vector<Int_t> result;
  TObjArray* tokens = csvString.Tokenize(",");
  for (int idx=0; idx < tokens->GetEntries(); ++idx){
    TString token = ((TObjString*)tokens->At(idx))->GetString();
    result.emplace_back(token.Atoi());
  }
  delete tokens;
  return result;
}

// Defining setters

void ConfigReader::SetIsotopeName(){
  TString key = "isotopeName";
  KeyExists(key);  
  isotopeName = env->GetValue(key, stringSentinel);
}

void ConfigReader::SetTimeScale(){
  TString key = "timeScale";
  KeyExists(key);  
  timeScale = env->GetValue(key, numSentinel);
}

void ConfigReader::SetTimeThreshold(){
  TString key = "timeThreshold";
  KeyExists(key);  
  timeThreshold = env->GetValue(key, numSentinel) * timeScale;
}

void ConfigReader::SetDecayGammaWindow(){
  TString startKey = "decayGammaWindow.start";
  KeyExists(startKey);  
  TString endKey = "decayGammaWindow.end";
  KeyExists(endKey);  

  Long64_t start = env->GetValue(startKey, numSentinel);
  Long64_t end = env->GetValue(endKey, numSentinel);

  decayGammaWindow = {start, end};
}

void ConfigReader::SetGammaGammaWindow(){
  TString startKey = "gammaGammaWindow.start";
  KeyExists(startKey);  
  TString endKey = "gammaGammaWindow.end";
  KeyExists(endKey);  

  Long64_t start = env->GetValue(startKey, numSentinel);
  Long64_t end = env->GetValue(endKey, numSentinel);

  gammaGammaWindow = {start, end};
}

void ConfigReader::SetImplantDeadTime(){
  TString key = "implantDeadTime";
  KeyExists(key);  
  implantDeadTime = env->GetValue(key, numSentinel);
}

void ConfigReader::SetLocalDeadTimePositionWindow(){
  TString key = "localDeadTimePositionWindow";
  KeyExists(key);  
  localDeadTimePositionWindow = env->GetValue(key, numSentinel);
}

void ConfigReader::SetVetoInterruptedImplants(){
  TString key = "vetoInterruptedImplants";
  KeyExists(key);  
  vetoInterruptedImplants = env->GetValue(key, boolSentinel);
}

void ConfigReader::SetOnlyOffspillDecays(){
  TString key = "onlyOffspillDecays";
  KeyExists(key);  
  onlyOffspillDecays = env->GetValue(key, boolSentinel);
}

void ConfigReader::SetAllowAjacentClusters(){
  TString key = "allowAjacentClusters";
  KeyExists(key);  
  allowAjacentClusters = env->GetValue(key, boolSentinel);
}

void ConfigReader::SetBetaGammaCandidateCut(){
  TString key = "betaGammaCandidateCut";
  KeyExists(key);  
  betaGammaCandidateCut = env->GetValue(key, numSentinel);
}

void ConfigReader::SetBrokenAidaStripsImplantX(){
  TString key = "brokenAidaStripsImplantX";
  KeyExists(key);  
  TString vectorString = env->GetValue(key, stringSentinel);
  brokenAidaStripsImplantX = ParseStringToVector(vectorString);
}

void ConfigReader::SetBrokenAidaStripsImplantY(){
  TString key = "brokenAidaStripsImplantY";
  KeyExists(key);  
  TString vectorString = env->GetValue(key, stringSentinel);
  brokenAidaStripsImplantY = ParseStringToVector(vectorString);
}

void ConfigReader::SetBrokenAidaStripsDecayX(){
  TString key = "brokenAidaStripsDecayX";
  KeyExists(key);  
  TString vectorString = env->GetValue(key, stringSentinel);
  brokenAidaStripsDecayX = ParseStringToVector(vectorString);
}

void ConfigReader::SetBrokenAidaStripsDecayY(){
  TString key = "brokenAidaStripsDecayY";
  KeyExists(key);  
  TString vectorString = env->GetValue(key, stringSentinel);
  brokenAidaStripsDecayY = ParseStringToVector(vectorString);
}

// Public

void ConfigReader::PrintConfigValues(){

  // Lambda for vector to string
  auto VectorToString = [](const std::vector<Int_t>& vector) -> std::string{
    std::ostringstream oss;
    for (size_t idx=0; idx<vector.size(); ++idx){
      oss << vector[idx];
      if ( idx < vector.size()-1 ) oss << ", ";
    }
    return oss.str();
  };

  Logger::Log("Config -> Isotope Name: " + std::string(isotopeName.Data()), Logger::Level::DEBUG);
  Logger::Log("Config -> Time Scale: " + std::to_string(timeScale), Logger::Level::DEBUG);
  Logger::Log("Config -> Time Threshold: " + std::to_string(timeThreshold), Logger::Level::DEBUG);
  Logger::Log("Config -> Decay Gamma Window: (" + std::to_string(decayGammaWindow.start) + "," + std::to_string(decayGammaWindow.end) + ")", Logger::Level::DEBUG);
  Logger::Log("Config -> Gamma Gamma Window: (" + std::to_string(gammaGammaWindow.start) + "," + std::to_string(gammaGammaWindow.end) + ")", Logger::Level::DEBUG);
  Logger::Log("Config -> Implant Deadtime: " + std::to_string(implantDeadTime), Logger::Level::DEBUG);
  Logger::Log("Config -> Local Deadtime Positiion Window: " + std::to_string(localDeadTimePositionWindow), Logger::Level::DEBUG);
  Logger::Log("Config -> Veto Interrupted Implants: " + std::to_string(vetoInterruptedImplants), Logger::Level::DEBUG);
  Logger::Log("Config -> Only Offspill Decays: " + std::to_string(onlyOffspillDecays), Logger::Level::DEBUG);
  Logger::Log("Config -> Allow Ajacent Clusters: " + std::to_string(allowAjacentClusters), Logger::Level::DEBUG);
  Logger::Log("Config -> Beta Gamma Candidate Cut: " + std::to_string(betaGammaCandidateCut), Logger::Level::DEBUG);
  Logger::Log("Config -> Broken Aida Strips Implant X: " + VectorToString(brokenAidaStripsImplantX), Logger::Level::DEBUG);
  Logger::Log("Config -> Broken Aida Strips Implant Y: " + VectorToString(brokenAidaStripsImplantY), Logger::Level::DEBUG);
  Logger::Log("Config -> Broken Aida Strips Decay X: " + VectorToString(brokenAidaStripsDecayX), Logger::Level::DEBUG);
  Logger::Log("Config -> Broken Aida Strips Decay Y: " + VectorToString(brokenAidaStripsDecayY), Logger::Level::DEBUG);
}

std::string ConfigReader::GetIsotopeName() const{ return std::string(isotopeName); }
Long64_t ConfigReader::GetTimeScale() const{ return timeScale; }
Long64_t ConfigReader::GetTimeThreshold() const{ return timeThreshold; }
PromptWindow ConfigReader::GetDecayGammaWindow() const{ return decayGammaWindow; }
PromptWindow ConfigReader::GetGammaGammaWindow() const{ return gammaGammaWindow; }
ULong64_t ConfigReader::GetImplantDeadTime() const{ return implantDeadTime; }
Double_t ConfigReader::GetLocalDeadTimePositionWindow() const{ return localDeadTimePositionWindow; }
bool ConfigReader::GetVetoInterruptedImplants() const{ return vetoInterruptedImplants; }
bool ConfigReader::GetOnlyOffspillDecays() const{ return onlyOffspillDecays; }
bool ConfigReader::GetAllowAjacentClusters() const{ return allowAjacentClusters; }
Int_t ConfigReader::GetBetaGammaCandidateCut() const{ return betaGammaCandidateCut; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsImplantX() const{ return brokenAidaStripsImplantX; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsImplantY() const{ return brokenAidaStripsImplantY; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsDecayX() const{ return brokenAidaStripsDecayX; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsDecayY() const{ return brokenAidaStripsDecayY; }