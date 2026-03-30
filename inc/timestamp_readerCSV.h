#ifndef TIMESTAMP_READERCSV_H
#define TIMESTAMP_READERCSV_H
#include <timestamp_reader.h>
#include <QFile>

#include <QTextStream>
#include "rapidcsv.h"

class ts_readerCSV : public ts_reader{
    Q_OBJECT
public:
    explicit ts_readerCSV(const QString &fn, QObject *parent = nullptr);
    ~ts_readerCSV();
    int getMaxIndex() override;
    int readString() override;
private:
    rapidcsv::Document *doc;
};

#endif // TIMESTAMP_READERCSV_H
