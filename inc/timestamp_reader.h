#ifndef TIMESTAMP_READER_H
#define TIMESTAMP_READER_H
#include <QObject>
#include <QTimer>
#include <QVector>
#include <QFile>

#include <QTextStream>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>

#include <QString>

#include "rapidcsv.h"

class ts_reader : public QObject{
    Q_OBJECT
public:
    explicit ts_reader(QString &fn, QObject *parent = nullptr);
    ~ts_reader();
    QVector<QString> get_values();
    QVector<QString> get_valuesSQL();
    void stop_ts();
    void start_ts();
    void connectToPostgres();
//    void handleTimerSignal();
//    void handleTimerSignal(){}
    // QTimer* getTimer();
    int readString();
    int readStringSQL();
private:
    QTimer *tm1;
//    QString *fname;
//    QFile *fl;

//    QTextStream *in;
    QSqlDatabase *pgdb;
//    QVector<QString> curValue;

    std::vector<std::string> curValue;
    QSqlRecord curValueSQL;

    rapidcsv::Document *doc;
    size_t maxRow;
    size_t maxRowSQL;
    size_t currRow;
    size_t currRowSQL;
    QSqlTableModel *model;
};

#endif // TIMESTAMP_READER_H
