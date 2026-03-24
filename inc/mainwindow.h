#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "timestamp_reader.h"
#include "tcpServer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString fn, QWidget *parent = nullptr);
    ~MainWindow();
    void startButtonClick();
    void stopButtonClick();
    void closeButtonClick();
    void setTimerLabel();

private:
    Ui::MainWindow *ui;
    QTimer *tmr;
public:
    ts_reader *tsr;
private:
    tcpServer *tcp;
    int secCounter;
};
#endif // MAINWINDOW_H
