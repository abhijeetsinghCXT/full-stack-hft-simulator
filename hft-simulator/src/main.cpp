// src/main.cpp
#include <iostream>
#include "OrderBook.h"
#include "KafkaConsumer.h"
#include "nlohmann/json.hpp"
#include <thread> // For std::this_thread::sleep_for

int main() {
    OrderBook orderBook;

    // --- STARTUP: REQUEST AND LOAD SNAPSHOT ---
    std::cout << "Requesting initial order book snapshot from Java service..." << std::endl;
    orderBook.RequestSnapshot();

    // Create a temporary consumer to get the snapshot response
    KafkaConsumer snapshotConsumer("localhost:9092", "orderbook-snapshots");
    std::optional<std::string> snapshotPayload;

    // Poll for up to 20 seconds to get the snapshot
    for (int i = 0; i < 20; ++i) {
        snapshotPayload = snapshotConsumer.consumeRaw();
        if (snapshotPayload)
            break;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (snapshotPayload) {
        std::cout << "Received snapshot. Loading initial order book state..." << std::endl;
        try {
            auto snapshot = nlohmann::json::parse(*snapshotPayload);
            if (snapshot.contains("bids")) {
                for (const auto& bid : snapshot["bids"]) {
                    orderBook.AddOrder({0, bid["price"], bid["quantity"], Side::BUY});
                }
            }
            if (snapshot.contains("asks")) {
                for (const auto& ask : snapshot["asks"]) {
                    orderBook.AddOrder({0, ask["price"], ask["quantity"], Side::SELL});
                }
            }
            std::cout << "Initial state loaded successfully." << std::endl;
        } catch(const std::exception& e) {
            std::cerr << "Failed to parse snapshot JSON: " << e.what() << std::endl;
        }
    } else {
        std::cout << "No snapshot received. Starting with an empty order book." << std::endl;
    }

    // --- MAIN LOOP: PROCESS NEW ORDERS ---
    KafkaConsumer orderConsumer("localhost:9092", "orders");
    std::cout << " Matching Engine Initialized and listening for new orders..." << std::endl;

    while (true) {
        auto orderOpt = orderConsumer.consume();
        if (orderOpt) {
            std::cout << "Received order: " << (orderOpt->side == Side::BUY ? "BUY" : "SELL") 
                      << " " << orderOpt->quantity << " @ " << orderOpt->price << std::endl;
            orderBook.AddOrder(*orderOpt);
        }
    }
    return 0;
}