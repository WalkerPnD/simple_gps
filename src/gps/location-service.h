#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ArduinoJson.h>
// Arduinoから見るとRXが受信でDXが送信
#define GPS_RX D2
#define GPS_TX D1
TinyGPSPlus gps;
SoftwareSerial GpsSerial(GPS_RX, GPS_TX, false, 250);
StaticJsonDocument<256> jsonGpsBuff;

class LocationService {

    public: void init() {
      GpsSerial.begin(9600);
      GpsSerial.listen();
    }

    public: JsonObject read() {
      // byte rawGeoLocation = GpsSerial.read("n");
      bool hasInfo = false;
      JsonObject gpsInfo = jsonGpsBuff.to<JsonObject>();

      while(!hasInfo) {
        gps.encode(GpsSerial.read());
        Serial.println(gps.charsProcessed());
        if (gps.time.isValid()) {
          JsonObject timeInfo = gpsInfo.createNestedObject("time");

          timeInfo["hour"] = String(gps.time.hour());
          timeInfo["minute"] = String(gps.time.minute());
          timeInfo["second"] = String(gps.time.second());

          hasInfo = true;
        }

        if (gps.date.isValid()) {
          JsonObject dateInfo = gpsInfo.createNestedObject("date");

          dateInfo["day"] = String(gps.date.day());
          dateInfo["month"] = String(gps.date.month());
          dateInfo["year"] = String(gps.date.year());
          
          hasInfo = true;
        }

        if (gps.location.isValid()) {
          JsonObject locationInfo = gpsInfo.createNestedObject("location");
          locationInfo["lat"] = String(gps.location.lat(), 7);
          locationInfo["lng"] = String(gps.location.lng(), 7);

          hasInfo = true;
        }

      }
      return gpsInfo;
    }

};