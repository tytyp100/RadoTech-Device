#include "mainwindow.h"
#include "RCSApp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* mainWindow = nullptr;
    RCSApp* app = nullptr;
    app = new RCSApp();
    mainWindow = new MainWindow(app);
    app->setMainWindow(mainWindow);

    //MainWindow w;
    mainWindow->show();
    mainWindow->setupDatabase();
    mainWindow->createTables();
    app->loadUsersFromDatabase();
    return a.exec();

    delete mainWindow;
    delete app;
}
