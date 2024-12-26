#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <createwindow.h>
#include <formwindow.h>
#include "RCSApp.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QLabel>
#include <QPixmap>
#include <QDir>
#include <QtGui>
#include <QtCore>
#include <QtCharts>
#include <unistd.h>
#include <random>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QVBoxLayout>
#include <vector>
#include <QStringList>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//may need to add forward class declaration for the RCSApp class
class RCSApp;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(RCSApp* app = nullptr,QWidget *parent = nullptr);
    ~MainWindow();
    void fillForm();
    void displayRawDataGraph(float);
    int getUserId();
    void setupDatabase();
    void createTables();
    void displayImg();
    void showRawData(float);
    void displayHealthGraph(const vector<float> &data);

    void updatePreviousMenu();

private slots:
    void on_btnMeasure_clicked();
    void on_btnLogin_clicked();
    void on_btnCreate_clicked();
    void updateBattery(int);
    void on_btnDelete_clicked();
    void on_btnDisplayHistoryData_clicked();

    void on_btnRecommendation_clicked();

private:
    Ui::MainWindow *ui;
    createWindow *cWindow;
    formWindow *fWindow;
    RCSApp* app;
    int loggedInUserId;
    QLabel *batteryLabel;
    QProgressBar *progressBar;
    int currentIteration;
    float measurementType;
    QComboBox* box_select;

    void login();
    void createProfile();
    void displayHistory();
    void measureIteration();
    void hideImages();
    void showImages();
    void clearGraphs();

};
#endif // MAINWINDOW_H
