#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
class tcpServer{
public:
    tcpServer();
    ~tcpServer();
private:
    QTcpServer Svr;
};

#endif // TCPSERVER_H
