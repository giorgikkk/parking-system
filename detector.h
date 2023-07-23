#ifndef METALDETECTOR_H
#define METALDETECTOR_H#include "Arduino.h"

class detector {
  public:
  void det(byte pulse, byte capacitor_charger_pin, byte capacitor_measurer_pin, byte led_pin, int state);
  void isHere(bool & res);
  private: byte pulses;
  byte capacitorChargerPin;
  byte capMeasurerPin;
  byte ledPin;
  byte stateInt;
  bool isCar;
  const int n = 256;
  int numberOfPulses;
  int diff;
  int average;
};

#endif