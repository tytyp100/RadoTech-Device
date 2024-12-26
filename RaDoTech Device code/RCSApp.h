#ifndef RCSAPP_H
#define RCSAPP_H

#include "user.h"
#include "mainwindow.h"
#include <string>
#include <vector>
#include<QDebug>
using namespace std;

//may need to add forward class declaration for the mainWindow class
class MainWindow;

class RCSApp
{
public:
    RCSApp();
    RCSApp(MainWindow*);
    ~RCSApp();
    void setMainWindow(MainWindow*);
    void loadUsersFromDatabase();
    void doMeasurement(float);
    void endMeasuring();
    void clearRawData();
    int getBattery();

    void saveRawDataToDataBase(int userID, const std::vector<float>& dataValues, float bodyTemp, float bloodPressureRight, float bloodPressureLeft, float heartRate, float weight, const QString& sleepingTime, const QString& face, const QString& ranking, const QString& notes);
    void confirmClicked(int loggedInUserId,float bodyTemp,float bPressureRight,float bPressureLeft,float heartRate,QString sleepTime,float weight,QString emotion,QString feeling,QString notes);
private:
    vector<User*> users;
    vector<float> rawData;
    MainWindow* mainWindow;
    void passRawData(float);

};
#endif
