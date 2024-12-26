#include "sensor.h"

Sensor::Sensor(){
    touchingSkin = false;
}

Sensor::~Sensor(){}

float Sensor::getMeasurement(float randomTest){
    random_device rd;
    mt19937 gen(rd());
    float randomNum = -1;
    if (randomTest < 6.0){
        //qInfo() << "Healthy measurements";
        uniform_real_distribution<>dis(5.0,50.0);
        randomNum = dis(gen);
        randomNum = round(randomNum*10)/10;
    }else if (randomTest < 9.0){
        //qInfo() << "Normal measurements";
        uniform_real_distribution<>dis(50.0,70.0);
        randomNum = dis(gen);
        randomNum = round(randomNum*10)/10;
    }else{
        //qInfo() << "Unhealthy measurements";
        uniform_real_distribution<>dis(70.0,180.0);
        randomNum = dis(gen);
        randomNum = round(randomNum*10)/10;
    }
    return randomNum;
}

void Sensor::setTouchingSkin(QString str){
    if (str == "Touching Skin (ON)"){
        qInfo() << "TOUCHING SKIN TRUE";
        touchingSkin = true;
    }else{
        touchingSkin = false;
    }
}


