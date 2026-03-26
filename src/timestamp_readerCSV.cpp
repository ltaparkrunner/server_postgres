#include "timestamp_readerCSV.h"

ts_reader::ts_reader(QString &fn, QObject *parent)
    : QObject(parent)
,   tm1(new QTimer(this))
//,   fname(new QString(fn))  // here to move not to copy? how to check?
,   currRow(0)
//,   currRowSQL(0)
{  // why doesn't work this pointer in new QTimer(/*this*/)?
    connect(tm1, &QTimer::timeout, this, &ts_reader::readString);
//    connect(tm1, &QTimer::timeout, this, &ts_reader::readStringSQL);
    tm1->setInterval(1000);
    QFile file(fn);
    if (!file.exists()) {
        qDebug() << "Файл не найден:" << fn;
        // message window here
        return;
    }
    rapidcsv::SeparatorParams sepParams(';');
    rapidcsv::LabelParams labelParams(-1, -1);
//    rapidcsv::Document doc(fn.toStdString(), labelParams, sepParams);
    doc = new rapidcsv::Document{fn.toStdString(), labelParams, sepParams};
    maxRow = doc->GetRowCount();
    qDebug() << "The number of rows is " << maxRow;
    curValue = doc->GetRow<std::string>(currRow);
//    connectToPostgres();
//    maxRowSQL =
}

ts_reader::~ts_reader(){}

QVector<QString> ts_reader::get_values(){
    QVector<QString> vstr; //{"10", "20", "30", "40", "50", "60", "70", "80"};
//    qDebug() << "curValue.size()" << curValue.size() << "currRow" << currRow;
    for(int i=0; i<8; i++){
        vstr.append(QString::fromStdString(curValue[i+1]));
    }
    return vstr;
}

/*
QVector<QString> ts_reader::get_valuesSQL(){
    QVector<QString> vstr; //{"10", "20", "30", "40", "50", "60", "70", "80"};
//    qDebug() << "curValueSQL.count()" << curValueSQL.count() << "currRow" << currRow;
    for(int i=0; i<8; i++){
//        qDebug() << "curValueSQL.value(" << i+1 << ")= " << curValueSQL.value(i+1);
        // vstr.append(curValueSQL.value("probe1").toString());
        // vstr.append(curValueSQL.value("probe1").toString());
        vstr.append(curValueSQL.value(i+1).toString());
    }
    return vstr;
}
*/

void ts_reader::start_ts(){ tm1->start(); }
void ts_reader::stop_ts(){ tm1->stop(); }

/*
void ts_reader::connectToPostgres() {
    // 1. Создаем объект базы данных с драйвером QPSQL
//    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    pgdb = new QSqlDatabase();
    *pgdb = QSqlDatabase::addDatabase("QPSQL");

    // 2. Указываем параметры подключения
    pgdb->setHostName("localhost");      // Адрес сервера
    pgdb->setDatabaseName("test_1"); // Имя базы
    pgdb->setUserName("postgres");       // Логин
    pgdb->setPassword("Forever");  // Пароль
    pgdb->setPort(5432);                 // Стандартный порт 5432

    // 3. Пытаемся открыть соединение
    if (!pgdb->open()) {
        qDebug() << "Ошибка подключения:" << pgdb->lastError().text();
    } else {
        qDebug() << "Успешное подключение к PostgreSQL!";
    }

    model = new QSqlTableModel(nullptr, *pgdb);
    model->setTable("test");
    model->select();
    qDebug() << "Connected to table. Row count:" << model->rowCount();
    maxRowSQL = model->rowCount();
    curValueSQL = model->record(currRowSQL);
    qDebug() << "connectToPostgres curValueSQL.count()" << curValueSQL.count();
}
*/
int ts_reader:: readString() {
    if(maxRow > currRow) currRow++;
    curValue = doc->GetRow<std::string>(currRow);
//    qDebug() << "curValue.size()" << curValue.size();
//    std::vector<QString> vec = doc->GetRow<QString>(currRow);
    return 0;
}
/*
int ts_reader:: readStringSQL() {
    if(maxRowSQL > currRowSQL) currRowSQL++;
    curValueSQL = model->record(currRowSQL);       //GetRow<std::string>(currRow);
//    qDebug() << "curValueSQL.count()" << curValueSQL.count();
    //    std::vector<QString> vec = doc->GetRow<QString>(currRow);
    return 0;
}
*/
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
