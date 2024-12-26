#include "radoDevice.h"

RadoDevice::RadoDevice(QObject* parent): QObject(parent), battery(100){
    //moveToThread(QApplication::instance()->thread());
    //qInfo() << "RadoDevice created with batrtery: " << battery;
    batteryTimer = new QTimer(this);
    connect(batteryTimer, &QTimer::timeout, this, &RadoDevice::decreaseBattery);
    Sensor sensor;
}

RadoDevice::~RadoDevice(){
    //qInfo() << "RadoDevice destroyed";
}

float RadoDevice::scan(float f){
   return sensor.getMeasurement(f);
}

void RadoDevice::startDepletion(){
    if(!batteryTimer->isActive()){
        batteryTimer->start(1350); //depletes battery every 1 second;
        //batteryTimer->start(100);  <== UNCOMMENT IF YOU WANT TO TEST BATTERY DEPLETION AND GRACEFUL SHUTDOWN
        qDebug() << "Battery depletion started!";
    }
}

void RadoDevice::stopDepletion(){
    batteryTimer->stop();
}

void RadoDevice::decreaseBattery(){
    if (battery > 0){
        battery--;
        emit batteryChanged(battery);
    }
    if( battery ==0){
        qInfo() << "Battery is 0. Gracefully shutting down.";
        stopDepletion();
    }
}

int RadoDevice::getBattery(){
    return battery;
}

