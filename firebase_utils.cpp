#include "firebase_utils.h"
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

bool signupOK = false;

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

FirebaseUtils::FirebaseUtils() {}

void FirebaseUtils::init() {

  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void FirebaseUtils::storeSensorData(float light, float temperature) {
  if (Firebase.ready() && signupOK) {
    if (!Firebase.RTDB.setFloat(&fbdo, "readings/brightness", light)) {
      Serial.println("Storing brightness FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (!Firebase.RTDB.setFloat(&fbdo, "readings/temperature", temperature)) {
      Serial.println("Storing temperature FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}


void FirebaseUtils::storeSystemUsage(int cpuUsage, int ramUsage) {
  if (Firebase.ready() && signupOK) {
    if (!Firebase.RTDB.setFloat(&fbdo, "readings/cpu", cpuUsage)) {
      Serial.println("Storing cpu usage FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (!Firebase.RTDB.setFloat(&fbdo, "readings/ram", ramUsage)) {
      Serial.println("Storing ram FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}