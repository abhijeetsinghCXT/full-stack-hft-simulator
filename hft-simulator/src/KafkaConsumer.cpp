// src/KafkaConsumer.cpp
#include "KafkaConsumer.h"
#include "nlohmann/json.hpp"
#include <iostream>

KafkaConsumer::KafkaConsumer(const std::string& brokers, const std::string& topic) {
    std::string errstr;
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("group.id", "matching-engine-group", errstr);

    consumer = RdKafka::KafkaConsumer::create(conf, errstr);
    if (!consumer) {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }
    delete conf;

    consumer->subscribe({topic});
}

// Add this new method's implementation
std::optional<std::string> KafkaConsumer::consumeRaw() {
    RdKafka::Message* msg = consumer->consume(1000);
    if (msg->err() == RdKafka::ERR_NO_ERROR) {
        std::string payload = static_cast<const char*>(msg->payload());
        delete msg;
        return payload;
    }
    delete msg;
    return std::nullopt;
}

KafkaConsumer::~KafkaConsumer() {
    consumer->close();
    delete consumer;
}

std::optional<Order> KafkaConsumer::consume() {
    RdKafka::Message* msg = consumer->consume(1000); // Poll for 1 second
    if (msg->err() == RdKafka::ERR_NO_ERROR) {
        std::string payload = static_cast<const char*>(msg->payload());
        try {
            auto json_order = nlohmann::json::parse(payload);
            Order order;
            order.orderId = std::chrono::system_clock::now().time_since_epoch().count(); // Simple unique ID
            order.price = json_order["price"];
            order.quantity = json_order["quantity"];
            std::string side = json_order["side"];
            order.side = (side == "BUY") ? Side::BUY : Side::SELL;
            delete msg;
            return order;
        } catch (const std::exception& e) {
            std::cerr << "Failed to parse order JSON: " << e.what() << std::endl;
        }
    }
    delete msg;
    return std::nullopt;
}