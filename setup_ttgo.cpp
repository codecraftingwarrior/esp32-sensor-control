#include <Arduino.h>
#define ASYNCWEBSERVER_REGEX 1
#include "setup_ttgo.h"
#include <ESPAsyncWebSrv.h>
#include <ArduinoJson.h>

void initLeds(int leds[], int size) {
  for (int i = 0; i < size; i++)
    pinMode(leds[i], OUTPUT);
}

WebServiceController::WebServiceController(AsyncWebServer &serverRef, std::vector<Sensor> &sensorsRef, int sensorCountRef, std::vector<LED> &ledsRef, int ledCountRef)
  : server(serverRef), sensors(sensorsRef), sensorCount(sensorCountRef), leds(ledsRef), ledCount(ledCountRef) {

  server.on("/sensors", HTTP_GET, std::bind(&WebServiceController::findSensors, this, std::placeholders::_1));
  server.on("/sensors/{id}/threshold", HTTP_PUT, std::bind(&WebServiceController::updateThreshold, this, std::placeholders::_1));

  server.on("/leds", HTTP_GET, std::bind(&WebServiceController::fetchLeds, this, std::placeholders::_1));
  server.on("/switch-leds", HTTP_PUT, std::bind(&WebServiceController::switchLed, this, std::placeholders::_1));
  server.on("/toggle-leds", HTTP_PUT, std::bind(&WebServiceController::toggleLed, this, std::placeholders::_1));
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
  Sensor *sensor = NULL;

  bool founded = false;
  for (int i = 0; i < this->sensorCount; i++)
    if (this->sensors[i].getPinId() == sensorId) {
      founded = true;
      sensor = &this->sensors[i];
      break;
    }

  if (founded) {
    // Sensor &sensor = this->sensors[sensorId];

    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, request->arg("plain"));

    if (jsonDoc.containsKey("threshold")) {
      float newThreshold = jsonDoc["threshold"];

      sensor->setThreshold(newThreshold);

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
  DynamicJsonDocument responseDoc(1024);
  const char *ID_FIELD = "pinId";
  const char *NEW_STATE_FIELD = "newState";
  int statusCode = 200;

  String rawId = "";
  String newState = "";
  if (request->hasParam(ID_FIELD))
    rawId = request->getParam(ID_FIELD)->value();

  if (request->hasParam(NEW_STATE_FIELD))
    newState = request->getParam(NEW_STATE_FIELD)->value();

  if (rawId == "" || rawId == NULL) {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "Veuillez fournir le numero du pin.";
  }

  if (newState == "" || newState == NULL) {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "Veuillez fournir le nouvel état de la LED";
  }

  int ledPinId = rawId.toInt();
  LED *led = NULL;

  bool founded = false;
  for (int i = 0; i < this->ledCount; i++)
    if (this->leds[i].getPinId() == ledPinId) {
      founded = true;
      led = &this->leds[i];
      break;
    }

  if (founded) {
    if (newState == "on") {
      led->switchOn();
    } else if (newState == "off") {
      led->switchOff();
    } else {
      responseDoc["state"] = "failure";
      responseDoc["error"] = "Etat invalide";

      String jsonResponse;
      serializeJson(responseDoc, jsonResponse);

      statusCode = 422;
      request->send(statusCode, "application/json", jsonResponse);
    }

    responseDoc["state"] = "success";
    responseDoc["Etat de la LED mis à jour avec succès."];

  } else {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "LED introuvable.";
    statusCode = 404;
  }

  String jsonResponse;
  serializeJson(responseDoc, jsonResponse);

  request->send(statusCode, "application/json", jsonResponse);
}

void WebServiceController::toggleLed(AsyncWebServerRequest *request) {
  const char *ID_FIELD = "pinId";
  DynamicJsonDocument responseDoc(1024);

  String rawId = "";
  if (request->hasParam(ID_FIELD))
    rawId = request->getParam(ID_FIELD)->value();

  if (rawId == "" || rawId == NULL) {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "Veuillez fournir le numero du pin.";

    String jsonResponse;
    serializeJson(responseDoc, jsonResponse);
    request->send(422, "application/json", jsonResponse);
  }

  int ledPinId = rawId.toInt();
  LED *led = NULL;

  bool founded = false;
  for (int i = 0; i < this->ledCount; i++)
    if (this->leds[i].getPinId() == ledPinId) {
      founded = true;
      led = &this->leds[i];
      break;
    }

  if (founded) {
    led->toggle();
    responseDoc["state"] = "success";
  } else {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "LED introuvable.";

    String jsonResponse;
    serializeJson(responseDoc, jsonResponse);
    request->send(422, "application/json", jsonResponse);
  }

  String jsonResponse;
  serializeJson(responseDoc, jsonResponse);
  request->send(200, "application/json", jsonResponse);
}

void WebServiceController::fetchLeds(AsyncWebServerRequest *request) {
  DynamicJsonDocument leds(1024);
  JsonArray ledsArray = leds.to<JsonArray>();

  for (int i = 0; i < this->ledCount; i++) {
    JsonObject ledObj = ledsArray.createNestedObject();
    ledObj["pinId"] = this->leds[i].getPinId();
    ledObj["name"] = this->leds[i].getName();
    ledObj["color"] = this->leds[i].getColor();
    ledObj["isOn"] = this->leds[i].getIsOn();
  }

  String jsonResponse;
  serializeJson(leds, jsonResponse);

  request->send(200, "application/json", jsonResponse);
}
