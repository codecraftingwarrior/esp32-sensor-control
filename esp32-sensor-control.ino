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
  
  int leds[2] = {RED_PIN, YELLOW_PIN};
  initLeds(leds, 2);

  lightSensor = Sensor::createSensor(Sensor::SensorType::BRIGHTNESS_SENSOR, LIGHT_SENSOR_PIN, "Photoresistance");
  temperatureSensor = Sensor::createSensor(Sensor::SensorType::TEMPERATURE_SENSOR, TEMPERATURE_SENSOR_PIN, "Thermistance");

  Sensor sensors[2] = {lightSensor, temperatureSensor};

  WebServiceController controller(server, sensors, 2);
  
  server.begin();
}

void loop() {

  

}
