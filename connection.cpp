#include "connection.h"
#include "mcprotocol/melsec_mc_bin.h"
#include<QDebug>
Connection::Connection(QObject* par) : QThread(par)
{

}
Connection::~Connection()
{

}

void Connection::plc_connect()
{
    char* plc_ip = myPLCip.toLocal8Bit().data();
    qDebug()<<"IP地址为："<<plc_ip;
    int fd = mc_connect(plc_ip, myPLCport, 0, 0);
        qDebug()<<"连接完成"<<fd;
    emit ConnectDone(fd);

}

void Connection::run()
{
   plc_connect();
}
