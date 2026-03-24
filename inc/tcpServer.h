#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
class tcpServer: public QObject{
    Q_OBJECT
public:
    tcpServer(QObject *parent = nullptr);
    ~tcpServer();
private:
    QTcpServer Svr;
};

#endif // TCPSERVER_H
