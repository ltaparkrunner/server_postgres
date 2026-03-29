#ifndef TIMESTAMP_READERDB_H
#define TIMESTAMP_READERDB_H
#include "timestamp_reader.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>

#include <QString>
#include <QSqlQuery>

class ts_readerDB : public ts_reader{
    Q_OBJECT
public:
    explicit ts_readerDB(QString &fn, QObject *parent = nullptr);
    ~ts_readerDB();
    int getMaxIndex() override;
    int readString() override;
    QSqlTableModel* init_table(QObject *parent = nullptr);
    bool getRowByKey(int key);
// signals:
//     void wasChanged(QVector<QString> vs);
private:
//    QSqlTableModel *model;
};

#endif // TIMESTAMP_READERDB_H
