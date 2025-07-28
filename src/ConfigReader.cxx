#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include "Logger.hh"
#include "ConfigReader.hh"

// Constructor 

ConfigReader::ConfigReader()
    : xmlDoc(std::make_unique<tinyxml2::XMLDocument>()) {};



ConfigReader& ConfigReader::Instance(){
  static ConfigReader instance;
  return instance;
}

void ConfigReader::Initialise(const std::string configFile, const std::string& isotopeName){
  if (initialised){
    Logger::Log("ConfigReader already initialised. Skipping", Logger::Level::WARNING);
    return;
  }

  this->configFile = configFile;
  this->requestedIsotopeName = isotopeName;
  LoadConfig();
  initialised = true;

  if (unsuccessfulConfigReading){
    Logger::Log("Could not read config file successfully", Logger::Level::FATAL);
    std::exit(1);
  }
}

// Private Methods

void ConfigReader::LoadConfig(){
  unsuccessfulConfigReading = false;

  // Load XML file
  tinyxml2::XMLError result = xmlDoc->LoadFile(configFile.c_str());
  if (result != tinyxml2::XML_SUCCESS){
    Logger::Log("Could not load XML config file: " + configFile + " (Error: " + std::to_string(result) + ")", Logger::Level::FATAL);
    unsuccessfulConfigReading = true;
    return;
  }

  // Check valid isotope name 
  if (FindIsotopeElement(true) == nullptr) return;

  // Set all parameters
  SetIsotopeName();
  SetTimeScale();
  SetTimeThreshold();
  SetDssd();
  SetDecayGammaWindow();
  SetGammaGammaWindow();
  SetImplantDeadTime();
  SetLocalDeadTimePositionWindow();
  SetPlotBetaGamma();
  SetVetoInterruptedImplants();
  SetOnlyOffspillDecays();
  SetAllowAjacentClusters();
  SetBetaGammaCandidateCut();
  SetCorrelationPositionWindow();
  SetBrokenAidaStripsImplantX();
  SetBrokenAidaStripsImplantY();
  SetBrokenAidaStripsDecayX();
  SetBrokenAidaStripsDecayY();

  ValidateAllParametersLoaded();
}

void ConfigReader::ValidateAllParametersLoaded(){
  if (isotopeName == stringSentinel) unsuccessfulConfigReading = true;
  if (timeScale == numSentinel) unsuccessfulConfigReading = true;
  if (timeThreshold == numSentinel) unsuccessfulConfigReading = true;
  if (dssd == numSentinel) unsuccessfulConfigReading = true;
  if (decayGammaWindow.start == numSentinel || decayGammaWindow.end == numSentinel) unsuccessfulConfigReading = true;
  if (gammaGammaWindow.start == numSentinel || gammaGammaWindow.end == numSentinel) unsuccessfulConfigReading = true;
  if (implantDeadTime == numSentinel) unsuccessfulConfigReading = true;
  if (localDeadTimePositionWindow == numSentinel) unsuccessfulConfigReading = true;
  if (betaGammaCandidateCut == numSentinel) unsuccessfulConfigReading = true;
  if (correlationPositionWindow == numSentinel) unsuccessfulConfigReading = true;
}

std::vector<Int_t> ConfigReader::ParseStringToVector(const std::string& spaceString){
  std::vector<Int_t> result;
  if (spaceString.empty() || spaceString == stringSentinel) return result;

  std::stringstream ss(spaceString);
  std::string token;
  
  while (ss >> token) result.push_back( std::stoi(token) ); // Loop through all non-whitespace tokens
  
  return result;
}

