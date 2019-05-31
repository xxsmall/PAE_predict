#include "kafka_producer.h"

kafka_producer::kafka_producer(QObject *parent) : QObject(parent)
{

    std::string brokers = "kafka1.fat:9092,kafka2.fat:9092,kafka3.fat:9092,kafka4.fat:9092,kafka5.fat:9092,kafka6.fat:9092";
    //std::string brokers = "172.23.4.12:9092,172.23.4.12:9093,172.23.4.12:9094";

    // topic_str="test";
    topic_str="external_measurement_data";

    //topic_str="job_notice";

    partition = RdKafka::Topic::PARTITION_UA;

    conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    conf->set("bootstrap.servers", brokers, errstr);

    ExampleEventCb ex_event_cb;
    conf->set("event_cb", &ex_event_cb, errstr);

    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);

    ExampleDeliveryReportCb ex_dr_cb;
    conf->set("dr_cb", &ex_dr_cb, errstr);

    producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        exit(1);
    }
    std::cout << "% Created producer " << producer->name() << std::endl;

    topic = RdKafka::Topic::create(producer, topic_str,
                                                   tconf, errstr);
    if (!topic) {
        std::cerr << "Failed to create topic: " << errstr << std::endl;
        exit(1);
    }

}


void kafka_producer::sendStringWithKafka(std::string str2)
{

   if( producer )
   {
       delete topic;
       delete producer;
   }

       producer = RdKafka::Producer::create(conf, errstr);
       topic = RdKafka::Topic::create(producer, topic_str,tconf, errstr);

    std::string line;
    line = str2;

    RdKafka::ErrorCode resp =
            producer->produce(topic, partition,
                              RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
                              const_cast<char *>(line.c_str()), line.size(),
                              NULL, NULL);


    if (resp != RdKafka::ERR_NO_ERROR)
        std::cerr << "% Produce failed: " <<
                     RdKafka::err2str(resp) << std::endl;
    else
        std::cerr << "% Produced message (" << line.size() << " bytes)" <<
                     std::endl;

   run_producer = true;


}
