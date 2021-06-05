#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <temperature.h>
#include <http.h>
#include <analogWrite.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

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
    
    // WS
    ws.onEvent(onEvent);
    server.addHandler(&ws);
    char data[18];
    snprintf(data, 18, "%.2f;%.2f;%.2f", temperatureEauEntree, temperatureEauSortie, temperatureAir);
    ws.textAll(String(data));
    ws.cleanupClients();
}