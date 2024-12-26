#include "formwindow.h"
#include "ui_formwindow.h"
#include "RCSApp.h"
formWindow::formWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formWindow)
{
    ui->setupUi(this);
}

formWindow::~formWindow()
{
    delete ui;
}

void formWindow::on_btnConfirm_clicked()
{
    float bodyTemp = ui->txtBodyTemp->text().toFloat();
    float bPressureRight = ui->txtBloodRight->text().toFloat();
    float bPressureLeft = ui->txtBloodLeft->text().toFloat();
    float heartRate = ui->txtHeartRate->text().toFloat();
    QString sleepTime = ui->txtSleep->text();
    float weight = ui->txtWeight->text().toFloat();
    QString emotion = ui->cBoxEmotion->currentText();
    QString feeling = ui->cBoxFeeling->currentText();
    QString notes = ui->txtNotes->text();
    app->confirmClicked(loggedInUserId,bodyTemp,bPressureRight,bPressureLeft,heartRate,sleepTime,weight,emotion,feeling,notes);
    this->close();


}

