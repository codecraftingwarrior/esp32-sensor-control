#include <Arduino.h>
#include "setup_ttgo.h"
#include "network.h"
#include "utils.h"

#define LIGHT_SENSOR_PIN 15
#define TEMPERATURE_SENSOR_PIN 2

#define RED_PIN 17
#define YELLOW_PIN 16

int lightSensorThreshold = 600;
int temperatureSensorThreshold = 50;
AsyncWebServer server(80);

std::vector<LED> ledVector;
std::vector<Sensor> sensorVector;
WebServiceController* controller = nullptr;  // Déclaration de controller comme pointeur global

void setup() {
  Serial.begin(9600);
  
  NetworkManager::connectToWiFi();

  LED lightLED = LED::createLED(RED_PIN, false, "red", "LED light control");
  LED temperatureLED = LED::createLED(YELLOW_PIN, false, "yellow", "LED temperature control");

  sensorVector.push_back(Sensor::createSensor(Sensor::SensorType::BRIGHTNESS_SENSOR, LIGHT_SENSOR_PIN, "Photoresistance", lightSensorThreshold));
  sensorVector.push_back(Sensor::createSensor(Sensor::SensorType::TEMPERATURE_SENSOR, TEMPERATURE_SENSOR_PIN, "Thermistance", temperatureSensorThreshold));

  ledVector.push_back(LED::createLED(RED_PIN, false, "red", "LED light control"));
  ledVector.push_back(LED::createLED(YELLOW_PIN, false, "yellow", "LED temperature control"));
  
  controller = new WebServiceController(server, sensorVector, sensorVector.size(), ledVector, ledVector.size());
  
  server.begin();
}

void loop() {

}
