#include "temperature.h"

// adresse sonde1 {0x28, 0x5c, 0x4c, 0x75, 0xd0, 0x01, 0x3c, 0x6c}
// adresse sonde2 {0x28, 0x6d, 0xac, 0x75, 0xd0, 0x01, 0x3c, 0x6c}
// adresse sonde air {0x28, 0x71, 0xe9, 0x2f, 0x00, 0x00, 0x80, 0xfe}

Temperature::Temperature(){};

void Temperature::releverTemperatureSurSonde(float listeTemperatures[]){
    sensors.requestTemperatures();
    float tempEauEntree = sensors.getTempC(sondeEauEntree);
    float tempEauSortie = sensors.getTempC(sondeEauSortie);
    float tempAir = sensors.getTempC(sondeAir);

    if (tempEauEntree == DEVICE_DISCONNECTED_C){
        Serial.println("Erreur de lecture sur la sonde d'eau");
    }

    if (tempEauSortie == DEVICE_DISCONNECTED_C){
        Serial.println("Erreur de lecture sur la sonde d'air");
    }

    listeTemperatures[0] = tempEauEntree;
    listeTemperatures[1] = tempEauSortie;
    listeTemperatures[2] = tempAir;
}

void Temperature::begin(){
    oneWire.begin(PIN_TEMP);
    sensors.setOneWire(&oneWire);
    sensors.begin();

    sensors.getAddress(sondeEauEntree, 2);
    log_d("adresse de la sondeAir : {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x}", sondeEauEntree[0], sondeEauEntree[1], sondeEauEntree[2], sondeEauEntree[3], sondeEauEntree[4], sondeEauEntree[5], sondeEauEntree[6], sondeEauEntree[7]);

    sensors.getAddress(sondeEauSortie, 0);
    log_d("adresse de la sondeEau : {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x}", sondeEauSortie[0], sondeEauSortie[1], sondeEauSortie[2], sondeEauSortie[3], sondeEauSortie[4], sondeEauSortie[5], sondeEauSortie[6], sondeEauSortie[7]);

     sensors.getAddress(sondeAir, 1);
    log_d("adresse de la sondeAir : {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x}", sondeAir[0], sondeAir[1], sondeAir[2], sondeAir[3], sondeAir[4], sondeAir[5], sondeAir[6], sondeAir[7]);
}
