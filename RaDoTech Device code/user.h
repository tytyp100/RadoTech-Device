#ifndef USER_H
#define USER_H

#include "radoDevice.h"
#include "scanData.h"
#include <string>
#include <vector>
#include<QDebug>
using namespace std;

class User
{
public:
    User();
    User(string, string, string, string, string, string, string, int);
    ~User();
    float scanBodyPart(float);
    void insertData(vector<float> data);
    RadoDevice* getDevice();

private:
    RadoDevice* radoDevice;
    vector<ScanData> scanData;
    int userId;
    string username;
    string password;
    string fName;
    string lName;
    string gender;
    string weight;
    string height;
};
#endif
