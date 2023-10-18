#include <Arduino.h>
#include <functional>
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

void setup() {
  Serial.begin(115200);
  
  NetworkManager::connectToWiFi();
  
  int leds[2] = {RED_PIN, YELLOW_PIN};
  initLeds(leds, 2);

  Sensor sensors[2];
  sensors[0] = createBrightnessSensor(LIGHT_SENSOR_PIN, "Photoresistance");
  sensors[1] = createTemperatureSensor(TEMPERATURE_SENSOR_PIN, "Thermistance");

  WebServiceController controller(server, sensors, 2);
  server.begin();
}

void loop() {

  

}
