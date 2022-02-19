#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <temperature.h>
#include <http.h>
#include <analogWrite.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Wire.h>

// preferences -> https://www.tutorialspoint.com/esp32_for_iot/esp32_for_iot_preferences.htm
// ESPasync -> https://github.com/me-no-dev/ESPAsyncWebServer
// ESPDIFF -> https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spiffs.html

const char *ssid = "Livebox-1EAA";
const char *password = "WntcVavEGakkVjRHTD";

Temperature temperature;
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, "192.168.1.16", 8888);
Preferences preferences;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Penser à changer les pins en accord avec le PCB
const int rs = 17, en = 16, d4 = 21, d5 = 19, d6 = 18, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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
  lcd.begin(16,2);  

  // WIFI
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting toto");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("Création du client HTTP");
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());


  // SPIFFS pour le site static
  if(!SPIFFS.begin(true, "/spiffs", 32)){
      log_e("An Error has occurred while mounting SPIFFS");
    }
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html").setCacheControl("max-age=6000");
  server.begin();
}

void loop(){
    delay(1000);

    // Températures
    float listeTemperatures[3];
    temperature.releverTemperatureSurSonde(listeTemperatures);
    float temperatureEauEntree = listeTemperatures[0];
    float temperatureEauSortie = listeTemperatures[1];
    float temperatureAir = listeTemperatures[2];
    boolean isPompeActive = temperatureEauSortie > temperatureEauEntree && temperatureEauEntree < 28.0; 
    
    // LCD
    char lcdData[16];
    snprintf(lcdData, 16, "%.1f %.1f %.1f", temperatureAir, temperatureEauEntree, temperatureEauSortie);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" air  in   out");
    lcd.setCursor(0,1);
    lcd.print(String(lcdData));

    // WS
    ws.onEvent(onEvent);
    server.addHandler(&ws);
    char data[25];
    snprintf(data, 25, "%.2f;%.2f;%.2f;%s", temperatureEauEntree, temperatureEauSortie, temperatureAir, isPompeActive ? "1" : "0");
    ws.textAll(String(data));
    ws.cleanupClients(); 
}