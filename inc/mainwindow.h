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
    MainWindow(const QString& testn, const QString& paramsfn, const QString& qssfn, QWidget *parent = nullptr);
    ~MainWindow();
    void startButtonClick();
    void stopButtonClick();
    void closeButtonClick();
    void setRTC();
    void setTimerLabel();
    void reached_end();
    QString readQss(const QString& qssfn);
//    void activateStylesheet(bool dark);
private:
    Ui::MainWindow *ui;
    QTimer *rtc;
    QTimer *tmr;
public:
//    ts_readerCSV *tsr;
    ts_reader *tsr;
private slots:
    void on_actionOpen_test_file_triggered();
    void on_actionDB_mode_toggled(bool checked);

private:
    watch_t watch;
    params prm;
    tcpServer *tcp;
    int secCounter;
    QString testfn;
    QString styleSheetText;
    bool is_DB;
};
#endif // MAINWINDOW_H
