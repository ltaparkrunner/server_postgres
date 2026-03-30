#include "mainwindow.h"
#include "../forms/ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(const QString& testn, const QString& paramsfn, const QString& qssfn, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , rtc(new QTimer(this))
    , tmr(new QTimer(this))

//    , tsr(new ts_readerCSV(fn, this))

    , tsr(nullptr)
    , watch()
//    , prm({paramsfn})
    , prm({paramsfn})
    , tcp(new tcpServer(watch, prm, this))
    , secCounter(0)
    , testfn(testn)
    , styleSheetText(readQss(qssfn))
    , is_DB(true)
    , first(true)
{
    if (!styleSheetText.isEmpty()) {
        qApp->setStyleSheet(styleSheetText);
    }
    ui->setupUi(this);
    setWindowTitle("Test server");
    this->setFixedSize(400, 630);
    this->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    ui->actionOpen_test_file->setEnabled(false);

    QDateTime now = watch.get_watch();
    ui->RTC_label->setText(now.toString("dd.MM.yyyy hh:mm:ss"));
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startButtonClick);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopButtonClick);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeButtonClick);

    rtc->setInterval(1000);
    tmr->setInterval(1000);
    connect(rtc, &QTimer::timeout, this, &MainWindow::setRTC);
    connect(tmr, &QTimer::timeout, this, &MainWindow::setTimerLabel);
    rtc->start();
    ui->stopButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startButtonClick(){
    if(first){
        if(is_DB) {
            tsr = new ts_readerDB(this);
        }
        else{
            tsr = new ts_readerCSV(testfn, this);
        }
        connect(tsr, &ts_reader::wasChanged, tcp, &tcpServer::getChanged);
        connect(tsr, &ts_reader::reached_end, this, &MainWindow::reached_end);
        connect(tsr, &ts_reader::msgStatusbar, this, &MainWindow::statusbarMsg);
        first = false;
    }
    tsr->start_ts();
    tmr->start();
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->actionDB_mode->setEnabled(false);
    ui->actionOpen_test_file->setEnabled(false);
}
void MainWindow::stopButtonClick(){
    tmr->stop();
    tsr->stop_ts();
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
}
void MainWindow::closeButtonClick(){
    this->close();
}
void MainWindow::setTimerLabel(){
    secCounter++;
    // Calculate hours, minutes, and seconds
    int seconds = secCounter % 60;
    int minutes = (secCounter / (60)) % 60;
    int hours = (secCounter / (60 * 60)) % 24; // Modulo 24 for daily counter

    // Format the time as "HH:mm:ss"
    QString formattedTime = QString("%1:%2:%3")
                                .arg(hours, 2, 10, QChar('0'))
                                .arg(minutes, 2, 10, QChar('0'))
                                .arg(seconds, 2, 10, QChar('0'));

    // Update the UI
    ui->time_label->setText(formattedTime);

    QVector<QString> curr = tsr->get_values();

    if(curr.size() >= 8){
        ui->probe_0->setText(curr[0]);
        ui->probe_1->setText(curr[1]);
        ui->probe_2->setText(curr[2]);
        ui->probe_3->setText(curr[3]);
        ui->probe_4->setText(curr[4]);
        ui->probe_5->setText(curr[5]);
        ui->probe_6->setText(curr[6]);
        ui->probe_7->setText(curr[7]);
    }
}

void MainWindow::reached_end(){
    tmr->stop();
    tsr->stop_ts();
}

void MainWindow::setRTC() {
    QDateTime now = watch.get_watch();
    ui->RTC_label->setText(now.toString("dd.MM.yyyy hh:mm:ss"));
}

void MainWindow::on_actionOpen_test_file_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", "", "csv files (*.csv)");
    if (!fileName.isEmpty()) {
        testfn = fileName;
    }
}


//void MainWindow::on_actionDB_mode_toggled(bool arg1)
void MainWindow::on_actionDB_mode_toggled(bool checked)
{
    if (checked) {
        ui->actionOpen_test_file->setEnabled(false);
        is_DB = true;
    } else {
        ui->actionOpen_test_file->setEnabled(true);
        is_DB = false;
    }
}

QString MainWindow::readQss(const QString& qssfn){
    QFile styleSheetFile(qssfn);
    if (!styleSheetFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't find" << styleSheetFile.fileName();
        return {};
    }
        return QString::fromUtf8(styleSheetFile.readAll());
}

void MainWindow::statusbarMsg(const QString& msg){
    ui->statusbar->showMessage(msg, 5000);
}
