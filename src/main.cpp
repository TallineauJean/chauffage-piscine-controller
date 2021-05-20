#include <Arduino.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>

#define LED_PIN 2
#define TEMP_PIN 22

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

const char *ssid = "Livebox-1EAA";
const char *password = "WntcVavEGakkVjRHTD";

char serverAddress[] = "192.168.1.16"; // server address
int port = 8888;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup(){
  Serial.begin(115200);
  // pinMode(LED_PIN, OUTPUT);
  sensors.begin();
  // WiFi.begin(ssid, password);

  //Serial.println("\nConnecting");

  // while(WiFi.status() != WL_CONNECTED){
  //    Serial.print(".");
  //     delay(100);
  // }

  // Serial.println("\nConnected to the WiFi network");
  // Serial.print("Local ESP32 IP: ");
  // Serial.println(WiFi.localIP());
}

void loop(){
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C){
    Serial.print("Température : ");
    Serial.println(tempC);
  } else {
    Serial.println("Error: Could not read temperature data");
  }
  // delay(3000);

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
}

void temp(void){
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C){
    Serial.print("Température : ");
    Serial.println(tempC);
  } else {
    Serial.println("Error: Could not read temperature data");
  }
}

void blink(){
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(3000);
}