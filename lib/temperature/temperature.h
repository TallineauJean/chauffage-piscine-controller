#include <DallasTemperature.h>

#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#define PIN_TEMP 22

class Temperature {
    private:
        OneWire oneWire;
        DallasTemperature sensors;
        DeviceAddress sondeEauEntree;
        DeviceAddress sondeEauSortie;
        DeviceAddress sondeAir;
        
    public: 
        void releverTemperatureSurSonde(float listeTemperature[2]);
        void begin();
        Temperature();
};

#endif