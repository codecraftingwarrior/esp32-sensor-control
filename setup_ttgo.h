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
    WebServiceController(AsyncWebServer& server, std::vector<Sensor>& sensors, int sensorCount, std::vector<LED>& ledsRef, int ledCount);

private:
    AsyncWebServer& server;
    std::vector<Sensor>& sensors;
    int sensorCount;
    std::vector<LED>& leds;
    int ledCount;
    void findSensors(AsyncWebServerRequest *request);
    void updateThreshold(AsyncWebServerRequest *request);
    void switchLed(AsyncWebServerRequest *request);
    void toggleLed(AsyncWebServerRequest *request);
    void fetchLeds(AsyncWebServerRequest *request);
    void test(AsyncWebServerRequest *request);
}; 

#endif
