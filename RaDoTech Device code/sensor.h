#ifndef SENSOR_H
#define SENSOR_H

#include<QDebug>
#include<random>
using namespace std;

class Sensor
{
public:
    Sensor();
    ~Sensor();
    float getMeasurement(float);
    void setTouchingSkin(QString);
private:
    bool touchingSkin;

};
#endif
