#include "timestamp_readerCSV.h"

ts_readerCSV::ts_readerCSV(QString &fn, QObject *parent)
    : ts_reader(parent)
{  // why doesn't work this pointer in new QTimer(/*this*/)?
    connect(tm1, &QTimer::timeout, this, &ts_readerCSV::readString);
    tm1->setInterval(1000);
    QFile file(fn);
    if (!file.exists()) {
        qDebug() << "Файл не найден:" << fn;
        // message window here
        return;
    }
    rapidcsv::SeparatorParams sepParams(';');
    rapidcsv::LabelParams labelParams(-1, -1);

    doc = new rapidcsv::Document{fn.toStdString(), labelParams, sepParams};
    maxRow = doc->GetRowCount();
//    qDebug() << "The number of rows is " << maxRow;
    std::vector<std::string> vstr = doc->GetRow<std::string>(currRow);
    curValue.clear();
    for(int i=0; i<8; i++){
        curValue.append(QString::fromStdString(vstr[i+1]));
    }
}

ts_readerCSV::~ts_readerCSV(){}

int ts_readerCSV:: readString() {
    if(maxRow > currRow) currRow++;
    std::vector<std::string> vstr = doc->GetRow<std::string>(currRow);
//    qDebug() << "forever_1";
    curValue.clear();
    for(int i=0; i<8; i++){
        curValue.append(QString::fromStdString(vstr[i+1]));
    }
    return 0;
}

/*
 *
cpp
// В заголовочном файле (.h)
private:
    QTimer *monitorTimer;
    int lastSecond = -1;
    void checkRtcSync();

// В файле реализации (.cpp)
monitorTimer = new QTimer(this);
// Устанавливаем высокую точность для Windows
monitorTimer->setTimerType(Qt::PreciseTimer);
connect(monitorTimer, &QTimer::timeout, this, &MyClass::checkRtcSync);

// Интервал 10-20 мс дает отличный баланс точности и нагрузки
monitorTimer->start(20);

void MyClass::checkRtcSync() {
    QTime currentTime = QTime::currentTime();
    int currentSecond = currentTime.second();

    // Если текущая секунда изменилась по сравнению с сохраненной
    if (currentSecond != lastSecond) {
        lastSecond = currentSecond;

        // ВЫПОЛНЯЕМ ДЕЙСТВИЕ (ровно в начале новой секунды)
        qDebug() << "RTC Tick:" << currentTime.toString("hh:mm:ss.zzz");

        emit secondChanged(currentTime);
    }
}
 */
