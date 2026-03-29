#include "mainwindow.h"
#include "../forms/ui_mainwindow.h"

MainWindow::MainWindow(QString testn, QString paramsfn, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , rtc(new QTimer(this))
    , tmr(new QTimer(this))

//    , tsr(new ts_readerCSV(fn, this))

    , tsr(new ts_readerDB(testn, this))
    , watch()
//    , prm({paramsfn})
    , prm({paramsfn})
    , tcp(new tcpServer(watch, prm, this))
    , secCounter(0)
{
    ui->setupUi(this);
    QDateTime now = watch.get_watch();
    ui->RTC_label->setText(now.toString("dd.MM.yyyy hh:mm:ss"));
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startButtonClick);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopButtonClick);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeButtonClick);
//    connect(this->tsr->getTimer(), &QTimer::timeout, this, &MainWindow::setTimerLabel);
    rtc->setInterval(1000);
    tmr->setInterval(1000);
    connect(rtc, &QTimer::timeout, this, &MainWindow::setRTC);
    connect(tmr, &QTimer::timeout, this, &MainWindow::setTimerLabel);
//    connect(tmr, &QTimer::timeout, tsr, &ts_reader::handleTimerSignal);
    connect(tsr, &ts_reader::wasChanged, tcp, &tcpServer::getChanged);
    connect(tsr, &ts_reader::reached_end, &ts_reader::reached_end);
    rtc->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startButtonClick(){
//    qDebug() << "MainWindow::startButtonClick()";
    tsr->start_ts();
    tmr->start();
}
void MainWindow::stopButtonClick(){
    tmr->stop();
    tsr->stop_ts();
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
