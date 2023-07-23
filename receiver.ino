#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte addresses[][6] = {
  "00001",
  "00002"
};
bool transmitter = false; 
int carAmount = 0;
const int limit = 200;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); 
  radio.openReadingPipe(1, addresses[1]); 
  radio.setPALevel(RF24_PA_MIN); 
}

void loop() {
  if (!transmitter) {
    radio.startListening();
    char ans[32] = "";
    radio.read( & ans, sizeof(ans));
    Serial.print(ans);
    if ((String) ans == "1001" && carAmount <= limit) {
      carAmount++;
      transmitter = true;
    }
  }
  
  if (transmitter) {
    radio.stopListening();
    unsigned long tm = millis();
    while (millis() - tm <= 100) {
      char migeba[32] = "correct";
      radio.write( & migeba, sizeof(migeba));
    }
    transmitter = false;
  }
}