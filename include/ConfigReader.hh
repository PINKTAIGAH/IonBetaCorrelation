#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <vector>
#include <utility>
#include <string>
#include <memory>
#include "utils.hh"
#include "tinyxml2.h"

class ConfigReader{
  
  public:
    // Singleton Methods
    static ConfigReader& Instance();
    void Initialise(const std::string configFile, const std::string& isotopeName);
    void PrintConfigValues();
    
    // Getters for parameters in config
    std::string GetIsotopeName() const;
    Long64_t GetTimeScale() const;
    Long64_t GetTimeThreshold() const;
    Int_t GetDssd() const;
    PromptWindow GetDecayGammaWindow() const;
    PromptWindow GetGammaGammaWindow() const;
    Long64_t GetImplantDeadTime() const;
    Double_t GetLocalDeadTimePositionWindow() const;
    bool GetPlotBetaGamma() const;
    bool GetVetoInterruptedImplants() const;
    bool GetOnlyOffspillDecays() const;
    bool GetAllowAjacentClusters() const;
    Int_t GetBetaGammaCandidateCut() const;
    std::vector<Int_t> GetBrokenAidaStripsImplantX() const;
    std::vector<Int_t> GetBrokenAidaStripsImplantY() const;
    std::vector<Int_t> GetBrokenAidaStripsDecayX() const;
    std::vector<Int_t> GetBrokenAidaStripsDecayY() const;

  private:
    // Singleton implementation
    explicit ConfigReader();
    ConfigReader(const ConfigReader&) = delete;
    ConfigReader& operator=(const ConfigReader&) = delete;
    
    bool initialised = false;
    std::string configFile;
    std::string requestedIsotopeName;
    bool unsuccessfulConfigReading;
    
    // TinyXML-2 document
    std::unique_ptr<tinyxml2::XMLDocument> xmlDoc;
    
    // Sentinel Values
    const Double_t numSentinel = 999999;
    const bool boolSentinel = false;
    const std::string stringSentinel = "__MISSING__";
    
    // Internal parameters for config values
    std::string isotopeName;
    Long64_t timeScale;
    Long64_t timeThreshold;
    Int_t dssd;
    PromptWindow decayGammaWindow;
    PromptWindow gammaGammaWindow;
    ULong64_t implantDeadTime;
    Double_t localDeadTimePositionWindow;
    bool plotBetaGamma;
    bool vetoInterruptedImplants;
    bool onlyOffspillDecays;
    bool allowAjacentClusters;
    Int_t betaGammaCandidateCut;
    std::vector<Int_t> brokenAidaStripsImplantX;
    std::vector<Int_t> brokenAidaStripsImplantY;
    std::vector<Int_t> brokenAidaStripsDecayX;
    std::vector<Int_t> brokenAidaStripsDecayY;
    
    // Helper methods
    void LoadConfig();
    void ValidateAllParametersLoaded();
    std::vector<Int_t> ParseStringToVector(const std::string& spaceString);
    tinyxml2::XMLElement* FindIsotopeElement(bool printError=false);
    tinyxml2::XMLElement* FindWindowElement(const std::string& windowName);
    std::string GetElementText(tinyxml2::XMLElement* element, const std::string& defaultValue = "");
    bool GetElementBool(tinyxml2::XMLElement* element, bool defaultValue = false);
    Double_t GetElementDouble(tinyxml2::XMLElement* element, Double_t defaultValue = 0.0);
    Int_t GetElementInt(tinyxml2::XMLElement* element, Int_t defaultValue = 0);
    
    // Setters for each parameter
    void SetIsotopeName();
    void SetTimeScale();
    void SetTimeThreshold();
    void SetDssd();
    void SetDecayGammaWindow();
    void SetGammaGammaWindow();
    void SetImplantDeadTime();
    void SetLocalDeadTimePositionWindow();
    void SetPlotBetaGamma();
    void SetVetoInterruptedImplants();
    void SetOnlyOffspillDecays();
    void SetAllowAjacentClusters();
    void SetBetaGammaCandidateCut();
    void SetBrokenAidaStripsImplantX();
    void SetBrokenAidaStripsImplantY();
    void SetBrokenAidaStripsDecayX();
    void SetBrokenAidaStripsDecayY();
};

#endif