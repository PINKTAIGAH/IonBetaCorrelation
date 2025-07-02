#ifndef GLOBAL_CONSTATNTS_H
#define GLOBAL_CONSTATNTS_H

#include "TObject.h"

namespace Global{

  const std::string isotopeName = "82nb";

  const Long64_t timeScale = 1e6;
  const Long64_t timeThreshold = 150 * timeScale;

  const ULong64_t implantDeadTime = 350e3;
  const Double_t localDeadTimePositionWindow = 5;

  const bool vetoInterruptedImplants = true;
  const bool onlyOffspillDecays = false;

  const std::vector<Int_t> BROKEN_AIDA_X_STRIPS_IMPLANT = {};
  const std::vector<Int_t> BROKEN_AIDA_Y_STRIPS_IMPLANT = {};
  const std::vector<Int_t> BROKEN_AIDA_X_STRIPS_DECAY = {63, 64, 66, 130, 189, 191, 194, 225, 256, 319, 320, 192, 128, 255};
  const std::vector<Int_t> BROKEN_AIDA_Y_STRIPS_DECAY = {127};

}

#endif