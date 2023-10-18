#include <Arduino.h>

#include "utils.h"
#include <FS.h>
#include <ArduinoJson.h>

String env(String key) {
  
  String configFile = "{\"ssid\":\"ssid\", \"password\":\"thereisapwd\"}"; //simuler un fichier d'environnement
  
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
