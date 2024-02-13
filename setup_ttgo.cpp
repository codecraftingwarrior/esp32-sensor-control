#include <Arduino.h>
#define ASYNCWEBSERVER_REGEX 1
#include "setup_ttgo.h"
#include <ESPAsyncWebSrv.h>
#include <ArduinoJson.h>

WebServiceController::WebServiceController(AsyncWebServer &serverRef, std::vector<Sensor*> &sensorsRef, int sensorCountRef, std::vector<LED> &ledsRef, int ledCountRef)
  : server(serverRef), sensors(sensorsRef), sensorCount(sensorCountRef), leds(ledsRef), ledCount(ledCountRef) {

  /*
    Ajout des différents endpoints et des callbacks associés
  */
  
  server.on("/sensors", HTTP_GET, std::bind(&WebServiceController::findSensors, this, std::placeholders::_1));
  server.on("/sensors/update-threshold", HTTP_PUT, std::bind(&WebServiceController::updateThreshold, this, std::placeholders::_1));

  server.on("/leds", HTTP_GET, std::bind(&WebServiceController::fetchLeds, this, std::placeholders::_1));
  server.on("/switch-leds", HTTP_PUT, std::bind(&WebServiceController::switchLed, this, std::placeholders::_1));
  server.on("/toggle-leds", HTTP_PUT, std::bind(&WebServiceController::toggleLed, this, std::placeholders::_1));
}

void WebServiceController::findSensors(AsyncWebServerRequest *request) {
  DynamicJsonDocument sensorData(1024);
  JsonArray sensorArray = sensorData.to<JsonArray>();

  for (int i = 0; i < this->sensorCount; i++) {
    JsonObject sensorObj = sensorArray.createNestedObject();
    sensorObj["pinId"] = this->sensors[i]->getPinId();
    sensorObj["name"] = this->sensors[i]->getName();
    sensorObj["type"] = this->sensors[i]->getType();
    sensorObj["isOn"] = this->sensors[i]->getIsOn();
    sensorObj["currentValue"] = this->sensors[i]->getCurrentValue();
  }

  String jsonResponse;
  serializeJson(sensorData, jsonResponse);

  request->send(200, "application/json", jsonResponse);
}

void WebServiceController::updateThreshold(AsyncWebServerRequest *request) {
  const char *TYPE_FIELD = "type";
  const char *THRESHOLD_FIELD = "threshold";
  DynamicJsonDocument responseDoc(1024);
  int statusCode = 200;

  String type = "";
  if (request->hasParam(TYPE_FIELD))
    type = request->getParam(TYPE_FIELD)->value();

  String rawThreshold = "";
  if (request->hasParam(THRESHOLD_FIELD))
    rawThreshold = request->getParam(THRESHOLD_FIELD)->value();

  if ((type == "" || type == NULL) || (type != Sensor::SENSOR_BRIGHTNESS_TYPE && type != Sensor::SENSOR_TEMPERATURE_TYPE)) {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "Veuillez fournir un type de capteur valide.";
    statusCode = 422;

    String jsonResponse;
    serializeJson(responseDoc, jsonResponse);

    request->send(statusCode, "application/json", jsonResponse);
  }

  if (rawThreshold == "" || rawThreshold == NULL) {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "Un seuil valide.";
    statusCode = 422;

    String jsonResponse;
    serializeJson(responseDoc, jsonResponse);

    request->send(statusCode, "application/json", jsonResponse);
  }

  Sensor *sensor = NULL;

  bool founded = false;
  for (int i = 0; i < this->sensorCount; i++)
    if (this->sensors[i]->getType().equals(type)) {
      founded = true;
      sensor = this->sensors[i];
      break;
    }

  if (founded) {
    float newThreshold = rawThreshold.toFloat();

    sensor->setThreshold(newThreshold);

    responseDoc["state"] = "success";
    responseDoc["message"] = "Seuil mis à jour avec succés.";
  } else {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "Capteur introuvable.";
    statusCode = 404;
  }

  String jsonResponse;
  serializeJson(responseDoc, jsonResponse);

  request->send(statusCode, "application/json", jsonResponse);
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
    responseDoc["message"] = "Operation reussi.";
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
