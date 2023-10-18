#include <Arduino.h>
#include "data_type.h"

Sensor::Sensor() {}

int Sensor::getPinId() const {
  return this->pinId;
}

void Sensor::setPinId(int id) {
  this->pinId = id;
}

String Sensor::getName() const {
  return this->name;
}

void Sensor::setName(String name) {
  this->name = name;
}

String Sensor::getType() const {
  return this->type;
}

void Sensor::setType(String type) {
  this->type = type;
}

boolean Sensor::getIsOn() const {
  return this->isOn;
}

void Sensor::setIsOn(boolean state) {
  this->isOn = state;
}

float Sensor::getCurrentValue() {
  const float BETA = 3950; 
  if (this->type.equals("temperature")) {

    int analogValue = analogRead(this->pinId);
    float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

    this->currentValue = celsius;
  } else if (this->type.equals("brightness")) {
    const float GAMMA = 0.7;
    const float RL10 = 50;

    int analogValue = analogRead(A0);
    float voltage = analogValue / 1024. * 5;
    float resistance = 2000 * voltage / (1 - voltage / 5);
    float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

    this->currentValue = lux;
  } else {
    this->currentValue = 0;
  }

  return this->currentValue;
}

void Sensor::setCurrentValue(float value) {
  this->currentValue = value;
}
