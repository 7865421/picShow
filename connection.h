#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QThread>
#include <QString>

class Connection : public QThread
{
    Q_OBJECT
signals:
    void ConnectDone(int res);
public:
    Connection(QObject* par);
    ~Connection();

private:
    void run();
    void plc_connect();//连接到PLC

public:

    QString myPLCip;//IP
    int myPLCport;
    int fd;

};




#endif // CONNECTION_H
