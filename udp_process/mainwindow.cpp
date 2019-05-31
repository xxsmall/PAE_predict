#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QDebug>
#include "func.h"
#include "udpreceiver.h"
#include "external_measurement_data.pb.h"

using namespace std;

int yyyy(std::string strTLE1,std::string strTLE2,int year2,int month2,int day2,int hour2,int minute2,double second2,
         double &P,double &A,double &E,double &V,double alt,double lat,double lon);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("测站PAE仿真");
    readStationConfig();

    UdpReceiver *updObj = new UdpReceiver(nullptr);
    connect(updObj,SIGNAL(sendRecvString(QString)),this,SLOT(process_predict_para(QString)));// 响应结果发送给生产者

    producer = new kafka_producer(NULL);
    connect(this,SIGNAL(sendPredictString(std::string)),producer,SLOT(sendStringWithKafka(std::string))); //生产者发送消息

    predictRsultXml ="";
    ////
//    QDateTime startT,endT;
//    QDate kDate(2019,4,2);
//    QTime kTime(16,19,0);
//    startT.setDate(kDate);
//    startT.setTime(kTime);
//    QDate kDate2(2019,4,2);
//    QTime kTime2(16,25,0);
//    endT.setDate(kDate2);
//    endT.setTime(kTime2);

//    cal_predict("43837",startT,endT,"QSX73C");
    qDebug()<<"main end";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
       std::string strTLE1;
       std::string strTLE2;
       int year2;
       int month2;
       int day2;
       int hour2;
       int minute2;
       double second2;
       double P;
       double A;
       double E;
       double V;
       double alt;
       double lat;
       double lon;

       strTLE1 = "1 43837U 18102G   19091.83896422  .00000688  00000-0  47676-4 0  9995";
       strTLE2 = "2 43837  97.6109 168.4497 0014039 242.8459 254.4839 15.08864636 17436";
       year2 = 2019;
       month2 =4;
       day2=3;
       hour2 =10;
       minute2 =0;
       second2 = 0.0;
       P=0;
       A=0;
       E=0;
       V=0;
       alt = 697.0;
       lat = 34.87;
       lon = 108.95;
       QDateTime dateTemp;
       QDate kDate(year2,month2,day2);
       QTime kTime(hour2,minute2,(int)second2);
       dateTemp.setDate(kDate);
       dateTemp.setTime(kTime);
       qDebug()<<dateTemp;
       QList<QString> needWritList;
       needWritList.clear();

       QDateTime datePredict;
       QString strEnd = "\r\n";
       QString str2;

       str2 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + strEnd;
       needWritList.append(str2);

       str2 = "<root>" + strEnd;
       needWritList.append(str2);

       str2 = "  <Satellite>" + strEnd;
       needWritList.append(str2);

       str2 = "    <satID> " + QString::number(43837) + " </satID>"+ strEnd;
       needWritList.append(str2);

       str2 = "    <stationName>  QSX73C </stationName>"+ strEnd;
       needWritList.append(str2);


       str2 = "    <Longitude> " + QString::number(lon,'f',6) + " </Longitude>"+ strEnd;
       needWritList.append(str2);

       str2 = "    <Latitude> " + QString::number(lat,'f',6) + " </Latitude>"+ strEnd;
       needWritList.append(str2);

       str2 = "    <Height> " + QString::number(alt,'f',6) + " </Height>"+ strEnd;
       needWritList.append(str2);


       str2 = "  </Satellite>" + strEnd;
       needWritList.append(str2);


       for(int i=0;i<3600*15;i++)
       {
            datePredict = dateTemp.addSecs(i);
            int  a ;
            int year3 = datePredict.date().year();
            int month3 = datePredict.date().month();
            int day3 = datePredict.date().day();
            int hour3 = datePredict.time().hour();
            int minute3 = datePredict.time().minute();
            double second3 = datePredict.time().second();
            a = yyyy(strTLE1,strTLE2,year3,month3,day3,hour3,minute3,second3,P,A,E,V,alt,lat,lon);

            str2 = "  <Data>" + strEnd;
            needWritList.append(str2);

            str2 = "    <Time> " + datePredict.toString("yyyy-M-d h:m:s.zzz") + " </Time>"+ strEnd;
            needWritList.append(str2);

            str2 = "    <P> " + QString::number(P,'f',6)+" </P>"+ strEnd;
            needWritList.append(str2);

            str2 = "    <A> " + QString::number(A,'f',6)+" </A>"+ strEnd;
            needWritList.append(str2);

            str2 = "    <E> " + QString::number(E,'f',6)+" </E>"+ strEnd;
            needWritList.append(str2);

            str2 = "    <Pdot> " +   QString::number(V,'f',6)+" </Pdot>"+ strEnd;
            needWritList.append(str2);

            str2 = "  </Data>" + strEnd;
            needWritList.append(str2);

       }

       str2 = "</root>" + strEnd;
       needWritList.append(str2);

       QString fileName = "./outPut.xml";
       func *funcObj = new func(NULL);
       funcObj->writeFile(needWritList,fileName);
       delete  funcObj;
       QMessageBox msgBox;
       msgBox.setWindowTitle("Predict Result");
       msgBox.setText("predict end ,\"outPut.xml\" is station predict file !");
       msgBox.exec();
}

