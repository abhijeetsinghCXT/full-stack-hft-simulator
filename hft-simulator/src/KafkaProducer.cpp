// src/KafkaProducer.cpp

#include "KafkaProducer.h"
#include <iostream>

KafkaProducer::KafkaProducer(const std::string& brokers) {
    std::string errstr;
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);

    producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        exit(1);
    }
    delete conf;
}

KafkaProducer::~KafkaProducer() {
    producer->flush(10000); // Wait 10s for messages to be sent
    delete producer;
}

// --- REPLACE the old produce method with these two ---
void KafkaProducer::produce(const std::string& message) {
    produce("trades", message); // Default to "trades" topic
}

void KafkaProducer::produce(const std::string& topic, const std::string& message) {
    RdKafka::ErrorCode err = producer->produce(
        topic,
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<char *>(message.c_str()), message.size(),
        nullptr, 0, 0, nullptr);

    if (err != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Failed to produce to topic " << topic << ": " << RdKafka::err2str(err) << std::endl;
    }
}