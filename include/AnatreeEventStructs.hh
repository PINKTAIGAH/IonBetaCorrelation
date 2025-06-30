// Header containing structs for event data from anatrees

#ifndef ANATREE_EVENT_STRUCTS_H
#define ANATREE_EVENT_STRUCTS_H

#include "TObject.h"

struct ImplantEvent {
  ULong64_t time;
  ULong64_t timeX; 
  ULong64_t timeY; 
  Double_t x; 
  Double_t y; 
  Double_t e; 
  Double_t eX; 
  Double_t eY;
  Int_t csX;
  Int_t csY;
  Int_t cminX;
  Int_t cmaxX;
  Int_t cminY;
  Int_t cmaxY;
  Int_t dssd;
  Int_t spill;
};

struct DecayEvent {
  ULong64_t time;
  ULong64_t timeX; 
  ULong64_t timeY; 
  Double_t x; 
  Double_t y; 
  Double_t e; 
  Double_t eX; 
  Double_t eY;
  Int_t csX;
  Int_t csY;
  Int_t cminX;
  Int_t cmaxX;
  Int_t cminY;
  Int_t cmaxY;
  Int_t dssd;
  Int_t spill;
};

struct GermaniumEvent {
  ULong64_t time;
  Double_t e;
  Int_t spill;
};

#endif