tinyxml2::XMLElement* ConfigReader::FindIsotopeElement(bool logError){
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root){
    Logger::Log("No root <config> element found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    return nullptr;
  }

  std::ostringstream oss; // To store names of all isotopes in file in case none are recognised

  for (tinyxml2::XMLElement* isotope = root->FirstChildElement("isotope"); isotope; isotope = isotope->NextSiblingElement("isotope")){
    const char* name = isotope->Attribute("name");
    oss << name << ", ";
    if (name && requestedIsotopeName == name) return isotope;
  }

  // Remove training comma
  std::string allIsotopeNames = oss.str();
  if (!oss.str().empty()) allIsotopeNames.pop_back();

  if (logError){
    Logger::Log("Isotope not found: " + requestedIsotopeName, Logger::Level::ERROR);
    Logger::Log("Isotopes defined in " + configFile + ": " + allIsotopeNames, Logger::Level::ERROR);
  }

  unsuccessfulConfigReading = true;
  return nullptr;
}

tinyxml2::XMLElement* ConfigReader::FindWindowElement(const std::string& windowName){
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root){
    Logger::Log("No root <config> element found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    return nullptr;
  }

  for (tinyxml2::XMLElement* window = root->FirstChildElement("window"); window; window = window->NextSiblingElement("window")){
    const char* name = window->Attribute("name");
    if (name && windowName == name) return window;
  }

  Logger::Log("Window not found: " + windowName, Logger::Level::ERROR);
  unsuccessfulConfigReading = true;
  return nullptr;
}

std::string ConfigReader::GetElementText(tinyxml2::XMLElement* element, const std::string& defaultValue){
  if (!element) return defaultValue;

  const char* text = element->GetText();
  if (!text) return defaultValue;

  // Trim whitespace
  std::string result = text;
  result.erase(0, result.find_first_not_of(" \t\n\r"));
  result.erase(result.find_last_not_of(" \t\n\r") + 1);

  return result;
}

bool ConfigReader::GetElementBool(tinyxml2::XMLElement* element, bool defaultValue){
  std::string text = GetElementText(element);
  if (text.empty()) return defaultValue;
  
  std::transform(text.begin(), text.end(), text.begin(), ::tolower);
  return (text == "true");
}

