#ifndef UDPCLENT_H
#define UDPCLENT_H
#include <QThread>
#include <QHostAddress>
#include <QtNetwork/QUdpSocket>
#include <QStringList>
class UDPclent : public QThread
{
public:
    void run();
    void InitSocket();
public:
    void Send();
    bool Read();
    QByteArray Buffer;
    QString information;
    QString email="NULL";
    QString status="0";
    QString info="12345678912345678912345678912345";
private:

    QUdpSocket  *udpSocket;   //套接字对象
    char ToAddress[20]="127.0.0.1";



};

#endif // UDPCLENT_H
