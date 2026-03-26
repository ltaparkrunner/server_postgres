#ifndef TIMESTAMP_READERDB_H
#define TIMESTAMP_READERDB_H

#include <QObject>
#include <QTimer>
#include <QVector>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>

#include <QString>
#include <QSqlQuery>

class ts_readerDB : public QObject{
    Q_OBJECT
public:
    explicit ts_readerDB(QString &fn, QObject *parent = nullptr);
    ~ts_readerDB();

    QVector<QString> get_valuesDB();
    void stop_ts();
    void start_ts();

    int readStringDB();
    QSqlTableModel* init_table(QObject *parent = nullptr);
    void getRowByKey(int key);
signals:
    void wasChanged(QVector<QString> vs);
private:
    QSqlTableModel *model;
    QTimer *tm1;

    QSqlRecord curValueDB;

    size_t currRowDB;
    size_t maxRowDB;
};

#endif // TIMESTAMP_READERDB_H
