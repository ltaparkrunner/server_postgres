#ifndef TCPSERVER_H
#define TCPSERVER_H
//#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include <QByteArray>
#include <params_watch.h>
#include <QMutex>

class tcpServer: public QTcpServer{
    Q_OBJECT
    static constexpr int arr_len = 8;
public:
    tcpServer(watch_t& w, params& p, QObject *parent = nullptr);
    ~tcpServer();
    void incomingConnection(qintptr socketDescriptor) override;
    void getChanged(QVector<QString>& vs);
private:
    int send_mklp_data_time(QByteArray &request, QByteArray &response);
    int RTC_Time_Install(QByteArray &request, QByteArray &response);
    int SetParams(QByteArray &request, QByteArray &response);
    void send_settings(QByteArray &request, QByteArray &response);

//    void send_OK(){}
    watch_t watch;
    params pars;
    QByteArray ba;
    QMutex  mx;
};

#endif // TCPSERVER_H
