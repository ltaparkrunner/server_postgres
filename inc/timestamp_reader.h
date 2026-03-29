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
    virtual ~ts_reader();
    virtual QVector<QString> get_values();

    virtual void start_ts(){ tm1->start(); }
    virtual void stop_ts(){ tm1->stop(); }

    virtual int readString() = 0;
    virtual int getMaxIndex() = 0;
signals:
    void wasChanged(QVector<QString>& vs);
    void reached_end();
    void tm_timeout();
protected:
    QTimer *tm1;
    QVector<QString> curValue;

    size_t currRow;
    size_t maxRow;
};

#endif // TIMESTAMP_READER_H
