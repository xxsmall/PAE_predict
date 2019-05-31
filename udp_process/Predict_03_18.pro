#-------------------------------------------------
#
# Project created by QtCreator 2019-03-18T14:35:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Predict_03_18
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    func.cpp \
    kafka_producer.cpp \
    external_measurement_data.pb.cc \
    udpreceiver.cpp

HEADERS += \
        mainwindow.h \
    func.h \
    kafka_producer.h \
    external_measurement_data.pb.h \
    udpreceiver.h

FORMS += \
        mainwindow.ui \
    func.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L/usr/local/lib/ -lTEST

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
unix:!macx: LIBS += -L/home/hadoop-3.1.0/lib/native/ -lhdfs

INCLUDEPATH += /home/hadoop-3.1.0/include
DEPENDPATH += /home/hadoop-3.1.0/include
INCLUDEPATH += /usr/java/jdk1.8.0_162/include
INCLUDEPATH += /usr/java/jdk1.8.0_162/include/linux



unix:!macx: LIBS += -L/usr/java/jdk1.8.0_162/jre/lib/amd64/server/ -ljvm

INCLUDEPATH += /usr/java/jdk1.8.0_162/jre/lib/amd64/server
DEPENDPATH += /usr/java/jdk1.8.0_162/jre/lib/amd64/server

INCLUDEPATH += /usr/local/include/librdkafka
LIBS += -L/usr/local/lib -lrdkafka
LIBS += -L/usr/local/lib -lrdkafka++

LIBS +=-lpthread
unix:!macx: LIBS += -L/usr/local/lib/ -lprotobuf

INCLUDEPATH += /usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include
