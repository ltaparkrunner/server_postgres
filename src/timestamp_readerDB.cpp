#include "timestamp_readerDB.h"

ts_readerDB::ts_readerDB(QObject *parent)
    : ts_reader(parent)
{
//    tm1->setInterval(1000);
    // 1. Creating a database object using the QPSQL driver
    QSqlDatabase pgdb = QSqlDatabase::addDatabase("QPSQL");

    // 2. Specify connection parameters
    pgdb.setHostName("localhost");      // server address
    pgdb.setDatabaseName("test_1"); // Base name
    pgdb.setUserName("postgres");       // Login
    pgdb.setPassword("Forever");  // Password
    pgdb.setPort(5432);                 // Standard port 5432

    // 3. trying to open a connection
    if (!pgdb.open()) {
        qDebug() << "Connection error:" << pgdb.lastError().text();
    } else {
        qDebug() << "Successful connection to PostgreSQL!";
    }

    maxRow = ts_readerDB::getMaxIndex();
    qDebug() << "maxRow" << maxRow;
    ts_readerDB::readString();
}

ts_readerDB::~ts_readerDB(){}

QSqlTableModel* ts_readerDB::init_table(QObject *parent){
    // 1. Creating database object with QPSQL driver Создаем объект базы данных с драйвером QPSQL
    QSqlDatabase pgdb = QSqlDatabase::addDatabase("QPSQL");

    // 2. Specify connection parameters
    pgdb.setHostName("localhost");      // server address
    pgdb.setDatabaseName("test_1"); // database name
    pgdb.setUserName("postgres");       // Login
    pgdb.setPassword("Forever");  // Password
    pgdb.setPort(5432);                 // Standard port 5432

    // 3. trying to open a connection
    if (!pgdb.open()) {
        qDebug() << "Connection error:" << pgdb.lastError().text();
    } else {
        qDebug() << "Successful connection to PostgreSQL!";
    }
    return new QSqlTableModel(parent, pgdb);
}

int ts_readerDB:: readString() {
    if(maxRow > currRow) currRow++;
    if(getRowByKey(currRow)) {
        emit wasChanged(curValue);
    }
    else {
        stop_ts();
        emit reached_end();
        return -1;
    }
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
