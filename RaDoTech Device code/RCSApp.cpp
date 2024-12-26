#include "RCSApp.h"

RCSApp::RCSApp(){}

RCSApp::RCSApp(MainWindow* window): mainWindow(window){
}


RCSApp::~RCSApp(){
    for(User* user: users){
        delete user;
    }
}

void RCSApp::doMeasurement(float f){
    int id = mainWindow->getUserId()-1;
    RadoDevice* device = users[id]->getDevice();
    device->startDepletion();
    mainWindow->displayImg();
    float data = users[id]->scanBodyPart(f);
    rawData.push_back(data);
    passRawData(data);
}

void RCSApp::passRawData(float f){
    //TODO: Give rawData to mainWindow
    mainWindow->displayRawDataGraph(f);
    mainWindow->showRawData(f);
}

void RCSApp::endMeasuring(){
    int id = mainWindow->getUserId()-1;
    users[id]->getDevice()->stopDepletion();
    mainWindow->fillForm();
}

void RCSApp::confirmClicked(int loggedInUserId,float bodyTemp,float bPressureRight,float bPressureLeft,float heartRate,QString sleepTime,float weight,QString emotion,QString feeling,QString notes){
    if(loggedInUserId == -1){
        return;
    }
    mainWindow->displayHealthGraph(rawData);
    saveRawDataToDataBase(loggedInUserId,rawData,bodyTemp,bPressureRight,bPressureLeft,heartRate,weight,sleepTime,emotion,feeling,notes);
    mainWindow->displayHealthGraph(rawData);
}

int RCSApp::getBattery(){
    int id = mainWindow->getUserId()-1;
    RadoDevice* device = users[id]->getDevice();
    return device->getBattery();
}

void RCSApp::loadUsersFromDatabase(){
    QSqlQuery query;
    query.prepare("SELECT userID, username, password, fName, lName, gender, weight, height FROM User");
    if(!query.exec()){
        qDebug() << "Failed to execute load user query" << query.lastError().text();
        return;
    }
    for(User* user: users){
        delete user;
    }
    users.clear();

    while (query.next()){
        int userID = query.value("userID").toInt();
        QString username = query.value("username").toString();
        QString password = query.value("password").toString();
        QString firstName = query.value("fName").toString();
        QString lastName = query.value("lName").toString();
        QString gender = query.value("gender").toString();
        QString weight = query.value("weight").toString();
        QString height = query.value("height").toString();

        User* user = new User(username.toStdString(),
                  password.toStdString(),
                  firstName.toStdString(),
                  lastName.toStdString(),
                  gender.toStdString(),
                  weight.toStdString(),
                  height.toStdString(),
                  userID);
        users.push_back(user);
    }
    qDebug() << "Loaded " << users.size() << " users from the database";
}

void RCSApp::setMainWindow(MainWindow* window){
    mainWindow = window;
}

void RCSApp::clearRawData(){
    rawData.clear();
}

void RCSApp::saveRawDataToDataBase(int userID, const std::vector<float>& dataValues,
                                   float bodyTemp, float bloodPressureRight, float bloodPressureLeft,
                                   float heartRate, float weight,
                                   const QString& sleepingTime, const QString& face,
                                   const QString& ranking, const QString& notes) {
  if (dataValues.size() != 24) {
      qDebug() << "Error: dataValues vector must have exactly 24 elements."
               << "size: " << dataValues.size();
      return;
  }

  // Get current date and time
  std::time_t t = std::time(nullptr);
  std::tm* tm = std::localtime(&t);
  char dateBuffer[11];  // Format: YYYY-MM-DD
  char timeBuffer[6];   // Format: HH:MM
  std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", tm);
  std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", tm);

  QString date = QString::fromUtf8(dateBuffer);
  QString time = QString::fromUtf8(timeBuffer);

  QSqlQuery query;
  query.prepare(R"(
      INSERT INTO Data (
          userID, date, time,
          X1, X2, X3, X4, X5, X6, X7, X8, X9, X10,
          X11, X12, X13, X14, X15, X16, X17, X18, X19, X20,
          X21, X22, X23, X24,
          body_temp, blood_pressure_right_hand, blood_pressure_left_hand,
          heart_rate, weight, sleeping_time, face, ranking, notes
      ) VALUES (
          :userID, :date, :time,
          :X1, :X2, :X3, :X4, :X5, :X6, :X7, :X8, :X9, :X10,
          :X11, :X12, :X13, :X14, :X15, :X16, :X17, :X18, :X19, :X20,
          :X21, :X22, :X23, :X24,
          :bodyTemp, :bloodPressureRight, :bloodPressureLeft,
          :heartRate, :weight, :sleepingTime, :face, :ranking, :notes
      )
  )");

  query.bindValue(":userID", userID);
  query.bindValue(":date", date);
  query.bindValue(":time", time);

  for (int i = 0; i < 24; ++i) {
      query.bindValue(QString(":X%1").arg(i + 1), dataValues[i]);
  }

  query.bindValue(":bodyTemp", bodyTemp);
  query.bindValue(":bloodPressureRight", bloodPressureRight);
  query.bindValue(":bloodPressureLeft", bloodPressureLeft);
  query.bindValue(":heartRate", heartRate);
  query.bindValue(":weight", weight);
  query.bindValue(":sleepingTime", sleepingTime);
  query.bindValue(":face", face);
  query.bindValue(":ranking", ranking);
  query.bindValue(":notes", notes);

  if (!query.exec()) {
      qDebug() << "Error inserting into Data table3: " << query.lastError().text();
  } else {
      qDebug() << "Successfully inserted into Data table.";
  }
}




