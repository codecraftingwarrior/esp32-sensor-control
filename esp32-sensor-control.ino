#include <Arduino.h>
#include <esp_system.h>
#include "setup_ttgo.h"
#include "network.h"
#include "utils.h"
#include <TFT_eSPI.h>  
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  

#define LIGHT_SENSOR_PIN 36
#define TEMPERATURE_SENSOR_PIN 37

#define NORMAL_LED_PIN 17
#define LIGHT_CONTROL_LED_PIN 33
#define TEMPERATURE_CONTROL_LED_PIN 37

int lightSensorThreshold = 500;
int temperatureSensorThreshold = 20;

AsyncWebServer server(80);

std::vector<LED> ledVector;
std::vector<Sensor*> sensorVector;
WebServiceController* controller = nullptr; 

LED lightLED;
LED temperatureLED;
LED normalLED;

Sensor lightSensor;
Sensor temperatureSensor;

void setup() {
  Serial.begin(9600);

  tft.init();
  tft.setRotation(1);

  NetworkManager::connectToWiFi();

  normalLED = LED::createLED(NORMAL_LED_PIN, false, "red", "LED Normal");
  lightLED = LED::createLED(LIGHT_CONTROL_LED_PIN, false, "green", "LED light control");
  temperatureLED = LED::createLED(TEMPERATURE_CONTROL_LED_PIN, false, "yellow", "LED temperature control");

  lightSensor = Sensor::createSensor(Sensor::SensorType::BRIGHTNESS_SENSOR, LIGHT_SENSOR_PIN, "Photoresistance", lightSensorThreshold);
  temperatureSensor = Sensor::createSensor(Sensor::SensorType::TEMPERATURE_SENSOR, TEMPERATURE_SENSOR_PIN, "Thermistance", temperatureSensorThreshold);

  sensorVector.push_back(&lightSensor);
  sensorVector.push_back(&temperatureSensor);

  ledVector.push_back(normalLED);
  ledVector.push_back(lightLED);
  ledVector.push_back(temperatureLED);

  controller = new WebServiceController(server, sensorVector, sensorVector.size(), ledVector, ledVector.size());

  server.begin();
}

void loop() {

  static unsigned long previousMillis = 0;
  const long interval = 600; 

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float currentLightValue = lightSensor.getCurrentValue();
    float currentTemperatureValue = temperatureSensor.getCurrentValue();

    updateTftDisplay(tft, lightSensor, temperatureSensor);

    if (currentLightValue < lightSensor.getThreshold())
      lightLED.blink(500);


    if (currentTemperatureValue < temperatureSensor.getThreshold())
      normalLED.blink(300);
  }
}