double ConfigReader::GetElementDouble(tinyxml2::XMLElement* element, Double_t defaultValue) {
  std::string text = GetElementText(element);
  if (text.empty()) return defaultValue;
  
  try {
    return std::stod(text);
  } catch (const std::exception& e) {
    Logger::Log("Error parsing double value: " + text, Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    return defaultValue;
  }
}

int ConfigReader::GetElementInt(tinyxml2::XMLElement* element, Int_t defaultValue) {
  std::string text = GetElementText(element);
  if (text.empty()) return defaultValue;
  
  try {
    return std::stoi(text);
  } catch (const std::exception& e) {
    Logger::Log("Error parsing int value: " + text, Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    return defaultValue;
  }
}

// Setters for each parameter

void ConfigReader::SetIsotopeName() {
  isotopeName = requestedIsotopeName;
}

void ConfigReader::SetTimeScale() {
  tinyxml2::XMLElement* isotopeElement = FindIsotopeElement();
  if (!isotopeElement) {
    timeScale = numSentinel;
    return;
  }
  
  tinyxml2::XMLElement* timeScaleElement = isotopeElement->FirstChildElement("timeScale");
  if (!timeScaleElement) {
    Logger::Log("timeScale element not found for isotope: " + requestedIsotopeName, Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    timeScale = numSentinel;
    return;
  }
  
  timeScale = static_cast<Long64_t>(GetElementDouble(timeScaleElement, numSentinel));
}

void ConfigReader::SetTimeThreshold() {
  tinyxml2::XMLElement* isotopeElement = FindIsotopeElement();
  if (!isotopeElement) {
    timeThreshold = numSentinel;
    return;
  }
  
  tinyxml2::XMLElement* timeThresholdElement = isotopeElement->FirstChildElement("timeThreshold");
  if (!timeThresholdElement) {
    Logger::Log("timeThreshold element not found for isotope: " + requestedIsotopeName, Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    timeThreshold = numSentinel;
    return;
  }
  
  Long64_t threshold = static_cast<Long64_t>(GetElementDouble(timeThresholdElement, numSentinel));
  timeThreshold = threshold * timeScale;
}

void ConfigReader::SetDssd() {
  
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root){
    dssd = numSentinel;
    return;
  }

  tinyxml2::XMLElement* element = root->FirstChildElement("dssd");
  if(!element){
    Logger::Log("dssd element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    dssd = numSentinel;
    return;
  }

  dssd = GetElementInt(element, numSentinel);

}

void ConfigReader::SetDecayGammaWindow() {
  tinyxml2::XMLElement* windowElement = FindWindowElement("decayGamma");
  if (!windowElement) {
    decayGammaWindow = {static_cast<Long64_t>(numSentinel), static_cast<Long64_t>(numSentinel)};
    return;
  }
  
  tinyxml2::XMLElement* startElement = windowElement->FirstChildElement("start");
  tinyxml2::XMLElement* endElement = windowElement->FirstChildElement("end");
  
  if (!startElement || !endElement) {
    Logger::Log("start or end element not found for decayGamma window", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    decayGammaWindow = {static_cast<Long64_t>(numSentinel), static_cast<Long64_t>(numSentinel)};
    return;
  }
  
  Long64_t start = static_cast<Long64_t>(GetElementDouble(startElement, numSentinel));
  Long64_t end = static_cast<Long64_t>(GetElementDouble(endElement, numSentinel));
  
  decayGammaWindow = {start, end};
}

void ConfigReader::SetGammaGammaWindow() {
  tinyxml2::XMLElement* windowElement = FindWindowElement("gammaGamma");
  if (!windowElement) {
    gammaGammaWindow = {static_cast<Long64_t>(numSentinel), static_cast<Long64_t>(numSentinel)};
    return;
  }
  
  tinyxml2::XMLElement* startElement = windowElement->FirstChildElement("start");
  tinyxml2::XMLElement* endElement = windowElement->FirstChildElement("end");
  
  if (!startElement || !endElement) {
    Logger::Log("start or end element not found for gammaGamma window", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    gammaGammaWindow = {static_cast<Long64_t>(numSentinel), static_cast<Long64_t>(numSentinel)};
    return;
  }
  
  Long64_t start = static_cast<Long64_t>(GetElementDouble(startElement, numSentinel));
  Long64_t end = static_cast<Long64_t>(GetElementDouble(endElement, numSentinel));
  
  gammaGammaWindow = {start, end};
}

void ConfigReader::SetImplantDeadTime() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) {
    implantDeadTime = numSentinel;
    return;
  }
  
  tinyxml2::XMLElement* element = root->FirstChildElement("implantDeadtime");
  if (!element) {
    Logger::Log("implantDeadtime element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    implantDeadTime = numSentinel;
    return;
  }
  
  implantDeadTime = static_cast<ULong64_t>(GetElementDouble(element, numSentinel));
}

void ConfigReader::SetLocalDeadTimePositionWindow() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) {
    localDeadTimePositionWindow = numSentinel;
    return;
  }
  
  tinyxml2::XMLElement* element = root->FirstChildElement("totalDeadtimePositionWindow");
  if (!element) {
    Logger::Log("totalDeadtimePositionWindow element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    localDeadTimePositionWindow = numSentinel;
    return;
  }
  
  localDeadTimePositionWindow = GetElementDouble(element, numSentinel);
}

void ConfigReader::SetPlotBetaGamma(){
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root){
    plotBetaGamma = boolSentinel;
    return;
  }

  tinyxml2::XMLElement* element = root->FirstChildElement("plotBetaGamma");
  if (!element){
    Logger::Log("plotBetaGamma element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    plotBetaGamma = boolSentinel;
    return;
  }

  plotBetaGamma = GetElementBool(element, boolSentinel);
}

void ConfigReader::SetVetoInterruptedImplants() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) {
    vetoInterruptedImplants = boolSentinel;
    return;
  }
  
  tinyxml2::XMLElement* element = root->FirstChildElement("vetoInterruptedImplant");
  if (!element) {
    Logger::Log("vetoInterruptedImplant element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    vetoInterruptedImplants = boolSentinel;
    return;
  }
  
  vetoInterruptedImplants = GetElementBool(element, boolSentinel);
}

void ConfigReader::SetOnlyOffspillDecays() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) {
    onlyOffspillDecays = boolSentinel;
    return;
  }
  
  tinyxml2::XMLElement* element = root->FirstChildElement("onlyOffspillDecay");
  if (!element) {
    Logger::Log("onlyOffspillDecay element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    onlyOffspillDecays = boolSentinel;
    return;
  }
  
  onlyOffspillDecays = GetElementBool(element, boolSentinel);
}

void ConfigReader::SetAllowAjacentClusters() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) {
    allowAjacentClusters = boolSentinel;
    return;
  }
  
  tinyxml2::XMLElement* element = root->FirstChildElement("allowAjacentClusters");
  if (!element) {
    Logger::Log("allowAjacentClusters element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    allowAjacentClusters = boolSentinel;
    return;
  }
  
  allowAjacentClusters = GetElementBool(element, boolSentinel);
}

void ConfigReader::SetBetaGammaCandidateCut() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) {
    betaGammaCandidateCut = numSentinel;
    return;
  }
  
  tinyxml2::XMLElement* element = root->FirstChildElement("betaGammaCandidateCut");
  if (!element) {
    Logger::Log("betaGammaCandidateCut element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    betaGammaCandidateCut = numSentinel;
    return;
  }
  
  betaGammaCandidateCut = GetElementInt(element, numSentinel);
}

