#include "OrderBook.h"
#include <iostream>
#include <algorithm> 
#include "nlohmann/json.hpp" // For creating JSON messages

OrderBook::OrderBook() : kafkaProducer("localhost:9092") {}

void OrderBook::AddOrder(Order order) {
    if (order.side == Side::BUY) {
        bids[order.price].push_back(order);
    } else { // Side::SELL
        asks[order.price].push_back(order);
    }
    MatchOrders();

    // Publish a snapshot of the order book every 50 processed orders
    orderCountSinceLastSnapshot++;
    if (orderCountSinceLastSnapshot >= 1) {
        PublishSnapshot();
        orderCountSinceLastSnapshot = 0;
    }
}

void OrderBook::CancelOrder(uint64_t orderId) {
    std::cout << "CancelOrder function is not yet implemented." << std::endl;
}


void OrderBook::MatchOrders() {
    // Loop as long as there are both bids and asks, and the best bid can cross the best ask
    while (!bids.empty() && !asks.empty() && bids.begin()->first >= asks.begin()->first) {

       

        // Get references to the best bid and ask price levels
        auto& bestBidLevel = bids.begin()->second;
        auto& bestAskLevel = asks.begin()->second;

        // Get references to the orders at the front of the queue (oldest orders)
        Order& bidOrder = bestBidLevel.front();
        Order& askOrder = bestAskLevel.front();

        // Determine the quantity to be traded
        uint32_t tradeQuantity = std::min(bidOrder.quantity, askOrder.quantity);

        // --- TRADE PUBLISHING ---
        nlohmann::json tradeJson;
        tradeJson["tradeId"] = std::chrono::system_clock::now().time_since_epoch().count(); // Use timestamp as unique ID
        tradeJson["buyOrderId"] = bidOrder.orderId;
        tradeJson["sellOrderId"] = askOrder.orderId;
        tradeJson["price"] = askOrder.price;
        tradeJson["quantity"] = tradeQuantity;

        // Produce the message to the "trades" topic
        kafkaProducer.produce(tradeJson.dump());
        std::cout << "PRODUCED trade to Kafka: " << tradeJson.dump() << std::endl;
        
        // In a real system, you would create and publish a Trade object here.
        // Trade trade = { /* ... trade details ... */ };
        // publishTrade(trade);

        // Update the quantities of the matched orders
        bidOrder.quantity -= tradeQuantity;
        askOrder.quantity -= tradeQuantity;

        // If an order is fully filled, remove it from the book
        if (bidOrder.quantity == 0) {
            bestBidLevel.pop_front();
        }
        if (askOrder.quantity == 0) {
            bestAskLevel.pop_front();
        }

        // If a price level is now empty, remove it from the map
        if (bestBidLevel.empty()) {
            bids.erase(bids.begin());
        }
        if (bestAskLevel.empty()) {
            asks.erase(asks.begin());
        }
    }
}

       
// Add this new method's implementation
void OrderBook::RequestSnapshot() {
    kafkaProducer.produce("snapshot-requests", "REQUEST");
}


void OrderBook::PublishSnapshot() {
    nlohmann::json snapshot;

    // Create a JSON array of bids
    for (auto const& [price, order_list] : bids) {
        for (auto const& order : order_list) {
            snapshot["bids"].push_back({
                {"price", order.price},
                {"quantity", order.quantity}
            });
        }
    }// Create a JSON array of asks
    for (auto const& [price, order_list] : asks) {
        for (auto const& order : order_list) {
            snapshot["asks"].push_back({
                {"price", order.price},
                {"quantity", order.quantity}
            });
        }
    }

    // Produce the message to the new "orderbook-snapshots" topic
    kafkaProducer.produce("orderbook-snapshots", snapshot.dump());
    // std::cout << "PRODUCED order book snapshot to Kafka." << std::endl; // Optional: for debugging
}