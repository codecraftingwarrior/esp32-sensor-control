#include <Arduino.h>

#include "utils.h"
#include <FS.h>
#include <ArduinoJson.h>


String env(String key) {
  
  String configFile = "{\"ssid\":\"Code\ Crafting\ Warrior\", \"password\":\"azertyam\"}"; //simuler un fichier d'environnement
  
  if (!configFile) {
    Serial.println("Erreur lors de l'ouverture du fichier JSON.");
    return ""; 
  }

  DynamicJsonDocument jsonDoc(1024); 
  DeserializationError error = deserializeJson(jsonDoc, configFile);

  if (error) {
    Serial.println("Erreur lors de la lecture du fichier JSON.");
    return ""; 
  }

  if (jsonDoc.containsKey(key)) {
    return jsonDoc[key].as<String>();
  } else {
    Serial.println("Variable non trouvée dans le fichier JSON.");
    return ""; 
  }
}

void updateTftDisplay(TFT_eSPI tft, Sensor lightSensor, Sensor temperatureSensor) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1.6);
  tft.print("Light = ");
  tft.println(lightSensor.getCurrentValue());
  tft.println();
  tft.print("Temperature = ");
  tft.println(temperatureSensor.getCurrentValue());
  tft.println();
  tft.print("Light threshold = ");
  tft.println(lightSensor.getThreshold());
  tft.println();
  tft.print("Temperature threshold = ");
  tft.println(temperatureSensor.getThreshold());
}