void ConfigReader::SetCorrelationPositionWindow(){
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root){
    correlationPositionWindow = numSentinel;
    return;
  }

  tinyxml2::XMLElement* element = root->FirstChildElement("correlationPositionWindow");
  if (!element){
    Logger::Log("correlationPositionWindow element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    correlationPositionWindow = numSentinel;
    return;
  }

  correlationPositionWindow = GetElementBool(element, numSentinel);
}

void ConfigReader::SetBrokenAidaStripsImplantX() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) return;
  
  tinyxml2::XMLElement* element = root->FirstChildElement("brokenAidaStripsImplantX");
  if (!element) {
    Logger::Log("brokenAidaStripsImplantX element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    return;
  }
  
  std::string valueStr = GetElementText(element);
  brokenAidaStripsImplantX = ParseStringToVector(valueStr);
}

void ConfigReader::SetBrokenAidaStripsImplantY() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) return;
  
  tinyxml2::XMLElement* element = root->FirstChildElement("brokenAidaStripsImplantY");
  if (!element) {
    Logger::Log("brokenAidaStripsImplantY element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    return;
  }
  
  std::string valueStr = GetElementText(element);
  brokenAidaStripsImplantY = ParseStringToVector(valueStr);
}

void ConfigReader::SetBrokenAidaStripsDecayX() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) return;
  
  tinyxml2::XMLElement* element = root->FirstChildElement("brokenAidaStripsDecayX");
  if (!element) {
    Logger::Log("brokenAidaStripsDecayX element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    return;
  }
  
  std::string valueStr = GetElementText(element);
  brokenAidaStripsDecayX = ParseStringToVector(valueStr);
}

void ConfigReader::SetBrokenAidaStripsDecayY() {
  tinyxml2::XMLElement* root = xmlDoc->FirstChildElement("config");
  if (!root) return;
  
  tinyxml2::XMLElement* element = root->FirstChildElement("brokenAidaStripsDecayY");
  if (!element) {
    Logger::Log("brokenAidaStripsDecayY element not found", Logger::Level::ERROR);
    unsuccessfulConfigReading = true;
    return;
  }
  
  std::string valueStr = GetElementText(element);
  brokenAidaStripsDecayY = ParseStringToVector(valueStr);
}

// Public Methods

