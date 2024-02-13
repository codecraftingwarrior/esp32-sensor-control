// network.cpp
#include "network.h"
#include "utils.h"

/*
  Classe utilitaire permettant de gérer la connexion à un point d'accés WIFI
*/

NetworkManager::NetworkManager() {}

void NetworkManager::connectToWiFi() {
  
  WiFi.begin(env("ssid").c_str(), env("password").c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.printf("Connexion au Wi-Fi %s en cours...\n", env("ssid").c_str());
  }

  Serial.println("Connecté au réseau Wi-Fi");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}
