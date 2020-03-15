#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

const int wPump = 13; // choosing a pin for the Water Pump

void setup() {
  Serial.begin(115200);

  pinMode(wPump, OUTPUT); //The Water Pump is an Output
  
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
}

void loop() {
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);
  delay(100);

  if (capread < 750) {
    Serial.println("Watering");
    digitalWrite(wPump, LOW);
    } else {
    digitalWrite(wPump, HIGH);
    }
  
}
