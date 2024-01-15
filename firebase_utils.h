#include <Firebase_ESP_Client.h>

#ifndef FIREBASE_UTILS
#define FIREBASE_UTILS

#define API_KEY "AIzaSyBQAOtsrnZlUQkhLdbQMzKn2aD26Rj7ASg"
#define DATABASE_URL "https://esp32-sensor-6ec3b-default-rtdb.europe-west1.firebasedatabase.app/"

class FirebaseUtils {
  public:
    FirebaseUtils();
    void init();
    void storeSensorData(float light, float temperature);
};


#endif