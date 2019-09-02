// HTTPS Client
#define MODEM_RX D3
#define MODEM_TX D4
// #define TINY_GSM_DEBUG Serial
#include <lib/Http.h>
#include <ArduinoJson.h>

const char server[] = "test.com";
const char resource[] = "/api/locations";
const char endpoint[] = "https://test.com/api/locations";
const char apn[]  = "timbrasil.br";
const char gprsUser[] = "";
const char gprsPass[] = "";
HTTP httpClient(9600, MODEM_RX, MODEM_TX, 0);

class HttpClient {
    // variable
    private: int m_value;

    // acsessor
    public: void init() {
        Serial.println("connecting to GSM....");
        httpClient.configureBearer(apn);
        Serial.print("Cofigure bearer: ");
        Serial.println(httpClient.configureBearer("timbrasil.br"));
        Serial.println("waiting for network....");
    }

    void post( JsonObject const value ) {
        char response[64];
        Result result;
        char contentBody[100];
        serializeJson(value, contentBody);
        Serial.println(contentBody);

        Serial.print(F("Performing POST request... "));
        result = httpClient.connect();
        Serial.print("HTTP connect: ");
        Serial.println(result);

        result = httpClient.post(endpoint, contentBody, response);
        httpClient.disconnect();

        if (result != SUCCESS) {
            Serial.println("HTTP ERR: ");
            Serial.println(response);
            return;
        }
        Serial.print("Sent! ");
    }
};