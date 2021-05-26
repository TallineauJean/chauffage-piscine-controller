#include "http.h"

/*
void Http::begin(WifiClient wifi){
  HttpClient client = HttpClient(wifi, "192.168.1.16", 8888);
}

void Http::posterLesDonnees(float temperatureReleve){
    char data[10];
    snprintf(data, 10, "temp=%f", temp);
     String postData = snprintf("temperature=", String::string(temp, 2));
   client.beginRequest();
   client.post("/temperature");
 client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  client.sendHeader("Content-Length", postData.length());
  client.sendHeader("X-Custom-Header", "custom-header-value");
   client.beginBody();
   client.print(postData);
  client.endRequest();
}
*/
