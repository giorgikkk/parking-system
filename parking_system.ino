#include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>
#include "detector.h"
#include <Servo.h>

Servo myServo;

detector myDetector;
const byte numberOfPulses = 12;
const byte capacitor_charger_p = A1;
const byte capacitor_measurer_p = A0;
const byte led_p = 8;
int state = 0;
int counter = 0;
int coursor = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2);

RF24 radio(9, 10);
bool transmit = true;
const byte addresses[][6] = {
  "00001",
  "00002"
};

int currCarNum = 0;
int maxCarNum = 3;

char message[32] = "    ";
int index = 0;

const int button1 = 2;
const int button2 = 3;
const int button3 = 4;

void setup() {
  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  pinMode(capacitor_charger_p, OUTPUT);
  pinMode(capacitor_measurer_p, INPUT);
  pinMode(led_p, OUTPUT);

  digitalWrite(capacitor_charger_p, LOW);
  digitalWrite(led_p, LOW);

  myDetector.det(numberOfPulses, capacitor_charger_p, capacitor_measurer_p, led_p, state);

  myServo.attach(6);

  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);

  myServo.write(5);
}

bool metalIsHere = false;
void loop() {
  char quant = '0' + maxCarNum - counter;
  lcd.setCursor(2, 0);
  lcd.print("Available ");
  lcd.print(quant);
  lcd.setCursor(5, 1);
  lcd.print("spot");

  delay(1100);
  if (counter < maxCarNum) {
    bool isCar = false;
    delay(2);
    myDetector.isHere(isCar);
    if (isCar) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Insert Password:");
      isCar = false;
      generatePassword();
      delay(1000);
    }
    if (digitalRead(button3) == HIGH) {
      Serial.print("uwu");
      exiting();
    }
  }
  lcd.clear();
}

void enterCar() {
  delay(2);
  lcd.clear();
  delay(2);
  delay(20);
  myServo.write(85);
  delay(100);

  bool metalHere = true;
  while (metalHere) {
    delay(4000);
    metalHere = false;
    myDetector.isHere(metalHere);
    delay(10);
  }

  currCarNum++;
  delay(100);
  myServo.write(1);
  counter++;
  delay(100);
  lcd.clear();
}

void exiting() {
  if (counter != 0) {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Exiting");
    myServo.write(85);
    delay(1000);
    myServo.write(1);
    counter--;
  }
}

void generatePassword() {
  if (transmit) {
    while (index != 4) {
      delay(5);
      if (digitalRead(button1) == HIGH) {
        delay(50);
        while (digitalRead(button1) == HIGH) {};
        if (digitalRead(button1) == LOW) {
          delay(5);
          message[index] = '1';
          delay(2);
          index++;
          delay(5);
          Serial.println("1");
          coursor++;
          lcd.setCursor(coursor, 1);
          lcd.print("*");
        }
      }

      delay(100);

      if (digitalRead(button2) == HIGH) {
        delay(50);
        while (digitalRead(button2) == HIGH) {};
        if (digitalRead(button2) == LOW) {
          delay(5);
          message[index] = '0';
          delay(2);
          index++;
          delay(5);
          Serial.println("0");
          coursor++;
          lcd.setCursor(coursor, 1);
          lcd.print("*");
        }
      }
    }

    index = 0;
    Serial.print("msg: ");
    Serial.println(message);
    radio.stopListening();

    radio.write( & message, sizeof(message));
    transmit = false;
    delay(10);
  }

  if (!transmit) {
    radio.startListening();
    delay(5);
    char feedbackMessage[32] = "";

    unsigned long t = millis();

    while (millis() - t <= 10) {
      radio.read( & feedbackMessage, sizeof(feedbackMessage));
    }

    delay(100);
    radio.read( & feedbackMessage, sizeof(feedbackMessage));
    if ((String) feedbackMessage == "correct") {
      lcd.print("correct");
      feedbackMessage[32] = "";
      enterCar();
      coursor = 5;
    } else {
      lcd.clear();
      lcd.print("incorrect");
      coursor = 5;
    }
    transmit = true;
    delay(10);
  }
}