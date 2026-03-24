#ifndef TIMESTAMP_READER_H
#define TIMESTAMP_READER_H
#include <QObject>
#include <QTimer>
#include <QVector>
#include <QFile>

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>


class ts_reader : public QObject{
    Q_OBJECT
public:
    ts_reader();
    ~ts_reader();
    QVector<int> get_values();
    // void stop_ts();
    // void start_ts();
    void connectToPostgres();
//    void handleTimerSignal();
    void handleTimerSignal(){}
    // QTimer* getTimer();
private:
    QTimer *tm1;
    QFile *fl;
    QSqlDatabase *db;

};

#endif // TIMESTAMP_READER_H
