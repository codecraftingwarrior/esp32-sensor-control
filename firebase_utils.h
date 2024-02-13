#include <Firebase_ESP_Client.h>

#ifndef FIREBASE_UTILS
#define FIREBASE_UTILS

#define API_KEY "AIzaSyBQAOtsrnZlUQkhLdbQMzKn2aD26Rj7ASg"
#define DATABASE_URL "https://esp32-sensor-6ec3b-default-rtdb.europe-west1.firebasedatabase.app/"

/*
  Classe utilitaire contenant la logique métier d'interaction avec la base de données firebase.
*/
class FirebaseUtils {
  public:
    FirebaseUtils();
    void init();
    void storeSensorData(float light, float temperature);
    void storeSystemUsage(int cpuUsage, int ramUsage);
};


#endif