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

Sensor lightSensor;
Sensor temperatureSensor;

void setup() {
  Serial.begin(115200);
  
  NetworkManager::connectToWiFi();

  LED lightLED = LED::createLED(RED_PIN, false, "red", "LED light control");
  LED temperatureLED = LED::createLED(YELLOW_PIN, false, "yellow", "LED temperature control");

  lightSensor = Sensor::createSensor(Sensor::SensorType::BRIGHTNESS_SENSOR, LIGHT_SENSOR_PIN, "Photoresistance", lightSensorThreshold);
  temperatureSensor = Sensor::createSensor(Sensor::SensorType::TEMPERATURE_SENSOR, TEMPERATURE_SENSOR_PIN, "Thermistance", temperatureSensorThreshold);

  Sensor sensors[2] = {lightSensor, temperatureSensor};
  LED leds[2] = {lightLED, temperatureLED};
  
  WebServiceController controller(server, sensors, 2, leds, 2);
  
  server.begin();
}

void loop() {

  

}
