#include "tcpServer.h"

tcpServer::tcpServer():
    Svr(new QTcpServer())
{}

tcpServer::~tcpServer(){}
