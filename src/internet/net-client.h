// HTTPS Client
#define MODEM_RX D3
#define MODEM_TX D4
SoftwareSerial HttpSerial(MODEM_RX, MODEM_TX, false, 256); 
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 650
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
// #define TINY_GSM_DEBUG Serial
#include <StreamDebugger.h>
#include <TinyGsmClient.h>
#include <ArduinoJson.h>
#include <HttpClient.h>

StreamDebugger debugger(HttpSerial, Serial);
TinyGsm modem(debugger);
const char *server = "test.com";
const char *resource = "/api/locations";
const char *apn  = "timbrasil.br";
const char gprsUser[] = "";
const char gprsPass[] = "";
// TinyGsm modem(HttpSerial); // NON DEBUG MODE
TinyGsmClientSecure client(modem);
HttpClient httpClient(client, server, 443);

class NetClient {
    // variable
    private: int m_value;

    // acsessor
    public: void init() {
        HttpSerial.begin(9600);
        Serial.println("connecting to GSM....");
        modem.gprsConnect(apn, gprsUser, gprsPass);
        Serial.println("waiting for network....");
        if (!modem.waitForNetwork()) {
            Serial.println("no network available");
            return;
        }
        if (!modem.hasSSL()) {
            Serial.println(F("SSL is not supported by this modem"));
        }
    }

    void post( JsonObject const value ) {
        String contentBody;
        serializeJson(value, contentBody);
        Serial.println(contentBody);

        Serial.print(F("Performing POST request... "));

        httpClient.connectionKeepAlive(); // Currently, this is needed for HTTPS
        httpClient.beginRequest();
        Serial.println(httpClient.post(resource));
        httpClient.sendHeader("Content-Type", "application/json");
        httpClient.sendHeader("Content-Length", contentBody.length());
        httpClient.beginBody();
        httpClient.println(contentBody);
        httpClient.endRequest();

        int status = httpClient.responseStatusCode();
        Serial.print("status: "); Serial.println(status);
        if (status < 0) {
            Serial.println("HTTP ERR: ");
            Serial.println(httpClient.responseBody());
            httpClient.stop();
            return;
        }
        httpClient.stop();
        Serial.print("Sent! ");
    }
};