#include "detector.h"

void detector::det(byte pulse, byte capacitor_charger_pin, byte capacitor_measurer_pin, byte led_pin, int state) {
  pulses = pulse;
  capacitorChargerPin = capacitor_charger_pin;
  capMeasurerPin = capacitor_measurer_pin;
  ledPin = led_pin;
  stateInt = state;
  numberOfPulses = 12;
  average = 0;
  pinMode(capacitorChargerPin, OUTPUT);
  digitalWrite(capacitorChargerPin, LOW);
  pinMode(capMeasurerPin, INPUT);
  Serial.begin(9600);
  long unsigned int sum = 0;
}

void detector::isHere(bool & res) {
  long unsigned int sum = 0;
  for (int i = 0; i < n + 2; i++) {
    pinMode(capMeasurerPin, OUTPUT);
    digitalWrite(capMeasurerPin, LOW);
    delayMicroseconds(20);
    pinMode(capMeasurerPin, INPUT);
    for (int ipulse = 0; ipulse < numberOfPulses; ipulse++) {
      digitalWrite(capacitorChargerPin, HIGH);
      delayMicroseconds(3);
      digitalWrite(capacitorChargerPin, LOW);
      delayMicroseconds(3);
    }
    int val = analogRead(capMeasurerPin);
    sum += val;
  }

  if (diff >= 100) stateInt = 1;
  if (diff < -100) stateInt = 2;

  if (stateInt == 2) {
    delay(10);
    diff = 0;
    res = true;
  }

  if (average == 0) average = sum << 6;
  
  long int avgsum = (average + 32) >> 6;
  diff = sum - avgsum - 18050 - 16000 - 700;
  stateInt = 0;
  Serial.print("sum: ");
  Serial.print(sum);
  Serial.print(" avg: ");
  Serial.print(avgsum);
  Serial.print(" diff: ");
  Serial.println(diff);
}