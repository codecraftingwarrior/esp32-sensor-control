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

#endif
