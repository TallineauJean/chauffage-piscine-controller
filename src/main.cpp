#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <temperature.h>
#include <http.h>
#include <analogWrite.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#define LED_PIN 2

#define RGB_G 12
#define RGB_B 13
#define RGB_R 14

#define RELAIS 27

// preferences -> https://www.tutorialspoint.com/esp32_for_iot/esp32_for_iot_preferences.htm
// ESPasync -> https://github.com/me-no-dev/ESPAsyncWebServer
// ESPDIFF -> https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spiffs.html

const char *ssid = "Livebox-1EAA";
const char *password = "WntcVavEGakkVjRHTD";

Temperature temperature;
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, "192.168.1.16", 8888);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

int status = WL_IDLE_STATUS;

void notifyClients() {
  ws.textAll(String("bonjour"));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void setup(){
  Serial.begin(115200);
  temperature.begin();
 
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }

  Serial.println("Création du client HTTP");

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(RELAIS, OUTPUT);
  digitalWrite(RELAIS, LOW);

  analogWrite(RGB_R, 128, 128);

  if(!SPIFFS.begin(true)){
      log_e("An Error has occurred while mounting SPIFFS");
    }

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html").setCacheControl("max-age=6000");
  server.begin();
}

void loop(){
    delay(1000);
    float listeTemperatures[3];

    temperature.releverTemperatureSurSonde(listeTemperatures);
    float temperatureEauEntree = listeTemperatures[0];
    float temperatureEauSortie = listeTemperatures[1];
    float temperatureAir = listeTemperatures[2];

    char log[74];
    snprintf(log, 74, "Temp eau entrée -> %.2f -- Temp eau sortie -> %.2f -- Temp air -> %.2f", temperatureEauEntree, temperatureEauSortie, temperatureAir);
    

    int gain = temperatureEauSortie - temperatureEauEntree;
    if(gain > 0){
     // analogWrite(RGB_R, 128, 128);
      digitalWrite(RELAIS, HIGH);
      analogWrite(RGB_G, 0, 128);
      analogWrite(RGB_B, 0, 128);
    } else if (gain < 0){
     // analogWrite(RGB_R, 50, 128);
      digitalWrite(RELAIS, LOW);
      analogWrite(RGB_G, 0, 128);
      analogWrite(RGB_B, 128, 128);
    } else {
     // analogWrite(RGB_R, 0, 128);
      digitalWrite(RELAIS, LOW);
      analogWrite(RGB_G, 128, 128);
      analogWrite(RGB_B, 0, 128);
    }

    Serial.println(log);


    // WS
    ws.onEvent(onEvent);
    server.addHandler(&ws);


    char data[18];
    snprintf(data, 18, "%.2f;%.2f;%.2f", temperatureEauEntree, temperatureEauSortie, temperatureAir);
    ws.textAll(String(data));

}