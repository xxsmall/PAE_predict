#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <hdfs.h>

#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <iostream>
#include <sys/time.h>
#include <getopt.h>
#include <unistd.h>
#include <sstream>

#include <ctime>
#include <google/protobuf/util/time_util.h>

#include "kafka_producer.h"
#include "func.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QList<ttcStationDesc> stationList;
    kafka_producer *producer;
    QString predictRsultXml;

    void readStationConfig();
    void cal_predict(QString id,QDateTime startDate,QDateTime endDate,QString stationName);
    void cal_interface(QString TleID,QString stationName,double altitude,double latitude,double longitude,QString TleLine1,QString TleLine2,
                       QDateTime startDate,double timeSpan);
signals:
    void sendPredictString(std::string strSend);
public slots:
    void process_predict_para(QString predictStrPara);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
