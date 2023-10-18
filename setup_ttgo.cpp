#include <Arduino.h>
#include "setup_ttgo.h"
#include <ESPAsyncWebSrv.h>
#include <ArduinoJson.h>


void initLeds(int leds[], int size) {
  for (int i = 0; i < size; i++)
    pinMode(leds[i], OUTPUT);
}

Sensor createBrightnessSensor(int pin, String name) {
  Sensor sensor;
  sensor.setPinId(pin);
  sensor.setName(name);
  sensor.setType("brightness");
  sensor.setIsOn(true);
  sensor.setCurrentValue(0);

  return sensor;
}

Sensor createTemperatureSensor(int pin, String name) {
  Sensor sensor;
  sensor.setPinId(pin);
  sensor.setName(name);
  sensor.setType("temperature");
  sensor.setIsOn(true);
  sensor.setCurrentValue(0);

  return sensor;
}


WebServiceController::WebServiceController(AsyncWebServer& server, Sensor* sensors, int sensorCount) : server(server), sensors(sensors), sensorCount(sensorCount) {
  server.on("/sensors", HTTP_GET, std::bind(&WebServiceController::findSensors, this, std::placeholders::_1));
}

void WebServiceController::findSensors(AsyncWebServerRequest *request) {
  DynamicJsonDocument sensorData(1024);
  JsonArray sensorArray = sensorData.to<JsonArray>();

  for (int i = 0; i < this->sensorCount; i++) {
    JsonObject sensorObj = sensorArray.createNestedObject();
    sensorObj["pinId"] = this->sensors[i].getPinId();
    sensorObj["name"] = this->sensors[i].getName();
    sensorObj["type"] = this->sensors[i].getType();
    sensorObj["isOn"] = this->sensors[i].getIsOn();
    sensorObj["currentValue"] = this->sensors[i].getCurrentValue();
  }

  String jsonResponse;
  serializeJson(sensorData, jsonResponse);

  request->send(200, "application/json", jsonResponse);
}
