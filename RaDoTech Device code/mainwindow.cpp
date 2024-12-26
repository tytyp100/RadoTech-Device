#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cWindow(nullptr)
    , fWindow(nullptr)
{
    ui->setupUi(this);
    loggedInUserId = -1;
    batteryLabel = ui->txtBatteryLife;
    progressBar = ui->batteryProgressBar;
    box_select = ui->cBoxHistory;
    updateBattery(100);

}

MainWindow::MainWindow(RCSApp* a,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cWindow(nullptr)
    , fWindow(nullptr)
    ,app(a)
{
    ui->setupUi(this);
    hideImages();
    loggedInUserId = -1;

    batteryLabel = ui->txtBatteryLife;
    progressBar = ui->batteryProgressBar;
    box_select = ui->cBoxHistory;
    updateBattery(100);

    //batteryLabel->setText("Battery Life: 100");

}

MainWindow::~MainWindow()
{
    delete ui;
    delete app;
}

void MainWindow::updateBattery(int b) {
    progressBar->setValue(b);
    if (b == 10) {
        QList<QTimer *> timers = this->findChildren<QTimer *>();
        QList<std::pair<QTimer *, int>> activeTimers;

        for (QTimer *timer : timers) {
            if (timer->isActive()) {
                activeTimers.append({timer, timer->interval()});
                timer->stop();
            }
        }
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Battery Low");
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        QVBoxLayout *layout = new QVBoxLayout(dialog);
        QLabel *label = new QLabel("Battery is low, program will shut down when the battery hits 0.");
        label->setWordWrap(true);
        layout->addWidget(label);
        dialog->setLayout(layout);

        dialog->show();
        QTimer::singleShot(2000, dialog, &QDialog::close);
        usleep(2000);
        connect(dialog, &QDialog::destroyed, this, [activeTimers]() {
            for (const auto &pair : activeTimers) {
                pair.first->start(pair.second);
            }
        });
    }
    if (b == 0) {
        QList<QTimer *> timers = this->findChildren<QTimer *>();
        for (QTimer *timer : timers) {
            if (timer->isActive()) {
                timer->stop();
            }
        }
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setWindowTitle("Battery Depleted");
        msgBox->setText("No more battery. Gracefully shutting down. All data will be saved.");
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setStandardButtons(QMessageBox::NoButton);
        msgBox->setAttribute(Qt::WA_DeleteOnClose);

        msgBox->show();
        QTimer::singleShot(1500, this, [this, msgBox]() {
            msgBox->close();
            this->close();
            QCoreApplication::quit();
        });
    }
}



void MainWindow::fillForm(){
    //TODO: Show form to fill out
    if(!fWindow){
        fWindow = new formWindow(this);
    }
    fWindow->setApp(app);
    fWindow->setLoggedInUserId(loggedInUserId);
    fWindow->exec();
    fWindow->deleteLater();
    fWindow = nullptr;
}


void MainWindow::displayRawDataGraph(float f){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<>rand(0.0,20.0);
    float randomNum = -1;

    QLineSeries *rawDataGraph = new QLineSeries();
    for (int i=0; i < 9; ++i){
        randomNum = rand(gen);
        if(f-10+randomNum < 0){
            randomNum = 10;
        }
        rawDataGraph->append(i,f-10+randomNum);
    }
    rawDataGraph->append(10,f);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(rawDataGraph);
    chart->createDefaultAxes();

    chart->axes(Qt::Vertical).first()->setRange(0,180);
    chart->axes(Qt::Horizontal).first()->setRange(0,10);
    chart->setTitle("Raw Data Graph");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *container = ui->rawGraphContainer;
    if (container->layout()) {
        QLayout *oldLayout = container->layout();
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (QWidget *widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete oldLayout;
    }
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->addWidget(chartView);
    container->setLayout(layout);
}

void MainWindow::showRawData(float f){
    ui->txtRawData->setText(QString::number(f));
}

void MainWindow::displayHealthGraph(const vector<float> &data) {
    // Ensure the data vector has 24 values
    if (data.size() != 24) {
        qWarning() << "Data vector must have exactly 24 values.";
        return;
    }

    // Aggregate data for each body part
    QBarSet *barSet = new QBarSet("Measurements");
    for (int i = 0; i < 12; ++i) {
        float value = (data[i] + data[i + 12]) / 2.0;  // Average the two values
        *barSet << value;
    }

    // Create the bar series and chart
    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    QChart *chart = new QChart();
    chart->addSeries(barSeries);
    chart->setTitle("Body Part Measurements");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Configure the y-axis (range 5–180)
    QValueAxis *yAxis = new QValueAxis();
    yAxis->setRange(5, 180);
    yAxis->setTitleText("Measurement");
    chart->addAxis(yAxis, Qt::AlignLeft);
    barSeries->attachAxis(yAxis);

    // Configure the x-axis with text labels
    QCategoryAxis *xAxis = new QCategoryAxis();
    QStringList bodyPartLabels = {
        "Lungs", "Pericardium", "Heart", "Small Intestine",
        "Lymph Vessel", "Large Intestine", "Spleen", "Liver",
        "Kidney/Adrenal", "Bladder", "Gallbladder", "Stomach"
    };

    for (int i = 0; i < bodyPartLabels.size(); ++i) {
        //xAxis->append(bodyPartLabels[i], i + 0.5);  // Align labels at the center of each bar
    }

    xAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    chart->addAxis(xAxis, Qt::AlignBottom);
    barSeries->attachAxis(xAxis);

    // Create the chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Create a container layout
    QWidget *container = ui->healthGraphContainer;
    if (container->layout()) {
        QLayout *oldLayout = container->layout();
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (QWidget *widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }
        delete oldLayout;
    }
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->addWidget(chartView);
    container->setLayout(layout);

    showImages();
    ui->btnRecommendation->setEnabled(true);

}

void MainWindow::displayHistory(){
    qWarning() << "Image not found: " << "imagePath";
}

void MainWindow::on_btnDisplayHistoryData_clicked(){
    QString selectedText = box_select->currentText();
    QRegularExpression regex("ID: (\\d+)$");
    QRegularExpressionMatch match = regex.match(selectedText);
    if (!match.hasMatch()) {
        qWarning() << "Invalid selection: Unable to extract ID.";
        return;
    }
    int dataID = match.captured(1).toInt();
    qDebug() << "Selected ID:" << dataID;
    QSqlQuery query;
    query.prepare("SELECT X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, "
                  "X11, X12, X13, X14, X15, X16, X17, X18, X19, X20, "
                  "X21, X22, X23, X24, "
                  "body_temp, blood_pressure_right_hand, blood_pressure_left_hand, "
                  "heart_rate, weight, sleeping_time, face, ranking, notes "
                  "FROM Data WHERE dataID = :dataID");
    query.bindValue(":dataID", dataID);

    if (!query.exec()) {
        qWarning() << "Error retrieving data from Data table:" << query.lastError().text();
        return;
    }
    std::vector<float> rawData;
    float bodyTemp, bloodPressureRight, bloodPressureLeft, heartRate, weight;
    QString sleepingTime, emotion, ranking, notes;

    if (query.next()) {
        rawData.clear();
        for (int i = 0; i < 24; ++i) {
            rawData.push_back(query.value(i).toFloat());
        }
        bodyTemp = query.value(24).toFloat();
        bloodPressureRight = query.value(25).toFloat();
        bloodPressureLeft = query.value(26).toFloat();
        heartRate = query.value(27).toFloat();
        weight = query.value(28).toFloat();
        sleepingTime = query.value(29).toString();
        emotion = query.value(30).toString();
        ranking = query.value(31).toString();
        notes = query.value(32).toString();

        displayHealthGraph(rawData);
        ui->btnRecommendation->setEnabled(true);

    } else {
        qWarning() << "No data found for the given ID:" << dataID;
    }
}


void MainWindow::displayImg(){
    QString imagePath = ":/photos/radotechScanningPhotos/point" + QString::number(currentIteration + 1) + ".png";
    QPixmap img(imagePath);
    if (!img.isNull()) {
        ui->imgBodyPart->setVisible(true);
        ui->imgBodyPart->setPixmap(img);
    } else {
        qWarning() << "Image not found: " << imagePath;
    }
}

void MainWindow::showImages(){
    ui->imgBladder->setVisible(true);
    ui->imgGallbladder->setVisible(true);
    ui->imgHeart->setVisible(true);
    ui->imgKidney->setVisible(true);
    ui->imgLIntestine->setVisible(true);
    ui->imgLiver->setVisible(true);
    ui->imgLungs->setVisible(true);
    ui->imgLymph->setVisible(true);
    ui->imgPericardium->setVisible(true);
    ui->imgSIntestine->setVisible(true);
    ui->imgSpleen->setVisible(true);
    ui->imgStomach->setVisible(true);

}

void MainWindow::hideImages(){
    ui->imgBladder->setVisible(false);
    ui->imgGallbladder->setVisible(false);
    ui->imgHeart->setVisible(false);
    ui->imgKidney->setVisible(false);
    ui->imgLIntestine->setVisible(false);
    ui->imgLiver->setVisible(false);
    ui->imgLungs->setVisible(false);
    ui->imgLymph->setVisible(false);
    ui->imgPericardium->setVisible(false);
    ui->imgSIntestine->setVisible(false);
    ui->imgSpleen->setVisible(false);
    ui->imgStomach->setVisible(false);
}

void MainWindow::on_btnMeasure_clicked()
{
    if(loggedInUserId == -1){
        QMessageBox::warning(this, "Error", "Must be signed in to measure.");
        return;
    }
    //app->startMeasuring();
    app->clearRawData();
    if(ui->cBoxTouchingSkin->currentText() != "Touching Skin (ON)"){
        qInfo() << "Unable to perform measurements because skin is not in contact with the device";
        return;
    }
    ui->cBoxTouchingSkin->setEnabled(false);
    currentIteration =0;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<>test(0.0,10.0);
    measurementType = test(gen);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::measureIteration);
    timer->start(1000); // 100ms delay between each iteration
}

void MainWindow::measureIteration(){
    updateBattery(app->getBattery());
    if (currentIteration < 24) {
        app->doMeasurement(measurementType);
        currentIteration++;
    } else{
          ui->imgBodyPart->setVisible(false);
        sender()->deleteLater();
        updatePreviousMenu();
        qInfo() << "Measurement complete!";

    }
    if (currentIteration == 24){
          ui->imgBodyPart->setVisible(false);
        app->endMeasuring();
        ui->cBoxTouchingSkin->setEnabled(true);
        currentIteration++;
          ui->imgBodyPart->setVisible(false);

    }
}

int MainWindow::getUserId(){
    return loggedInUserId;
}

void MainWindow::on_btnLogin_clicked()
{
    if(ui->btnLogin->text() == "Login"){
    QString username = ui->txtUsername->text();
    QString password = ui->txtPassword->text();

    QSqlQuery query;
    query.prepare("SELECT userID FROM User WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (!query.exec()){
        qDebug() << "Login query failed";
        return;
    }
    if (query.next()){
        loggedInUserId = query.value(0).toInt();
        qInfo() << "Login successful!";
        qInfo() << "User id: " << loggedInUserId;
        ui->txtUsername->setEnabled(false);
        ui->txtPassword->setEnabled(false);
        ui->btnLogin->setEnabled(false);
        ui->btnCreate->setEnabled(false);

        ui->btnLogin->setText("Logout");
        ui->btnLogin->setEnabled(true);
        updatePreviousMenu();
        app->loadUsersFromDatabase();
        return;
    }
    qInfo() << "Failed to login";
    }else{
        ui->btnLogin->setText("Login");
        ui->btnLogin->setEnabled(true);
        ui->txtUsername->setEnabled(true);
        ui->txtPassword->setEnabled(true);
        ui->txtUsername->setText("");
        ui->txtPassword->setText("");
        ui->btnCreate->setEnabled(true);
        clearGraphs();
        hideImages();
        ui->btnRecommendation->setEnabled(false);
        loggedInUserId = -1;
        updatePreviousMenu();
        app->loadUsersFromDatabase();
    }
}

void MainWindow::clearGraphs(){
    ui->imgBodyPart->clear();
    QLayout *lineGraphLayout = ui->rawGraphContainer->layout();
    if (lineGraphLayout) {
        QLayoutItem *item;
        while ((item = lineGraphLayout->takeAt(0)) != nullptr) {
            delete item->widget(); // Deletes the child widget
            delete item;           // Deletes the layout item
        }
    }
    QLayout *barGraphLayout = ui->healthGraphContainer->layout();
    if (barGraphLayout) {
        QLayoutItem *item;
        while ((item = barGraphLayout->takeAt(0)) != nullptr) {
            delete item->widget(); // Deletes the child widget
            delete item;           // Deletes the layout item
        }
    }
}

void MainWindow::on_btnCreate_clicked()
{
    //TODO: Add username and password to the database
    if(!cWindow){
        cWindow = new createWindow(this);
    }
    cWindow->exec();
    cWindow->deleteLater();
    cWindow = nullptr;
}

void MainWindow::setupDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("radoDevice.db");

    if (!db.open()){
        qDebug() << "Error: unable to connect to database" << db.lastError().text();
        return;
    }
    qDebug() <<"Database connected!";
}

