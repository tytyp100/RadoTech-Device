#ifndef RADODEVICE_H
#define RADODEVICE_H

#include "sensor.h"
#include <QTimer>
#include <QObject>
#include <QDebug>
#include <QApplication>

class RadoDevice:public QObject
{
    Q_OBJECT
public:
    explicit RadoDevice(QObject* parent = nullptr);
    ~RadoDevice();
    float scan(float);
    void startDepletion();
    void stopDepletion();
    int getBattery();

signals:
    void batteryChanged(int battery);

private:
    int battery;
    Sensor sensor;
    QTimer* batteryTimer;
    void decreaseBattery();

};
#endif
