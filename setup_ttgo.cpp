#include <Arduino.h>
#define ASYNCWEBSERVER_REGEX 1
#include "setup_ttgo.h"
#include <ESPAsyncWebSrv.h>
#include <ArduinoJson.h>


void initLeds(int leds[], int size) {
  for (int i = 0; i < size; i++)
    pinMode(leds[i], OUTPUT);
}

WebServiceController::WebServiceController(AsyncWebServer& server, Sensor* sensors, int sensorCount) : server(server), sensors(sensors), sensorCount(sensorCount) {
  
  server.on("/sensors", HTTP_GET, std::bind(&WebServiceController::findSensors, this, std::placeholders::_1));
  server.on("/sensors/{id}/threshold", HTTP_PUT, std::bind(&WebServiceController::updateThreshold, this, std::placeholders::_1));
  
  server.on("/leds/{id}", HTTP_PUT, std::bind(&WebServiceController::switchLed, this, std::placeholders::_1));
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

void WebServiceController::updateThreshold(AsyncWebServerRequest *request) {
  String rawId = request->pathArg(0);
  int sensorId = rawId.toInt();
  DynamicJsonDocument responseDoc(1024);

  bool founded = false;
  for(int i=0; i<this->sensorCount; i++) 
    if(this->sensors[i].getPinId() == sensorId)
      founded = true;

  if(founded) {
        Sensor &sensor = this->sensors[sensorId];

        DynamicJsonDocument jsonDoc(1024);
        deserializeJson(jsonDoc, request->arg("plain"));

        if (jsonDoc.containsKey("threshold")) {
            float newThreshold = jsonDoc["threshold"];
            
            sensor.setThreshold(newThreshold);
            
            responseDoc["success"] = true;
            responseDoc["message"] = "Seuil mis à jour avec succés.";
        } else {
            responseDoc["success"] = false;
            responseDoc["error"] = "Veuillez fournir le champ 'threshold'.";
        }
    
  } else {
    responseDoc["success"] = false;
    responseDoc["error"] = "Capteur introuvable.";
  }


  String jsonResponse;
  serializeJson(responseDoc, jsonResponse);

 request->send(200, "application/json", jsonResponse);  
}

void WebServiceController::switchLed(AsyncWebServerRequest *request) {
  String rawId = request->pathArg(0);
  int ledPinId = rawId.toInt();
  DynamicJsonDocument responseDoc(1024);

  // to be continue
}
