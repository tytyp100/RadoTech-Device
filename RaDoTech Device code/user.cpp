#include "user.h"

User::User(){
}

User::User(string n, string p, string f, string l, string g, string w, string h, int i){
    username = n;
    password = p;
    fName = f;
    lName =l;
    gender = g;
    weight = w;
    height = h;
    userId = i;
    radoDevice = new RadoDevice();
    //qInfo() << "User created with id: " << userId;
    //qInfo() << "USER RADODEVICE BATT: " <<radoDevice->getBattery();
    vector<ScanData> scanData;
}

User::~User(){
    delete radoDevice;
}

float User::scanBodyPart(float f){
    return radoDevice->scan(f);
}

void User::insertData(vector<float> data){
    scanData.push_back(ScanData(data));
}

RadoDevice* User::getDevice(){
    return radoDevice;
}




