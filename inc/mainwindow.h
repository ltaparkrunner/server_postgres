#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#define TXT
#ifdef TXT
#include "timestamp_readerCSV.h"
#else
#include "timestamp_readerDB.h"
#endif
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
#ifdef TXT
    ts_readerCSV *tsr;
#else
    ts_readerDB *tsr;
#endif
private:
    tcpServer *tcp;
    int secCounter;
};
#endif // MAINWINDOW_H
