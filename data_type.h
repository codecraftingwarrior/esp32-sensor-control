#include <Arduino.h>

#ifndef DATA_TYPE_H
#define DATA_TYPE_H

class Sensor { 
  private:
    int pinId;
    float threshold;
    String name;
    String type;
    boolean isOn;
    float currentValue;
    
  public:
    Sensor();
    enum SensorType { BRIGHTNESS_SENSOR, TEMPERATURE_SENSOR };
    int getPinId() const;
    void setPinId(int);
    String getName() const;
    void setName(String);
    String getType() const;
    void setType(String);
    boolean getIsOn() const;
    void setIsOn(boolean);
    float getCurrentValue();
    void setCurrentValue(float);
    float getThreshold();
    float setThreshold(float);
    static Sensor createSensor(SensorType type, int pinId, String name, float threshold);
};

class LED {
  private:
    int pinId;
    boolean isOn;
    String color;
    String name;

  public:
    LED();
    int getPinId() const;
    void setPinId(int);
    boolean getIsOn() const;
    void setIsOn(boolean);
    String getColor() const;
    void setColor(String);
    String getName() const;
    void setName(String);
    static LED createLED(int pinId, boolean isOn, String color, String name);
    void toggle();
    void switchOff();
    void switchOn();
};

#endif
