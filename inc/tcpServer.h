#ifndef TCPSERVER_H
#define TCPSERVER_H
//#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include <QByteArray>


class tcpServer: public QTcpServer{
    Q_OBJECT
    static constexpr int arr_len = 8;
public:
    tcpServer(QObject *parent = nullptr);
    ~tcpServer();
    void incomingConnection(qintptr socketDescriptor) override;
    void getChanged(QVector<QString>&);
private:
    int send_mklp_data_time(QByteArray &request, QByteArray &response);
    int RTC_Time_Install(QByteArray &request, QDataStream &out);
    int SetParams(QByteArray &request, QDataStream &out);
    void send_settings(QByteArray &request, QDataStream &out);
//    QByteArray val{arr_len*2, 4};
};

#endif // TCPSERVER_H
