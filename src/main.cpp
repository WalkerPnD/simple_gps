#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <./gps/location-service.h>
#include <./internet/http-client.h>
#define BAND_RATE 9600
#define INTERVAL_SHORT 300

// GPS
LocationService location;
HttpClient netClient;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(BAND_RATE);

  Serial.println("setting up");
  location.init();
  netClient.init();
  Serial.println("set up: success!");
}



void loop() {

  while (true) {
    JsonObject gpsInfo = jsonGpsBuff.to<JsonObject>(); // clear and replace with a new JsonObject

    if (GpsSerial.available() > 0) {
      Serial.println("\nGpsSerial: OK");
      gpsInfo = location.read();
      serializeJsonPretty(gpsInfo, Serial);
      netClient.post(gpsInfo);
      
    } else {
      Serial.println("GpsSerial: NOT Ready");
    }
    
    
    delay(1500);
  }
}