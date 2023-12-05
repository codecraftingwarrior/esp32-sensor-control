#include <Arduino.h>
#define ASYNCWEBSERVER_REGEX 1
#include "setup_ttgo.h"
#include <ESPAsyncWebSrv.h>
#include <ArduinoJson.h>

void initLeds(int leds[], int size)
{
  for (int i = 0; i < size; i++)
    pinMode(leds[i], OUTPUT);
}

WebServiceController::WebServiceController(AsyncWebServer &server, Sensor *sensors, int sensorCount, LED *leds, int ledCount) : server(server), sensors(sensors), sensorCount(sensorCount), leds(leds), ledCount(ledCount)
{

  server.on("/sensors", HTTP_GET, std::bind(&WebServiceController::findSensors, this, std::placeholders::_1));
  server.on("/sensors/{id}/threshold", HTTP_PUT, std::bind(&WebServiceController::updateThreshold, this, std::placeholders::_1));
  
  server.on("/leds", HTTP_GET, std::bind(&WebServiceController::fetchLeds, this, std::placeholders::_1));
  server.on("/leds/{id}", HTTP_PUT, std::bind(&WebServiceController::switchLed, this, std::placeholders::_1));
  server.on("/leds/{id}/toggle", HTTP_PUT, std::bind(&WebServiceController::toggleLed, this, std::placeholders::_1));
}

void WebServiceController::findSensors(AsyncWebServerRequest *request)
{
  DynamicJsonDocument sensorData(1024);
  JsonArray sensorArray = sensorData.to<JsonArray>();

  for (int i = 0; i < this->sensorCount; i++)
  {
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

void WebServiceController::updateThreshold(AsyncWebServerRequest *request)
{
  String rawId = request->pathArg(0);
  int sensorId = rawId.toInt();
  DynamicJsonDocument responseDoc(1024);
  Sensor *sensor = NULL;

  bool founded = false;
  for (int i = 0; i < this->sensorCount; i++)
    if (this->sensors[i].getPinId() == sensorId)
    {
      founded = true;
      sensor = &this->sensors[i];
      break;
    }

  if (founded)
  {
    // Sensor &sensor = this->sensors[sensorId];

    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, request->arg("plain"));

    if (jsonDoc.containsKey("threshold"))
    {
      float newThreshold = jsonDoc["threshold"];

      sensor->setThreshold(newThreshold);

      responseDoc["success"] = true;
      responseDoc["message"] = "Seuil mis à jour avec succés.";
    }
    else
    {
      responseDoc["success"] = false;
      responseDoc["error"] = "Veuillez fournir le champ 'threshold'.";
    }
  }
  else
  {
    responseDoc["success"] = false;
    responseDoc["error"] = "Capteur introuvable.";
  }

  String jsonResponse;
  serializeJson(responseDoc, jsonResponse);

  request->send(200, "application/json", jsonResponse);
}

void WebServiceController::switchLed(AsyncWebServerRequest *request)
{
  String rawId = request->pathArg(0);
  int ledPinId = rawId.toInt();
  DynamicJsonDocument responseDoc(1024);
  LED *led = NULL;

  bool founded = false;
  for (int i = 0; i < this->ledCount; i++)
    if (this->leds[i].getPinId() == ledPinId)
    {
      founded = true;
      led = &this->leds[i];
      break;
    }

  if (founded)
  {
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, request->arg("plain"));

    if (jsonDoc.containsKey("newState"))
    {
      if (jsonDoc["newState"] == "on")
        led->switchOn();

      else if (jsonDoc["newState"] == "off")
        led->switchOff();

      responseDoc["state"] = "success";
    }
    else
    {
      responseDoc["state"] = "failure";
      responseDoc["error"] = "Veuillez fournir le champ 'newState'.";
    }
  }
  else
  {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "LED introuvable.";
  }

  String jsonResponse;
  serializeJson(responseDoc, jsonResponse);

  request->send(200, "application/json", jsonResponse);
}

void WebServiceController::toggleLed(AsyncWebServerRequest *request)
{
  String rawId = request->pathArg(0);
  int ledPinId = rawId.toInt();
  DynamicJsonDocument responseDoc(1024);
  LED *led = NULL;

  bool founded = false;
  for (int i = 0; i < this->ledCount; i++)
    if (this->leds[i].getPinId() == ledPinId)
    {
      founded = true;
      led = &this->leds[i];
      break;
    }

  if (founded)
  {
    led->toggle();
    responseDoc["state"] = "success";
  }
  else
  {
    responseDoc["state"] = "failure";
    responseDoc["error"] = "LED introuvable.";
  }

  String jsonResponse;
  serializeJson(responseDoc, jsonResponse);

  request->send(200, "application/json", jsonResponse);
}

void WebServiceController::fetchLeds(AsyncWebServerRequest *request) {
  DynamicJsonDocument leds(1024);
  JsonArray ledsArray = leds.to<JsonArray>();

  for (int i = 0; i < this->ledCount; i++)
  {
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
