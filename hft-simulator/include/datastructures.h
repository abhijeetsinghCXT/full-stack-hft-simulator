#pragma once // Prevents the file from being included multiple times

#include <cstdint>

// A type-safe enum to represent the side of an order
enum class Side {
    BUY,
    SELL
};

// Represents a single order in the order book
struct Order {
    uint64_t orderId;
    uint64_t price;     // Use integer for price to avoid floating-point issues. E.g., 10050 represents $100.50
    uint32_t quantity;
    Side side;
};

// Represents a trade that occurred from a match
struct Trade {
    uint64_t tradeId;
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    uint64_t price;
    uint32_t quantity;
    uint64_t timestamp; // timestamp of the trade
};