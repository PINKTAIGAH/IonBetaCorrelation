#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <vector>
#include <utility>
#include <string>

#include "TEnv.h"
#include "TString.h"
#include "TObject.h"

#include "utils.hh"

class ConfigReader{
  
  public:
    // Singleton Methods
    static ConfigReader& Instance();

    void Initialise(const std::string configFile);
    void PrintConfigValues();

    // Getters for parameters in config
    std::string GetIsotopeName() const;
    Long64_t GetTimeScale() const;
    Long64_t GetTimeThreshold() const;
    PromptWindow GetDecayGammaWindow() const;
    PromptWindow GetGammaGammaWindow() const;
    ULong64_t GetImplantDeadTime() const;
    Double_t GetLocalDeadTimePositionWindow() const;
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

    // TEnv Object
    std::unique_ptr<TEnv> env;
    std::string configFile;
    bool unsuccesfulConfigReading;

    // Sentinel Values
    const Double_t numSentinel = 999999;
    const bool boolSentinel = false;
    const TString stringSentinel = "__MISSING__";

    // Methods
    void LoadConfig();
    void KeyExists(const TString& key);
    std::vector<Int_t> ParseStringToVector(TString& csvString);

    // internal parameters for config values
    TString isotopeName;
    Long64_t timeScale;
    Long64_t timeThreshold;
    PromptWindow decayGammaWindow;
    PromptWindow gammaGammaWindow;
    ULong64_t implantDeadTime;
    Double_t localDeadTimePositionWindow;
    bool vetoInterruptedImplants;
    bool onlyOffspillDecays;
    bool allowAjacentClusters;
    Int_t betaGammaCandidateCut;
    std::vector<Int_t> brokenAidaStripsImplantX;
    std::vector<Int_t> brokenAidaStripsImplantY;
    std::vector<Int_t> brokenAidaStripsDecayX;
    std::vector<Int_t> brokenAidaStripsDecayY;
    // ...

    // Setters for each parameter passing TEnv as parameter
    void SetIsotopeName();
    void SetTimeScale();
    void SetTimeThreshold();
    void SetDecayGammaWindow();
    void SetGammaGammaWindow();
    void SetImplantDeadTime();
    void SetLocalDeadTimePositionWindow();
    void SetVetoInterruptedImplants();
    void SetOnlyOffspillDecays();
    void SetAllowAjacentClusters();
    void SetBetaGammaCandidateCut();
    void SetBrokenAidaStripsImplantX();
    void SetBrokenAidaStripsImplantY();
    void SetBrokenAidaStripsDecayX();
    void SetBrokenAidaStripsDecayY();
    // ...

};


#endif