#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <iostream>
#include <sys/time.h>
#include <getopt.h>
#include <unistd.h>
#include <QTimer>
#include <sstream>

#include <ctime>
#include <google/protobuf/util/time_util.h>


#include "external_measurement_data.pb.h"

class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UdpReceiver(QObject *parent = 0);

signals:
    void sendRecvString(QString strResault);
public slots:
    void receive();

private:
    QUdpSocket *uSocket;


};

#endif // UDPRECEIVER_H
