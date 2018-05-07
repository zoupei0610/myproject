#include "udpclent.h"

void UDPclent::run()
{

}

void UDPclent::InitSocket()
{
   udpSocket = new QUdpSocket(this);
}

void UDPclent::Send()
{
    information=QString("##%1;S=%2;w=%3#").arg(email).arg(status).arg(info);
    QByteArray info_byte =information.toLatin1();
    udpSocket->writeDatagram(info_byte,QHostAddress(ToAddress),8099);
}

bool UDPclent::Read()
{
    int ms = 0;

    while (!(udpSocket->hasPendingDatagrams())) {

            ms++;
            if(ms > 100)
            {
                return false;
            }
            sleep(1);
       }

    Buffer.resize(udpSocket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    udpSocket->readDatagram(Buffer.data(), Buffer.size(), &sender, &senderPort);
    qDebug() << "Message from:" << sender.toString();
    qDebug() << "Message port:" << senderPort;
    qDebug() << "Message:" << Buffer;
    return true;
}
