// network.cpp
#include "network.h"
#include "utils.h"

NetworkManager::NetworkManager() {}

void NetworkManager::connectToWiFi() {
  
  
  WiFi.begin(env("ssid").c_str(), env("password").c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au Wi-Fi en cours...");
  }

  Serial.println("Connecté au réseau Wi-Fi");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}
