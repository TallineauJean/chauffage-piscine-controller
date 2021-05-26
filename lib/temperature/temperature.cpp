#include "temperature.h"

// adresse sonde1 {0x28, 0x5c, 0x4c, 0x75, 0xd0, 0x01, 0x3c, 0x6c}
// adresse sonde2 {0x28, 0x6d, 0xac, 0x75, 0xd0, 0x01, 0x3c, 0x6c}

Temperature::Temperature(){};

void Temperature::releverTemperatureSurSonde(float listeTemperatures[]){
    sensors.requestTemperatures();
    float tempEau = sensors.getTempC(sondeEau);
    float tempAir = sensors.getTempC(sondeAir);

    if (tempEau == DEVICE_DISCONNECTED_C){
        Serial.println("Erreur de lecture sur la sonde d'eau");
    }

    if (tempAir == DEVICE_DISCONNECTED_C){
        Serial.println("Erreur de lecture sur la sonde d'air");
    }

    listeTemperatures[0] = tempAir;
    listeTemperatures[1] = tempEau;
}

void Temperature::begin(){
    oneWire.begin(PIN_TEMP);
    sensors.setOneWire(&oneWire);
    sensors.begin();
    sensors.getAddress(sondeAir, 0);
    log_d("adresse de la sondeAir : {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x}", sondeAir[0], sondeAir[1], sondeAir[2], sondeAir[3], sondeAir[4], sondeAir[5], sondeAir[6], sondeAir[7]);

    sensors.getAddress(sondeEau, 1);
    log_d("adresse de la sondeEau : {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x}", sondeEau[0], sondeEau[1], sondeEau[2], sondeEau[3], sondeEau[4], sondeEau[5], sondeEau[6], sondeEau[7]);
}
