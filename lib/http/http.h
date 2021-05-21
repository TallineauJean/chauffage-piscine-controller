#include <WiFi.h>
#include <ArduinoHttpClient.h>

#ifndef CLIENT_HTTP
#define CLIENT_HTTP
#define char serverAddress[] = "192.168.1.16";
#define int port = 8888;

class Http {
    public:
        Http();
        void posterLesDonnees(float temperatureReleve);
    private:
        HttpClient client;
};

#endif