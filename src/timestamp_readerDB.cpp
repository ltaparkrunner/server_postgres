#include "timestamp_readerDB.h"

ts_readerDB::ts_readerDB(QString &fn, QObject *parent)
    : ts_reader(parent)
//    ,   model(init_table(this))
{
//    connect(tm1, &QTimer::timeout, this, &ts_reader::readString);
    tm1->setInterval(1000);

    QSqlDatabase pgdb = QSqlDatabase::addDatabase("QPSQL");

    // 2. Указываем параметры подключения
    pgdb.setHostName("localhost");      // Адрес сервера
    pgdb.setDatabaseName("test_1"); // Имя базы
    pgdb.setUserName("postgres");       // Логин
    pgdb.setPassword("Forever");  // Пароль
    pgdb.setPort(5432);                 // Стандартный порт 5432

    // 3. Пытаемся открыть соединение
    if (!pgdb.open()) {
        qDebug() << "Ошибка подключения:" << pgdb.lastError().text();
    } else {
        qDebug() << "Успешное подключение к PostgreSQL!";
    }

    maxRow = ts_readerDB::getMaxIndex();
    qDebug() << "maxRow" << maxRow;
    // model->setTable("test");
    // model->select();
    // maxRow = model->rowCount();

    // QSqlRecord sr = model->record(currRow);
    // curValue.clear();
    // for(int i=0; i<8; i++){
    //     curValue.append(sr.value(i+1).toString());
    // }
    // qDebug() << "connectToPostgres curValueSQL.count()" << curValue.count();

}

ts_readerDB::~ts_readerDB(){}

QSqlTableModel* ts_readerDB::init_table(QObject *parent){
    // 1. Создаем объект базы данных с драйвером QPSQL
    QSqlDatabase pgdb = QSqlDatabase::addDatabase("QPSQL");

    // 2. Указываем параметры подключения
    pgdb.setHostName("localhost");      // Адрес сервера
    pgdb.setDatabaseName("test_1"); // Имя базы
    pgdb.setUserName("postgres");       // Логин
    pgdb.setPassword("Forever");  // Пароль
    pgdb.setPort(5432);                 // Стандартный порт 5432

    // 3. Пытаемся открыть соединение
    if (!pgdb.open()) {
        qDebug() << "Ошибка подключения:" << pgdb.lastError().text();
    } else {
        qDebug() << "Успешное подключение к PostgreSQL!";
    }
    return new QSqlTableModel(parent, pgdb);
}

int ts_readerDB:: readString() {
    if(maxRow > currRow) currRow++;
/*
    QSqlRecord sr = model->record(currRow);
    //  curValue.clear();
    for(int i=0; i<8; i++){
        //  curValue.append(sr.value(i+1).toString());
        curValue[i] = sr.value(i+1).toString();
    }
*/
//    qDebug() << "ts_readerDB:: readString() curr_row: " << currRow;
    if(getRowByKey(currRow)) {
        emit wasChanged(curValue);
    }
    else {
        stop_ts();
        emit reached_end();
        return -1;
    }
//    qDebug() << "ts_readerDB:: readString() fin " << currRow;
    return 0;
}

int ts_readerDB::getMaxIndex(){
    QSqlQuery query;
    // Preparing a request
    query.prepare("SELECT MAX(row_num) FROM Test");
    if (query.exec()) {
        if (query.next()) {
            if(query.isValid() )
                return query.value(0).toInt();
            else {
                return -1;
            }
        } else {
            qDebug() << "record not found";
            return -1;
        }
    } else {
        qDebug() << "Request error:" << query.lastError().text();
        return -1;
    }
}


bool ts_readerDB::getRowByKey(int key) {
    QSqlQuery query;
    // Preparing a request
    query.prepare("SELECT * FROM Test WHERE row_num = :id");
    query.bindValue(":id", key);

    if (query.exec()) {
        if (query.next()) {
            // Getting data by column names or indexes
            if(query.isValid() )
                for(int i=0; i<8; i++){
                    // qDebug() << " getRowByKey query the row num: " << key << " the index: "
                    //          << i << "  " << query.value(i).toString();
                    //qDebug() << "i= " << i;
                    curValue[i] = query.value(i+1).toString();
                }
            else {  qDebug() << " query is not valid key: " << key;
                return false;
            }
        } else {
            qDebug() << "record not found";
            return false;
        }
    } else {
        qDebug() << "Request error:" << query.lastError().text();
        return false;
    }
    return true;
}
