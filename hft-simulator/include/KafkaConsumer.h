// include/KafkaConsumer.h
#pragma once
#include "datastructures.h"
#include <string>
#include <rdkafkacpp.h>
#include <optional>

class KafkaConsumer {
public:
    KafkaConsumer(const std::string& brokers, const std::string& topic);
    ~KafkaConsumer();
    std::optional<Order> consume();
    // Add this new public method
std::optional<std::string> consumeRaw();

private:
    RdKafka::KafkaConsumer* consumer;
};