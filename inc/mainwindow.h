#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#define TXT
//#ifdef TXT
#include "timestamp_readerCSV.h"
//#else
#include "timestamp_readerDB.h"
//#endif
#include <params_watch.h>
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
    MainWindow(QString testn, QString paramsfn, QWidget *parent = nullptr);
    ~MainWindow();
    void startButtonClick();
    void stopButtonClick();
    void closeButtonClick();
    void setTimerLabel();
    void reached_end();
private:
    Ui::MainWindow *ui;
    QTimer *tmr;
public:
//    ts_readerCSV *tsr;
    ts_readerDB *tsr;
private:
    watch_t watch;
    params prm;
    tcpServer *tcp;
    int secCounter;
};
#endif // MAINWINDOW_H
