#ifndef FUNC_H
#define FUNC_H

#include <QMainWindow>
#include <QDebug>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>
#include <math.h>


#include <stdio.h>
#include <string.h>

//#include "Constant.h"


struct  deviceDeviatDesc
{

    QString deviceNum;
    QString A_measure;
    QString A_sys;
    QString E_measure;
    QString E_sys;
    QString speed_measure;
    QString speed_sys;
    QString range_measure;
    QString range_sys;

};

struct  ttcStationDesc
{
    QString name;       //测站名称
    QString code;       //测站编码
    QString longitude;  //经度
    QString latitude;   //纬度
    QString altitude;   //高程
    QString X;          //笛卡尔
    QString Y;          //笛卡尔
    QString Z;          //笛卡尔
    QString sigmaD;     //测速精度
    QString sigmaR;     //测距精度
    QString sigmaA;     //方位精度
    QString sigmaE;     //俯仰精度
};

struct   orbit_GKCL_Desc
{
    QString year;
    QString month;
    QString day;
    QString hour;
    QString minute;
    QString second;
    QString a;
    QString e;
    QString i;
    QString Q;
    QString w;
    QString M;
    QString damp;
    QString sun;

};

struct  xyzClass
{
    double x;
    double y;
    double z;
    double vx;
    double vy;
    double vz;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;

};


namespace Ui {
class func;
}

class func : public QMainWindow
{
    Q_OBJECT

public:
    explicit func(QWidget *parent = 0);
    ~func();

    void               writeFile(QList<QString> textList,QString fileName);//写入文件
    QList<QString>     readFile(QString fileName);//按行读取文件，形成一个QList<QString>
    QString            removeMoreSpace(QString str); //将每行连续的空格变为单个空格
    QString            strToNwidth(int needWidth, QString addDirection, QString oldStr);
    QString            getTableData(int row,int col ,QAbstractItemModel* modelTemp);
    QString            getNotepadPath();
    double             jde(int Y,int M,int D,int hour,int min,float sec);
    QDate              getDateFromStr(QString year,QString month,QString day);//转成QDate类型
    QTime              getTimeFromStr(QString hour,QString min,QString second);//转成QTime类型，秒是带.需要特别处理
    QString            removeNotNumberChar(QString strOld);
    double             getAngle(double source);
    QDateTime          getTleTime(QString tleLine1);
    double             getTleMJD(QString tleLine1);
    qint64             getDateTimeMs(QDateTime startTime,QDateTime endTime);
private:
    Ui::func *ui;
};

#endif // FUNC_H
