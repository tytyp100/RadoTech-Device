#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class createWindow;
}

class createWindow : public QDialog
{
    Q_OBJECT

public:
    explicit createWindow(QWidget *parent = nullptr);
    ~createWindow();

private slots:
    void on_btnConfirm_clicked();

private:
    Ui::createWindow *ui;
};

#endif // CREATEWINDOW_H
