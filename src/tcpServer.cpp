#include "tcpServer.h"

tcpServer::tcpServer(watch_t& w, params& p, QObject *parent):QTcpServer(parent)
    , watch(w)
    , pars(p)
    , ba({16,0})
{

    if (listen(QHostAddress::Any, 502)) {
        qDebug() << "Server started on port 502";
    }
}

tcpServer::~tcpServer(){}

void tcpServer::incomingConnection(qintptr socketDescriptor){
    QTcpSocket *socket = new QTcpSocket(this);
//    socket->setSocketDescriptor(socketDescriptor);

//    qDebug() << "New connection attempt, socket descriptor:" << socketDescriptor;

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
            request[8] == 0 && request[9] == 0 && request[10] == 0 && request[11] == 16) {
            send_mklp_data_time(request, response);
        }
        else if (request[2] == 0 && request[3] == 0 && request[4] == 0 && request[5] > 0 &&
            request[7] == 0x10 && request[9] == 8 && request[11] == 3){
            RTC_Time_Install(request, response);
        }
        else if (request[0] == 0x47 && request[1] == 0x42 && request[5] == 0x3e && request[7] == 0x10
            && request[8] == 0 && request[9] == 0x33){
            SetParams(request,  response);
        }
        else if (request[0] == 0x47 && request[1] == 0x42 && request[5] == 6 && request[7] == 3
            && request[8] == 0 && request[9] == 0x33){
            send_settings(request,  response);
        }
        else qDebug() << "all the variants missed";
        socket->write(response);
    });

    // --- (Tear down) ---
    connect(socket, &QTcpSocket::disconnected, [socket]() {
        socket->deleteLater();
    });
}

#define TO_BCD(val) (static_cast<uchar>((( (val) / 10) << 4) | ((val) % 10)))

int tcpServer::send_mklp_data_time(QByteArray &request, QByteArray &response){
    response.resize(2*request[11]+9); // 2*16

    for (uint8_t k = 0;k<8;k++)response[k] = request[k];
    response[5] = 2 * request[11] + 3;
    response[8] = 2 * request[11];

        for(int k=0;k<8;k++) {
            QMutexLocker locker(&mx);
            response[2*k + 9] = ba[2*k]; //3;
            response[2*k + 10] = ba[2*k+1]; //3;
        }
        QDateTime dt;
        watch.get_watch(dt);
        QDate d = dt.date();
        QTime t = dt.time();
//        uchar tmp = t.second();
        response[25] = TO_BCD(t.second());
        response[26] = TO_BCD(t.minute());
        response[27] = TO_BCD(t.hour());
        response[28] = TO_BCD(d.day());
        response[29] = TO_BCD(d.month());
        response[30] = TO_BCD(d.year()%100);
    return 0;
}
int tcpServer::RTC_Time_Install(QByteArray &request, QByteArray &response){
    int year = (request[18] >> 4) * 10 + request[18] & 0xf + 100;
    int mon = ((request[17] & 0x1f) >> 4) * 10 + request[17] & 0xf;
    int mday = (request[16] >> 4) * 10 + request[16] & 0xf;

    int hour = (request[15] >> 4) * 10 + request[15] & 0xf;
    int min = (request[14] >> 4) * 10 + request[14] & 0xf;
    int sec = (request[13] >> 4) * 10 + request[13] & 0xf;

    QDateTime rdt(QDate(year, mon, mday), QTime(hour, min, sec));
    watch.set_watch(rdt);
    response.resize(12);
    for(int k=0;k<12;k++)
        response[k]=request[k];
        response[5]=6;
    return 0;
}
int tcpServer::SetParams(QByteArray &request, QByteArray &response){
    pars.set_params(request);

    response.resize(4);
    response[0] = 0;
    response[1] = 'O';
    response[2] = 'K';
    response[3] = '!';

//    qDebug() << "tcpServer::SetParams";
    return 0;
}
void tcpServer::send_settings(QByteArray &request, QByteArray &response){
    pars.get_params(response);
}

void tcpServer::getChanged(QVector<QString>& VS ){
    if(VS.count() < arr_len) return;
    for(int i=0; i<arr_len; i++){
        QStringList parts = VS[i].split(',');
        int value = parts.at(0).toInt() * 2;
        if (parts.size() > 1 && !parts.at(1).isEmpty()) {
            value += 1;
        }
        {
            QMutexLocker locker(&mx);
            ba[2*i] = static_cast<uint>(value) >> 8;
            ba[2*i+1] = static_cast<uint>(value) & 0x00FF;
        }
    }
}
