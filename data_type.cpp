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

float Sensor::getThreshold() {
  return this->threshold;
}

float Sensor::setThreshold(float newValue) {
  this->threshold = newValue;
}

Sensor Sensor::createSensor(SensorType type, int pinId, String name, float threshold) {
  Sensor sensor;
  sensor.setPinId(pinId);
  sensor.setName(name);
  sensor.setIsOn(true);
  sensor.setCurrentValue(0);
  sensor.setThreshold(threshold);

  if(type == Sensor::SensorType::BRIGHTNESS_SENSOR)
    sensor.setType("brightness");
  else if (type == Sensor::SensorType::TEMPERATURE_SENSOR)
    sensor.setType("temperature");

  return sensor;
}


LED::LED() {}

int LED::getPinId() const {
  return this->pinId;
}

void LED::setPinId(int pin) {
  this->pinId = pin;
}

boolean LED::getIsOn() const {
  return this->isOn;
}

void LED::setIsOn(boolean on) {
  this->isOn = on;
}

String LED::getColor() const {
  return this->color;
}

void LED::setColor(String color) {
  this->color = color;
}

String LED::getName() const {
  return this->name;
}

void LED::setName(String name) {
  this->name = name;
}

LED LED::createLED(int pinId, boolean isOn, String color, String name) {
  LED led;
  led.setPinId(pinId);
  led.setIsOn(isOn);
  led.setColor(color);
  led.setName(name);
  
  pinMode(pinId, OUTPUT);
  return led;
}

void LED::toggle() {
  if(this->isOn) {
    digitalWrite(this->pinId, LOW);
  } else {
    digitalWrite(this->pinId, HIGH);
  }
  this->isOn = !this->isOn;
}

void LED::switchOff() {
  this->isOn = false;
  digitalWrite(this->pinId, LOW);
}

void LED::switchOn() {
  this->isOn = true;
  digitalWrite(this->pinId, HIGH);
}
