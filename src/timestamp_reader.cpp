#include "timestamp_reader.h"

ts_reader::ts_reader(/*QString &fn, QString &tbln, */ QObject *parent)
    : QObject(parent)
    ,   tm1(new QTimer(this))
    ,   curValue ({ "490", "490", "490", "490", "490", "490", "490", "490" })
    ,   currRow(0)
    ,   maxRow(0)
{
    qDebug() << "ts_reader::ts_reader Constructor";
}

ts_reader::~ts_reader() {
    delete tm1;
}

QVector<QString> ts_reader::ts_reader::get_values(){
    return curValue;
}
