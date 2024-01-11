#include <Arduino.h>
#include "data_type.h"

const String Sensor::SENSOR_BRIGHTNESS_TYPE = "brightness";
const String Sensor::SENSOR_TEMPERATURE_TYPE = "temperature";

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
  const float VCC = 3.3;      // Tension d'alimentation
  const int ADC_MAX = 4095;   // Maximum de la lecture ADC
  const float R_REF = 10000;  // Résistance de référence en ohms
  const float BETA = 3950;    // Valeur Bêta de la thermistance
  const float R0 = 10000;     // Résistance de la thermistance à 25°C
  const float T0 = 298.15;
  if (this->type.equals("temperature")) {

    int analogValue = analogRead(this->getPinId());

    float R_thermistor = R_REF * (float(analogValue) / (ADC_MAX - analogValue));

    // Calcul de la température en Kelvin
    float temperatureK = BETA / (log(R_thermistor / R0) + (BETA / T0));

    // Convertir la température en Celsius
    float temperatureC = temperatureK - 273.15;

    this->currentValue = temperatureC;
  } else if (this->type.equals("brightness")) {

    int analogValue = analogRead(this->pinId);

    this->currentValue = analogValue;
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

void Sensor::setThreshold(float newValue) {
  this->threshold = newValue;
}

Sensor Sensor::createSensor(SensorType type, int pinId, String name, float threshold) {
  Sensor sensor;
  sensor.setPinId(pinId);
  sensor.setName(name);
  sensor.setIsOn(true);
  sensor.setCurrentValue(0);
  sensor.setThreshold(threshold);

  if (type == Sensor::SensorType::BRIGHTNESS_SENSOR)
    sensor.setType(Sensor::SENSOR_BRIGHTNESS_TYPE);
  else if (type == Sensor::SensorType::TEMPERATURE_SENSOR)
    sensor.setType(Sensor::SENSOR_TEMPERATURE_TYPE);

  pinMode(pinId, INPUT);
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

void LED::blink(int delayVal) {
  this->switchOn();
  delay(delayVal);
  this->switchOff();
  delay(delayVal);
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
  Serial.printf("Toggling LED on pin %d\n", this->getPinId());
  if (this->isOn) {
    digitalWrite(this->pinId, LOW);
  } else {
    digitalWrite(this->pinId, HIGH);
  }
  this->isOn = !this->isOn;
}

void LED::switchOff() {
  Serial.printf("Switching OFF LED on pin %d.\n", this->getPinId());
  this->isOn = false;
  digitalWrite(this->pinId, LOW);
}

void LED::switchOn() {
  Serial.printf("Switching ON LED on pin %d.\n", this->getPinId());
  this->isOn = true;
  digitalWrite(this->pinId, HIGH);
}