void MainWindow::createTables(){
    QSqlQuery query;
    QString createUserTable = R"(
        CREATE TABLE IF NOT EXISTS User(
        userID INTEGER PRIMARY KEY,
        username TEXT NOT NULL,
        password TEXT NOT NULL,
        fName TEXT NOT NULL,
        lName TEXT NOT NULL,
        gender TEXT NOT NULL,
        weight TEXT NOT NULL,
        height TEXT NOT NULL
        );
    )";

    QString createDataTable = R"(

    CREATE TABLE IF NOT EXISTS Data(
        dataID INTEGER PRIMARY KEY,
        userID INTEGER,
        date TEXT,
        time TEXT,
        X1 REAL,
        X2 REAL,
        X3 REAL,
        X4 REAL,
        X5 REAL,
        X6 REAL,
        X7 REAL,
        X8 REAL,
        X9 REAL,
        X10 REAL,
        X11 REAL,
        X12 REAL,
        X13 REAL,
        X14 REAL,
        X15 REAL,
        X16 REAL,
        X17 REAL,
        X18 REAL,
        X19 REAL,
        X20 REAL,
        X21 REAL,
        X22 REAL,
        X23 REAL,
        X24 REAL,
        body_temp REAL,
        blood_pressure_right_hand REAL,
        blood_pressure_left_hand REAL,
        heart_rate REAL,
        weight REAL,
        sleeping_time TEXT,
        face TEXT,
        ranking TEXT,
        notes TEXT,
        FOREIGN KEY (userID) REFERENCES User (userID) ON DELETE CASCADE
    );
    )";
    // datetime format ex: 2024-11-30          05:24

    if (!query.exec(createUserTable)){
        qDebug() << "Error creating User table: " << query.lastError().text();
    }
    if (!query.exec(createDataTable)){
        qDebug() << "Error creating Data table: " << query.lastError().text();
    }
}

