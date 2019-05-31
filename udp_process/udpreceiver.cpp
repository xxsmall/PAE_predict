#include "udpreceiver.h"

UdpReceiver::UdpReceiver(QObject *parent) : QObject(parent)
{
    uSocket = new QUdpSocket;
    uSocket->bind(QHostAddress("127.0.0.1"), 2223);
    connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));

}


void UdpReceiver::receive()
{
    QByteArray ba;
       while(uSocket->hasPendingDatagrams())
       {
           ba.resize(uSocket->pendingDatagramSize());
           uSocket->readDatagram(ba.data(), ba.size());
           std::cout << ba.data() << std::endl;

           char* k4 = ba.data(); //dont' use std::string ,it will be crash

           tutorial::ExternalMeasurementDataBean  pbData;
           bool parseFlag = pbData.ParseFromString(k4);
           const tutorial::ParameterBody& recvData = pbData.parameter();

           qDebug()<<"parse falg ===  " <<parseFlag;
           std::cout<< recvData.spatialencoding()<<std::endl;
           std::cout<< recvData.begintime() <<std::endl ;
           std::cout<< recvData.endtime() <<std::endl;
           std::cout<< recvData.devicename() <<std::endl;
           std::string aaa = recvData.begintime();
           std::cout<<"aaa 1="<<aaa <<std::endl;
           aaa = recvData.spatialencoding() + " " + recvData.begintime() +
                  " " + recvData.endtime()+ " " + recvData.devicename();
           std::cout<<"aaa 2="<<aaa <<std::endl;
           emit sendRecvString(QString::fromStdString(aaa));

       }


}
