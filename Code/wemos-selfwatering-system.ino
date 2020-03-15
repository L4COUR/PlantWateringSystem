//libraries
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

// wifi name
const char ssid[] = "INSERT WIFI NAME";
// wifi code
const char pass[] = "INSERT WIFI CODE";

// Insert 'key/username' from shiftr
const char key[] = "PlantFeeling";
// Insert secret/password frrom shiftr
const char secret[] = "372575487ca8b282";

WiFiClient net;
MQTTClient client;

void connect();

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);
  connect();

//Soil-moisture sensor
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
  
}
// Checking Wifi-status
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("Plant", key, secret)){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");
  
  client.subscribe("/PlantThirst");
  client.subscribe("/Temperature");
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  // soil-moisure sensor
  float tempC = ss.getTemp();
  String stringtempC = String(tempC);
  uint16_t capread = ss.touchRead(0);
  String stringCapread = String(capread);
  
  publishMessage("PlantThirst", stringCapread);
  publishMessage("Temperature", stringtempC);
  delay(500);
}
void publishMessage(String topic, String message){
  client.publish(topic, message);
}
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic=="/PlantThirst") {
    int v = payload.toInt();
    if (v<350) {
      Serial.println("Plant Need water");
      }
    }
}
