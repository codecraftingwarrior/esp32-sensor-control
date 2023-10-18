#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>

class NetworkManager {
  public:
    NetworkManager();
    static void connectToWiFi();
};

#endif