void MainWindow::readStationConfig()
{
    stationList.clear();
    func  *funObj = new func(nullptr);
    QList<QString> stationTxtList;
    stationTxtList.clear();
    stationTxtList =  funObj->readFile("./station.txt");
    int size = stationTxtList.size();
    for(int i=0;i<size;i++)
    {
        QString strLine = stationTxtList[i];
        if(strLine.contains("#"))
        {
            continue ;
        }else
        {
            strLine = funObj->removeMoreSpace(strLine);
            strLine = strLine.trimmed();
            QStringList paraList = strLine.split(" ");
            ttcStationDesc stationObj;
            int sizePara = paraList.size();
            if(sizePara == 4)
            {
                stationObj.name = paraList[0];
                stationObj.altitude = paraList[1];
                stationObj.latitude = paraList[2];
                stationObj.longitude = paraList[3];
                stationList.append(stationObj);
            }
        }


    }

    delete  funObj;

    size = stationList.size();
    for(int i=0;i<size;i++)
    {
        qDebug()<<stationList[i].name<<stationList[i].altitude<<stationList[i].latitude<<stationList[i].longitude;
    }


}

void MainWindow::cal_predict(QString id, QDateTime startDate, QDateTime endDate, QString stationName)
{
     func *funObj = new func(nullptr);
     QList<QString> tleTxtList;
     tleTxtList.clear();
     tleTxtList =  funObj->readFile("./tle.txt");
     QString tleID =id;
     tleID = tleID.toUpper();
     tleID = tleID.trimmed();
     int sizeStation = stationList.size();
     QString stationAltitude,stationLatitude,stationLongitude;
     bool findStation = false;
     for(int i=0;i<sizeStation;i++)
     {
         if(stationList[i].name == stationName)
         {
             stationAltitude = stationList[i].altitude;
             stationLatitude = stationList[i].latitude;
             stationLongitude = stationList[i].longitude;
             findStation = true;
         }
     }

     if(!findStation)
     {
         qDebug()<<"not find station named ="<<stationName;
         return ;
     }

     int sizeTLETxt = tleTxtList.size();
     bool findTle =false;
     QString strTLE1,strTLE2;
     for(int i=0;i<sizeTLETxt;i++)
     {
         QString tempID = tleID;
         if(tleID.length()<5)
         {
             tempID = " " + tleID ;
         }

         if(tleTxtList[i].contains(tempID + "U"))
         {
            strTLE1 = tleTxtList[i];
            strTLE2 = tleTxtList[i+1];
            findTle = true;
            break ;
         }

     }

     if(!findTle)
     {
         qDebug()<<"not find TLE = "<<tleID;
         return ;
     }

     double epochMSStart = startDate.toMSecsSinceEpoch();
     double epochMSEnd = endDate.toMSecsSinceEpoch();
     double timeSpan = (epochMSEnd - epochMSStart)/1000.0;

     cal_interface(tleID,stationName,stationAltitude.toDouble(),stationLatitude.toDouble(),stationLongitude.toDouble(),strTLE1,strTLE2,startDate,timeSpan);

}

