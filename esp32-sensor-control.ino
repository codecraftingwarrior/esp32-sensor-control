#include <Arduino.h>
#include <esp_system.h>
#include "setup_ttgo.h"
#include "network.h"
#include "utils.h"
#include <TFT_eSPI.h>
#include <SPI.h>
 
#include "firebase_utils.h"

#define LIGHT_SENSOR_PIN 36
#define TEMPERATURE_SENSOR_PIN 37

#define NORMAL_LED_PIN 17
#define LIGHT_CONTROL_LED_PIN 33

TFT_eSPI tft = TFT_eSPI();

AsyncWebServer server(80);

std::vector<LED> ledVector;
std::vector<Sensor*> sensorVector;

// Controller pour les APIRest
WebServiceController* controller = nullptr;

//Objet Firebase permettant d'interagir avec la base de données
FirebaseUtils* firebase = nullptr;

//Valeur seuil pour capteur luminosité
int lightSensorThreshold = 500;

//Valeur seuil pour capteur temperature
int temperatureSensorThreshold = 20;

LED lightLED;
LED normalLED;

Sensor lightSensor;
Sensor temperatureSensor;

void setup() {
  Serial.begin(9600); // Démarrage de la communication série à une vitesse de 9600 bauds

  tft.init();
  tft.setRotation(1);

  randomSeed(analogRead(0));
  //Appel de la méthode permettant de se connecter au WI-FI
  NetworkManager::connectToWiFi();

  normalLED = LED::createLED(NORMAL_LED_PIN, false, "red", "LED temperature");
  lightLED = LED::createLED(LIGHT_CONTROL_LED_PIN, false, "green", "LED light control");

  lightSensor = Sensor::createSensor(Sensor::SensorType::BRIGHTNESS_SENSOR, LIGHT_SENSOR_PIN, "Photoresistance", lightSensorThreshold);
  temperatureSensor = Sensor::createSensor(Sensor::SensorType::TEMPERATURE_SENSOR, TEMPERATURE_SENSOR_PIN, "Thermistance", temperatureSensorThreshold);

  sensorVector.push_back(&lightSensor);
  sensorVector.push_back(&temperatureSensor);

  ledVector.push_back(normalLED);
  ledVector.push_back(lightLED);

  controller = new WebServiceController(server, sensorVector, sensorVector.size(), ledVector, ledVector.size());
  firebase = new FirebaseUtils();

  //Initialisation de la communication avec firebase (authentification, configuration API_KEY et DATABASE_URL)
  firebase->init();

  //Démarrage du Server Web et exposition des API REST
  server.begin();
}

void loop() {

  static unsigned long previousMillis = 0;
  const long interval = 600;

  unsigned long currentMillis = millis();


  //Recupération et affichage sur l'écran, par intervalle régulier, les valeurs des capteurs de luminosité et de resistance
  //Envoie par intervalle regulier des données des capteurs à la base de données firebase
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float currentLightValue = lightSensor.getCurrentValue();
    float currentTemperatureValue = temperatureSensor.getCurrentValue();

    //Mise à jour des informations affichées sur l'écran TFT
    updateTftDisplay(tft, lightSensor, temperatureSensor);

    if (currentLightValue < lightSensor.getThreshold())
      lightLED.blink(500);


    if (currentTemperatureValue < temperatureSensor.getThreshold())
      normalLED.blink(300);

    //Envoie des données des capteurs dans firebase realtime database
    firebase->storeSensorData(currentLightValue, currentTemperatureValue);
    firebase->storeSystemUsage(random(0, 95), random(0, 95));
  }
}
