#include "tcpServer.h"

tcpServer::tcpServer(QObject *parent)
    : QObject(parent)
,    Svr(new QTcpServer())
{}

tcpServer::~tcpServer(){}
