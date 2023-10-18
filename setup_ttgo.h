#ifndef SETUP_TTGO_H
#define SETUP_TTGO_H

#include <ESPAsyncWebSrv.h>
#include "data_type.h"


void initLeds(int leds[], int size);

void getTemperature(int pin);

Sensor createBrightnessSensor(int pin, String name);
Sensor createTemperatureSensor(int pin, String name);

class WebServiceController {
public:
    WebServiceController(AsyncWebServer& server, Sensor* sensors, int sensorCount);

private:
    AsyncWebServer& server;
    Sensor* sensors;
    int sensorCount;
    void findSensors(AsyncWebServerRequest *request);
}; 

#endif
