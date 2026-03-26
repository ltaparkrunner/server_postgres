#ifndef TIMESTAMP_READERCSV_H
#define TIMESTAMP_READERCSV_H
#include <QObject>
#include <QTimer>
#include <QVector>
#include <QFile>

#include <QTextStream>
#include <QDebug>
#include <QString>
#include "rapidcsv.h"

class ts_reader : public QObject{
    Q_OBJECT
public:
    explicit ts_reader(QString &fn, QObject *parent = nullptr);
    ~ts_reader();
    QVector<QString> get_values();

    void stop_ts();
    void start_ts();

    int readString();
private:
    QTimer *tm1;
    std::vector<std::string> curValue;

    rapidcsv::Document *doc;
    size_t maxRow;
    size_t currRow;
};

#endif // TIMESTAMP_READERCSV_H
