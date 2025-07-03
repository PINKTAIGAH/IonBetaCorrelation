#include "TObjArray.h"
#include "TObjString.h"

#include "Logger.hh"
#include "ConfigReader.hh"


// Constructor
ConfigReader::ConfigReader(const std::string configFile){
  this->configFile = configFile;
  LoadConfig();

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

std::vector<Int_t> ParseStringToVector(TString& csvString){
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
  TString key = "betaGammmaCandidateCut";
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

std::string ConfigReader::GetIsotopeName(){ return std::string(isotopeName); }
Long64_t ConfigReader::GetTimeScale(){ return timeScale; }
Long64_t ConfigReader::GetTimeThreshold(){ return timeThreshold; }
PromptWindow ConfigReader::GetDecayGammaWindow(){ return decayGammaWindow; }
PromptWindow ConfigReader::GetGammaGammaWindow(){ return gammaGammaWindow; }
ULong64_t ConfigReader::GetImplantDeadTime(){ return implantDeadTime; }
Double_t ConfigReader::GetLocalDeadTimePositionWindow(){ return localDeadTimePositionWindow; }
bool ConfigReader::GetVetoInterruptedImplants(){ return vetoInterruptedImplants; }
bool ConfigReader::GetOnlyOffspillDecays(){ return onlyOffspillDecays; }
bool ConfigReader::GetAllowAjacentClusters(){ return allowAjacentClusters; }
Int_t ConfigReader::GetBetaGammaCandidateCut(){ return betaGammaCandidateCut; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsImplantX(){ return brokenAidaStripsImplantX; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsImplantY(){ return brokenAidaStripsImplantY; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsDecayX(){ return brokenAidaStripsDecayX; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsDecayY(){ return brokenAidaStripsDecayY; }