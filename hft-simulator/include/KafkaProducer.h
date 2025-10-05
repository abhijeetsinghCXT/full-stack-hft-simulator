// include/KafkaProducer.h
#pragma once
#include <string>
#include <rdkafkacpp.h>

class KafkaProducer {
public:
    KafkaProducer(const std::string& brokers);
    ~KafkaProducer();
    void produce(const std::string& message);
    void produce(const std::string& topic, const std::string& message);

private:
    RdKafka::Producer* producer;
};