void MainWindow::on_btnDelete_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Account",
                                  "Are you sure you want to delete your account? This action cannot be undone.",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }
    QSqlQuery pragmaQuery;
    pragmaQuery.exec("PRAGMA foreign_keys = ON;");

    QSqlQuery deleteDataQuery;
    deleteDataQuery.prepare("DELETE FROM Data WHERE userID = :id");
    deleteDataQuery.bindValue(":id", loggedInUserId);

    if (!deleteDataQuery.exec()) {
        qWarning() << "Failed to delete user data:" << deleteDataQuery.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to delete associated data. Please try again.");
        return;
    }
    QSqlQuery query;
    query.prepare("DELETE FROM User WHERE userID = :id");
    query.bindValue(":id", loggedInUserId);

    if (!query.exec()) {
        qWarning() << "Failed to delete user:" << query.lastError().text();
        QMessageBox::critical(this, "Error", "Failed to delete your account. Please try again.");
        return;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Error", "No account found to delete.");
        return;
    }

    QMessageBox::information(this, "Account Deleted", "Your account has been successfully deleted.");
    ui->txtUsername->setEnabled(true);
    ui->txtPassword->setEnabled(true);
    ui->btnLogin->setEnabled(true);
    ui->btnCreate->setEnabled(true);
    ui->btnLogin->setText("Login");
    loggedInUserId = -1;
    ui->txtUsername->setText("");
    ui->txtPassword->setText("");

    app->loadUsersFromDatabase();
    updatePreviousMenu();
}

void MainWindow::updatePreviousMenu() {
    if(loggedInUserId == -1){
        box_select->clear();
        return;
    }
    box_select->clear();
    QSqlQuery query;
    query.prepare("SELECT date, time, dataID FROM Data WHERE userID = :userID");
    query.bindValue(":userID", loggedInUserId);
    if (!query.exec()) {
        qDebug() << "Error retrieving records for user:" << query.lastError().text();
        return;
    }
    std::vector<QString> records;
    while (query.next()) {
        QString date = query.value(0).toString();
        QString time = query.value(1).toString();
        int id = query.value(2).toInt();
        QString record = QString("%1   %2   ID: %3").arg(date, time).arg(id);
        records.push_back(record);
        box_select->addItem(record);
    }
    for (const auto& record : records) {
        qDebug() << "Record:" << record;
    }
}

void MainWindow::on_btnRecommendation_clicked()
{
    QMessageBox::information(this, "Recommendation", "Recommendation placeholder");
}

