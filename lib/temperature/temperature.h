#include <DallasTemperature.h>

#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#define PIN_TEMP 22

class Temperature {
    private:
        OneWire oneWire;
        DallasTemperature sensors;
        DeviceAddress sonde1;
        
    public: 
        float releverTemperatureSurSonde();
        void begin();
        Temperature();
};

#endif