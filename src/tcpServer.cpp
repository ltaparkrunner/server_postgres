#include "tcpServer.h"

tcpServer::tcpServer(QObject *parent):QTcpServer(parent)
{
    if (listen(QHostAddress::Any, 502)) {
        qDebug() << "Server started on port 502";
    }
}

tcpServer::~tcpServer(){}

void tcpServer::incomingConnection(qintptr socketDescriptor){
    QTcpSocket *socket = new QTcpSocket(this);
//    socket->setSocketDescriptor(socketDescriptor);

    qDebug() << "New connection attempt, socket descriptor:" << socketDescriptor;

    if (!socket->setSocketDescriptor(socketDescriptor)) {
        socket->deleteLater();
        return;
    }

    connect(socket, &QTcpSocket::readyRead, [socket, this]() {
        QByteArray request = socket->readAll();
        if (request.size() < 7) return; // Минимум для MBAP заголовка

        // Данные для ответа (8 значений по 16 бит)
        uint16_t values[8] = {400, 410, 420, 430, 440, 460, 470, 480};

        QByteArray response;
        //QDataStream out(&response, QIODevice::WriteOnly);
        //out.setByteOrder(QDataStream::BigEndian);

        // process the msg
        if (request[0] == 1 && request[1] == 1 && request[2] == 0 && request[3] == 0 &&
            request[4] == 0 && request[5] == 6 && request[6] == 33 && request[7] == 3 &&
            request[8] == 0 && request[9] == 0 && request[10] == 0 && request[11] == 16)
            send_mklp_data_time(request, response);

        if (request[2] == 0 && request[3] == 0 && request[4] == 0 && request[5] > 0 &&
            request[7] == 0x10 && request[9] == 8 && request[11] == 3)
//            RTC_Time_Install(request, out);
            ;
        if (request[0] == 0x47 && request[1] == 0x42 && request[5] == 0x3e && request[7] == 0x10
            && request[8] == 0 && request[9] == 0x33)
//            SetParams(request, out);
            ;
        if (request[0] == 0x47 && request[1] == 0x42 && request[5] == 6 && request[7] == 3
            && request[8] == 0 && request[9] == 0x33)
//            send_settings(request, out);
            ;
/*
        // 1. MBAP Header
        out << (uint16_t)0x0001;      // Transaction ID (копируем из запроса обычно)
        out << (uint16_t)0x0000;      // Protocol ID (всегда 0 для Modbus)
        out << (uint16_t)(2 + 1 + 16); // Length (Unit ID + FC + Data)
        out << (uint8_t)0x01;         // Unit ID

        // 2. Modbus PDU
        out << (uint8_t)0x03;         // Function Code (Read Holding Registers)
        out << (uint8_t)16;           // Byte Count (8 регистров * 2 байта)

        for(int i = 0; i < 8; ++i) {
            out << values[i];         // Сами данные
        }
*/
        socket->write(response);
        qDebug() << "socket->write(response);";
    });

//    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    // // --- Установка (Establish) ---
    // connect(socket, &QTcpSocket::readyRead, [socket]() {
    //     QByteArray data = socket->readAll();
    //     qDebug() << "Data received from" << socket->peerAddress().toString() << ":" << data;
    //     // Эхо-ответ (опционально)
    //     socket->write("ACK: " + data);
    // });

    // --- Завершение (Tear down) ---
    connect(socket, &QTcpSocket::disconnected, [socket]() {
        qDebug() << "Client disconnected:" << socket->peerAddress().toString();
        // Важно: планируем удаление объекта сокета, чтобы не было утечек памяти
        socket->deleteLater();
    });

    qDebug() << "Connection established with:" << socket->peerAddress().toString();
}

int tcpServer::send_mklp_data_time(QByteArray &request, QByteArray &response){
    qDebug() << "tcpServer::send_mklp_data_time";
    qDebug() << "request[11]" << static_cast<int>(request[11]);
    response.resize(2*request[11]+9); // 2*16
    qDebug() << "response.len" << response.size();
    for (uint8_t k = 0;k<8;k++)response[k] = request[k];
    response[5] = 2 * request[11] + 3;
    response[8] = 2 * request[11];//
//    int wdata_len = response[8] + 9;
    qDebug() << "response.len" << response.size();
    for (uint8_t k = 0;k < request[11];k++) {
//        float buf[arr_len];
//        tfs -> read(buf);
        response[2 * k + 9] = 3;//val[2 * k];
        response[2 * k + 10] = 3; //val[2*k+1];
    }

    qDebug() << "tcpServer::send_mklp_data_time: ";
    return 0;
}
int tcpServer::RTC_Time_Install(QByteArray &request, QDataStream &out){
    qDebug() << "tcpServer::RTC_Time_Install";
    return 0;
}
int tcpServer::SetParams(QByteArray &request, QDataStream &out){
    qDebug() << "tcpServer::SetParams";
    return 0;
}
void tcpServer::send_settings(QByteArray &request, QDataStream &out){
    qDebug() << "tcpServer::send_settings";
}

void tcpServer::getChanged(QVector<QString>& VS ){
    if(VS.count() < arr_len) return;
    for(int i=0; i<arr_len; i++){
        QStringList parts = VS[i].split('.');
        int value = parts.at(0).toInt() * 2;
        if (parts.size() > 1 && !parts.at(1).isEmpty()) {
            value += 1;
        }
//        val[2*i] = static_cast<int>(value) >> 8;
//        val[2*i+1] = static_cast<int>(value) & 0x00FF;
    }
}
