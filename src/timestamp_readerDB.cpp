#include "timestamp_readerDB.h"

ts_readerDB::ts_readerDB(QString &fn, QObject *parent)
    : QObject(parent)
    ,   model(init_table(this))
    ,   tm1(new QTimer(this))
    ,   curValueDB()
    ,   currRowDB(0)
    ,   maxRowDB(0)
{
    connect(tm1, &QTimer::timeout, this, &ts_readerDB::readStringDB);
    tm1->setInterval(1000);
    model->setTable("test");
    model->select();
    maxRowDB = model->rowCount();
    qDebug() << "Connected to table. Row count:" << maxRowDB;
    curValueDB = model->record(currRowDB);
    qDebug() << "connectToPostgres curValueSQL.count()" << curValueDB.count();
}

ts_readerDB::~ts_readerDB(){}

QVector<QString> ts_readerDB::get_valuesDB(){
    QVector<QString> vstr; //{"10", "20", "30", "40", "50", "60", "70", "80"};
    for(int i=0; i<8; i++){
        vstr.append(curValueDB.value(i+1).toString());
    }
    return vstr;
}

void ts_readerDB::start_ts(){ tm1->start(); }
void ts_readerDB::stop_ts(){ tm1->stop(); }

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
    getRowByKey(3);
    getRowByKey(15);
    getRowByKey(260000);

    return new QSqlTableModel(parent, pgdb);
}

int ts_readerDB:: readStringDB() {
    if(maxRowDB > currRowDB) currRowDB++;
    curValueDB = model->record(currRowDB);       //GetRow<std::string>(currRow);
//    qDebug() << " curValueDB length = " << curValueDB.count();
    return 0;
}

void ts_readerDB::getRowByKey(int key) {
    QSqlQuery query;
    // Подготавливаем запрос
    query.prepare("SELECT * FROM Test WHERE row_num = :id");
    query.bindValue(":id", key);

    if (query.exec()) {
        if (query.next()) {
            // Получаем данные по именам столбцов или индексам
            // QString name = query.value("name").toString();
            // int age = query.value("age").toInt();
            if(query.isValid() )
                for(int i=1; i<9; i++){
                    qDebug() << " getRowByKey query the row num: " << key << " the index: "
                             << i << "  " << query.value(i).toString();
                }
//            qDebug() << "Найдено:" << name << age;
            else qDebug() << " query is not valid key: " << key;
        } else {
            qDebug() << "Запись не найдена";
        }
    } else {
        qDebug() << "Ошибка запроса:" << query.lastError().text();
    }
}