void ConfigReader::PrintConfigValues() {
  // Lambda for vector to string
  auto VectorToString = [](const std::vector<Int_t>& vector) -> std::string {
    std::ostringstream oss;
    for (size_t idx = 0; idx < vector.size(); ++idx) {
      oss << vector[idx];
      if (idx < vector.size() - 1) oss << ", ";
    }
    return oss.str();
  };

  Logger::Log("Config -> Isotope Name: " + isotopeName, Logger::Level::DEBUG);
  Logger::Log("Config -> Time Scale: " + std::to_string(timeScale), Logger::Level::DEBUG);
  Logger::Log("Config -> Time Threshold: " + std::to_string(timeThreshold), Logger::Level::DEBUG);
  Logger::Log("Config -> Dssd: " + std::to_string(dssd), Logger::Level::DEBUG);
  Logger::Log("Config -> Decay Gamma Window: (" + std::to_string(decayGammaWindow.start) + "," + std::to_string(decayGammaWindow.end) + ")", Logger::Level::DEBUG);
  Logger::Log("Config -> Gamma Gamma Window: (" + std::to_string(gammaGammaWindow.start) + "," + std::to_string(gammaGammaWindow.end) + ")", Logger::Level::DEBUG);
  Logger::Log("Config -> Implant Deadtime: " + std::to_string(implantDeadTime), Logger::Level::DEBUG);
  Logger::Log("Config -> Local Deadtime Position Window: " + std::to_string(localDeadTimePositionWindow), Logger::Level::DEBUG);
  Logger::Log("Config -> Plot Beta Gamma: " + std::to_string(plotBetaGamma), Logger::Level::DEBUG);
  Logger::Log("Config -> Veto Interrupted Implants: " + std::to_string(vetoInterruptedImplants), Logger::Level::DEBUG);
  Logger::Log("Config -> Only Offspill Decays: " + std::to_string(onlyOffspillDecays), Logger::Level::DEBUG);
  Logger::Log("Config -> Allow Adjacent Clusters: " + std::to_string(allowAjacentClusters), Logger::Level::DEBUG);
  Logger::Log("Config -> Beta Gamma Candidate Cut: " + std::to_string(betaGammaCandidateCut), Logger::Level::DEBUG);
  Logger::Log("Config -> Correlation Position Window: " + std::to_string(correlationPositionWindow), Logger::Level::DEBUG);
  Logger::Log("Config -> Broken Aida Strips Implant X: " + VectorToString(brokenAidaStripsImplantX), Logger::Level::DEBUG);
  Logger::Log("Config -> Broken Aida Strips Implant Y: " + VectorToString(brokenAidaStripsImplantY), Logger::Level::DEBUG);
  Logger::Log("Config -> Broken Aida Strips Decay X: " + VectorToString(brokenAidaStripsDecayX), Logger::Level::DEBUG);
  Logger::Log("Config -> Broken Aida Strips Decay Y: " + VectorToString(brokenAidaStripsDecayY), Logger::Level::DEBUG);
}

// Parameter Getters

std::string ConfigReader::GetIsotopeName() const { return isotopeName; }
Long64_t ConfigReader::GetTimeScale() const { return timeScale; }
Long64_t ConfigReader::GetTimeThreshold() const { return timeThreshold; }
Int_t ConfigReader::GetDssd() const { return dssd; }
PromptWindow ConfigReader::GetDecayGammaWindow() const { return decayGammaWindow; }
PromptWindow ConfigReader::GetGammaGammaWindow() const { return gammaGammaWindow; }
Long64_t ConfigReader::GetImplantDeadTime() const { return implantDeadTime; }
Double_t ConfigReader::GetLocalDeadTimePositionWindow() const { return localDeadTimePositionWindow; }
bool ConfigReader::GetPlotBetaGamma() const { return plotBetaGamma; }
bool ConfigReader::GetVetoInterruptedImplants() const { return vetoInterruptedImplants; }
bool ConfigReader::GetOnlyOffspillDecays() const { return onlyOffspillDecays; }
bool ConfigReader::GetAllowAjacentClusters() const { return allowAjacentClusters; }
Int_t ConfigReader::GetBetaGammaCandidateCut() const { return betaGammaCandidateCut; }
Int_t ConfigReader::GetCorrelationPositionWindow() const { return correlationPositionWindow; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsImplantX() const { return brokenAidaStripsImplantX; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsImplantY() const { return brokenAidaStripsImplantY; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsDecayX() const { return brokenAidaStripsDecayX; }
std::vector<Int_t> ConfigReader::GetBrokenAidaStripsDecayY() const { return brokenAidaStripsDecayY; }