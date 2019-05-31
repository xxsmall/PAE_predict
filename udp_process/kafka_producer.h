#ifndef KAFKA_PRODUCER_H
#define KAFKA_PRODUCER_H

#include <QObject>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <QDebug>

#include <getopt.h>

#include "rdkafkacpp.h"

static bool run_producer = true;

static void sigterm (int sig) {
    run_producer = false;
}

class kafka_producer : public QObject
{
    Q_OBJECT
public:
    explicit kafka_producer(QObject *parent = 0);

    class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
    public:
        void dr_cb (RdKafka::Message &message) {
            std::cout << "Message delivery for (" << message.len() << " bytes): " <<
                         message.errstr() << std::endl;
            if (message.key())
                std::cout << "Key: " << *(message.key()) << ";" << std::endl;
        }
    };

    class ExampleEventCb : public RdKafka::EventCb {
    public:
        void event_cb (RdKafka::Event &event) {
            switch (event.type())
            {
            case RdKafka::Event::EVENT_ERROR:
                std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " <<
                             event.str() << std::endl;
                if (event.err() == RdKafka::ERR__ALL_BROKERS_DOWN)
                    run_producer = false;
                break;

            case RdKafka::Event::EVENT_STATS:
                std::cerr << "\"STATS\": " << event.str() << std::endl;
                break;

            case RdKafka::Event::EVENT_LOG:
                fprintf(stderr, "LOG-%i-%s: %s\n",
                        event.severity(), event.fac().c_str(), event.str().c_str());
                break;

            default:
                std::cerr << "EVENT " << event.type() <<
                             " (" << RdKafka::err2str(event.err()) << "): " <<
                             event.str() << std::endl;
                break;
            }
        }
    };


   RdKafka::Producer *producer;
   RdKafka::Topic *topic;
   int32_t partition;
   RdKafka::Conf *conf;
   RdKafka::Conf *tconf;
   std::string errstr;
   std::string topic_str;

signals:

public slots:
   void sendStringWithKafka(std::string str2);
};

#endif // KAFKA_PRODUCER_H
