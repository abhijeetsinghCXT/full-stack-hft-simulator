#pragma once

#include "datastructures.h"
#include <map>
#include <list>
#include <functional> // For std::greater
#include "KafkaProducer.h" // Add this include

class OrderBook {

    
public:

    OrderBook(); // Add a constructor

    // The main function to add a new order and trigger matching
    void AddOrder(Order order);

    void RequestSnapshot();

    // Function to cancel an existing order (we'll implement this later)
    void CancelOrder(uint64_t orderId);

    void PublishSnapshot(); 

private:
    // The core matching logic that checks for and executes trades
    void MatchOrders();

    KafkaProducer kafkaProducer; // Kafka producer instance

private:
    // Container for buy orders (bids), sorted from highest to lowest price
    std::map<uint64_t, std::list<Order>, std::greater<uint64_t>> bids;

    // Container for sell orders (asks), sorted from lowest to highest price
    std::map<uint64_t, std::list<Order>> asks;

    int orderCountSinceLastSnapshot = 0;
};