#include "timestamp_readerDB.h"

ts_readerDB::ts_readerDB(QString &fn, QObject *parent)
    : ts_reader(parent)
    ,   model(init_table(this))
{
    connect(tm1, &QTimer::timeout, this, &ts_readerDB::readString);
    tm1->setInterval(1000);
    model->setTable("test");
    model->select();
    maxRow = model->rowCount();
    qDebug() << "Connected to table. Row count:" << maxRow;
    QSqlRecord sr = model->record(currRow);
    curValue.clear();
    for(int i=0; i<8; i++){
        curValue.append(sr.value(i+1).toString());
    }
    qDebug() << "connectToPostgres curValueSQL.count()" << curValue.count();
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
    getRowByKey(3);
    getRowByKey(15);
    getRowByKey(260000);

    return new QSqlTableModel(parent, pgdb);
}

int ts_readerDB:: readString() {
    if(maxRow > currRow) currRow++;
    QSqlRecord sr = model->record(currRow);
    curValue.clear();
    for(int i=0; i<8; i++){
        curValue.append(sr.value(i+1).toString());
    }
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
            if(query.isValid() )
                for(int i=1; i<9; i++){
                    qDebug() << " getRowByKey query the row num: " << key << " the index: "
                             << i << "  " << query.value(i).toString();
                }
            else qDebug() << " query is not valid key: " << key;
        } else {
            qDebug() << "Запись не найдена";
        }
    } else {
        qDebug() << "Ошибка запроса:" << query.lastError().text();
    }
}
