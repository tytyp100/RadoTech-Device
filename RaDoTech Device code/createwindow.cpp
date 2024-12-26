#include "createwindow.h"
#include "ui_createwindow.h"

createWindow::createWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createWindow)
{
    ui->setupUi(this);
}

createWindow::~createWindow()
{
    delete ui;
}

void createWindow::on_btnConfirm_clicked()
{
    QSqlQuery query;
    QString username = ui->txtUsername->text();
    QString password = ui->txtPassword->text();
    QString fName = ui->txtFirstName->text();
    QString lName = ui->txtLastName->text();
    QString gender = ui->txtGender->currentText();
    QString weight = ui->txtWeight->text();
    QString height = ui->txtHeight->text();

    if(ui->txtCPassword->text() != ui->txtPassword->text()){
        qInfo() << "Passwords do not match. Try again";
        return;
    }

    query.prepare(R"(INSERT INTO User (username, password, fName, lName,gender,weight,height)
                  VALUES (:username,:password, :fName, :lName, :gender, :weight, :height)
    )");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":fName", fName);
    query.bindValue(":lName", lName);
    query.bindValue(":gender", gender);
    query.bindValue(":weight", weight);
    query.bindValue(":height", height);

    if (!query.exec()){
        qDebug() << "Creating query failed" << query.lastError().text();
        return;
    }
    qInfo() << "Profile created successufully";
    this->close();
}