void MainWindow::cal_interface(QString TleID,QString stationName, double altitude, double latitude, double longitude, QString TleLine1, QString TleLine2,
                   QDateTime startDate, double timeSpan)
{

    std::string strTLE1 = TleLine1.toStdString();
    std::string strTLE2 = TleLine2.toStdString();
    int year2;
    int month2;
    int day2;
    int hour2;
    int minute2;
    double second2;
    double P;
    double A;
    double E;
    double V;
    double alt;
    double lat;
    double lon;
    double ms;

    year2 = startDate.date().year();
    month2 =startDate.date().month();
    day2=startDate.date().day();
    hour2 =startDate.time().hour();
    minute2 =startDate.time().minute();
    second2 = startDate.time().second();
    ms = startDate.time().msec();
    second2 = ms/1000.0 + second2;
    P=0;
    A=0;
    E=0;
    V=0;
    alt = altitude;
    lat = latitude;
    lon = longitude;
    QDateTime dateTemp;
    QDate kDate(year2,month2,day2);
    QTime kTime(hour2,minute2,(int)second2);
    dateTemp.setDate(kDate);
    dateTemp.setTime(kTime);
    qDebug()<<dateTemp;
    QList<QString> needWritList;
    needWritList.clear();

    QDateTime datePredict;
    QString strEnd = "\r\n";
    QString str2;

    str2 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + strEnd;
    needWritList.append(str2);

    str2 = "<root>" + strEnd;
    needWritList.append(str2);

    str2 = "  <Satellite>" + strEnd;
    needWritList.append(str2);

    str2 = "    <satID> " + TleID + " </satID>"+ strEnd;
    needWritList.append(str2);

    str2 = "    <stationName> " + stationName+ "</stationName>"+ strEnd;
    needWritList.append(str2);


    str2 = "    <Longitude> " + QString::number(lon,'f',6) + " </Longitude>"+ strEnd;
    needWritList.append(str2);

    str2 = "    <Latitude> " + QString::number(lat,'f',6) + " </Latitude>"+ strEnd;
    needWritList.append(str2);

    str2 = "    <Height> " + QString::number(alt,'f',6) + " </Height>"+ strEnd;
    needWritList.append(str2);


    str2 = "  </Satellite>" + strEnd;
    needWritList.append(str2);


    for(int i=0;i<timeSpan;i++) //unit second
    {
         datePredict = dateTemp.addSecs(i);
         int  a ;
         int year3 = datePredict.date().year();
         int month3 = datePredict.date().month();
         int day3 = datePredict.date().day();
         int hour3 = datePredict.time().hour();
         int minute3 = datePredict.time().minute();
         double second3 = datePredict.time().second();
         a = yyyy(strTLE1,strTLE2,year3,month3,day3,hour3,minute3,second3,P,A,E,V,alt,lat,lon);

         str2 = "  <Data>" + strEnd;
         needWritList.append(str2);

         str2 = "    <Time> " + datePredict.addSecs(8*3600).toString("yyyy-MM-dd hh:mm:ss.zzz") + " </Time>"+ strEnd;
         needWritList.append(str2);

         str2 = "    <P> " + QString::number(P,'f',6)+" </P>"+ strEnd;
         needWritList.append(str2);

         str2 = "    <A> " + QString::number(A,'f',6)+" </A>"+ strEnd;
         needWritList.append(str2);

         str2 = "    <E> " + QString::number(E,'f',6)+" </E>"+ strEnd;
         needWritList.append(str2);

         str2 = "    <Pdot> " +   QString::number(V,'f',6)+" </Pdot>"+ strEnd;
         needWritList.append(str2);

         str2 = "  </Data>" + strEnd;
         needWritList.append(str2);

    }

    str2 = "</root>";
    needWritList.append(str2);

    QString writAllString = "";
    int sizeFinal = needWritList.size();
    for(int j=0;j<sizeFinal;j++)
    {
        writAllString = writAllString + needWritList[j];
    }

    predictRsultXml = writAllString;
//    QByteArray array = writAllString.toUtf8();

//    if(1)
//    {
//        hdfsFS fs = hdfsConnect("172.23.5.1", 9000);//defaultÎª±¾µØÎÄ¼þÏµÍ³£¬ipµØÖ·ÎªHDFSÏµÍ³
//        const char* writePath = "/temp/outPut.xml";//
//        hdfsFile writeFile = hdfsOpenFile(fs, writePath, O_WRONLY|O_CREAT, 0, 0, 0);
//        if(!writeFile)
//        {
//              fprintf(stderr, "Failed to open %s for writing!\n", writePath);
//              return ;
//        }

//        char* buffer = array.data();
//        tSize num_written_bytes = hdfsWrite(fs, writeFile, (void*)buffer, strlen(buffer)+1);
//        if (hdfsFlush(fs, writeFile))
//        {
//             fprintf(stderr, "Failed to 'flush' %s\n", writePath);
//             return ;
//        }

//        hdfsCloseFile(fs, writeFile);
//     }

}

void MainWindow::process_predict_para(QString predictStrPara)
{
     //tutorial::ExternalMeasurementDataBean pbData;
     qDebug()<<"run here 1:   "<<predictStrPara;
     QStringList paraList = predictStrPara.split(" ");
     if(paraList.size()>=6)
     {
         QString id = paraList[0];
         QString date1Str = paraList[1];
         QString time1Str = paraList[2];
         QString date2Str = paraList[3];
         QString time2Str = paraList[4];
         QString deviceName = paraList[5];

         QDateTime startT,endT;
         QDate kDate;
         QTime kTime;
         kDate = kDate.fromString(date1Str,"yyyy-MM-dd");
         kTime = kTime.fromString(time1Str,"hh:mm:ss");
         startT.setDate(kDate);
         startT.setTime(kTime);

         QDate kDate2;
         QTime kTime2;
         kDate2 = kDate2.fromString(date2Str,"yyyy-MM-dd");
         kTime2 = kTime2.fromString(time2Str,"hh:mm:ss");
         endT.setDate(kDate2);
         endT.setTime(kTime2);

         startT = startT.addSecs(-8*3600);
         endT = endT.addSecs(-8*3600);

         cal_predict(id,startT,endT,deviceName);
         if(!predictRsultXml.isEmpty())
         {
             tutorial::ExternalMeasurementDataBean  pbData;
             tutorial::PredictDataBody *pbPredictResult = new tutorial::PredictDataBody();
             pbPredictResult->set_datastr(predictRsultXml.toStdString());


             pbData.set_allocated_predictdata(pbPredictResult);

             std::string buff;
             pbData.SerializeToString(&buff); //must use "SerializeToString" function


             emit sendPredictString(buff);
         }

         qDebug()<<"hdfs not write ,use kafka xml end !";
     }
}
