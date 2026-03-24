#include "mainwindow.h"
#include "../forms/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tmr(new QTimer())
    , tsr(new ts_reader())
    , tcp(new tcpServer())
    , secCounter(0)
{
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startButtonClick);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopButtonClick);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeButtonClick);
//    connect(this->tsr->getTimer(), &QTimer::timeout, this, &MainWindow::setTimerLabel);
    tmr->setInterval(1000);
    connect(tmr, &QTimer::timeout, this, &MainWindow::setTimerLabel);
    connect(tmr, &QTimer::timeout, tsr, &ts_reader::handleTimerSignal);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startButtonClick(){
//    tsr->start_ts();
    tmr->start();
}
void MainWindow::stopButtonClick(){
//    tsr->stop_ts();
    tmr->stop();
}
void MainWindow::closeButtonClick(){}
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
}
