#ifndef FORMWINDOW_H
#define FORMWINDOW_H

#include <QDialog>
#include <QDebug>

class RCSApp;
namespace Ui {
class formWindow;
}

class formWindow : public QDialog
{
    Q_OBJECT

public:
    explicit formWindow(QWidget *parent = nullptr);
    ~formWindow();


    inline void setLoggedInUserId(int userId) { loggedInUserId = userId; }
    inline void setApp(RCSApp* application) { app = application; }

private slots:
    void on_btnConfirm_clicked();

private:
    Ui::formWindow *ui;

    int loggedInUserId;
    RCSApp* app;

};

#endif // FORMWINDOW_H
