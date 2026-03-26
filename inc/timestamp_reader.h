#ifndef TIMESTAMP_READER_H
#define TIMESTAMP_READER_H

#include <QObject>
#include <QTimer>
#include <QVector>

#include <QDebug>
#include <QString>

class ts_reader : public QObject{
    Q_OBJECT
public:
    explicit ts_reader(/*QString &fn, QString &tbln, */ QObject *parent = nullptr);
    virtual ~ts_reader() = 0;
    virtual QVector<QString> get_values() = 0;

    virtual void start_ts(){ tm1->start(); }
    virtual void stop_ts(){ tm1->stop(); }

    virtual int readString() = 0;
signals:
    void wasChanged(QVector<QString> vs);
protected:
    QTimer *tm1;
    QVector<QString> curValue;

    size_t maxRow;
    size_t currRow;
};

#endif // TIMESTAMP_READER_H
