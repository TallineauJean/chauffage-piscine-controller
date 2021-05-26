#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <temperature.h>
#include <http.h>

#define LED_PIN 2

// preferences -> https://www.tutorialspoint.com/esp32_for_iot/esp32_for_iot_preferences.htm
// ESPasync -> https://github.com/me-no-dev/ESPAsyncWebServer
// ESPDIFF -> https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spiffs.html

const char *ssid = "Livebox-1EAA";
const char *password = "WntcVavEGakkVjRHTD";

Temperature temperature;
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, "192.168.1.16", 8888);

int status = WL_IDLE_STATUS;

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

}

void loop(){
    delay(1000);
    float listeTemperatures[2];

    temperature.releverTemperatureSurSonde(listeTemperatures);
    float temperatureAir = listeTemperatures[0];
    float temperatureEau = listeTemperatures[1];
    
    Serial.println("Temp eau -> %.2f -- Temp air -> %.2f", temperatureEau, temperatureAir);
  
}








  // delay(3000);

  // char data[10];
      // snprintf(data, 10, "temp=%f", temp);

  //String postData = "temperature=21.9";
  // client.beginRequest();
  // client.post("/temperature");
  // client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  //client.sendHeader("Content-Length", postData.length());
  //client.sendHeader("X-Custom-Header", "custom-header-value");
  // client.beginBody();
  // client.print(postData);
  //client.endRequest();

  //int statusCode = client.responseStatusCode();
  // String response = client.responseBody();

  // Serial.print("Status code: ");
  // Serial.println(statusCode);
  // Serial.print("Response: ");
  // Serial.println(response);