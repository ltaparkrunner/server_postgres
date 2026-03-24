#include "timestamp_reader.h"

ts_reader::ts_reader() : tm1(new QTimer(/*this*/)){  // why doesn't work this pointer in new QTimer(/*this*/)?
    tm1->setInterval(1000);
}

ts_reader::~ts_reader(){}
QVector<int> ts_reader::get_values(){
    return {};
}
// void ts_reader::stop_ts(){ tm1->start(); }
// void ts_reader::start_ts(){ tm1->stop(); }
// QTimer* ts_reader::getTimer(){
//     return tm1;
// }

void ts_reader::connectToPostgres() {
    // 1. Создаем объект базы данных с драйвером QPSQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    // 2. Указываем параметры подключения
    db.setHostName("localhost");      // Адрес сервера
    db.setDatabaseName("test_1"); // Имя базы
    db.setUserName("postgres");       // Логин
    db.setPassword("Forever");  // Пароль
    db.setPort(5432);                 // Стандартный порт 5432

    // 3. Пытаемся открыть соединение
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
    } else {
        qDebug() << "Успешное подключение к PostgreSQL!";
    }
}

// void ts_reader::handleTimerSignal(){
// }
