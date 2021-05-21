#include "temperature.h"

// adresse sonde {0x28, 0x6d, 0xac, 0x75, 0xd0, 0x01, 0x3c, 0x6c}

Temperature::Temperature(){};

float Temperature::releverTemperatureSurSonde(){
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);

    if (tempC == DEVICE_DISCONNECTED_C){
        Serial.println("Error: Could not read temperature data");
    }
    return tempC;
};

void Temperature::begin(){
    oneWire.begin(PIN_TEMP);
    sensors.setOneWire(&oneWire);
    sensors.begin();
    sensors.getAddress(sonde1, 0);
    log_d("adresse de la sonde: 0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x", sonde1[0], sonde1[1], sonde1[2], sonde1[3], sonde1[4], sonde1[5], sonde1[6], sonde1[7]);
}
