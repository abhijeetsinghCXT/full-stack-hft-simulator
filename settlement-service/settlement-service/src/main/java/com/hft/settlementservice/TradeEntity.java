// src/main/java/com/hft/settlementservice/TradeEntity.java
package com.hft.settlementservice;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import lombok.Data;
import lombok.NoArgsConstructor;

@Entity
@Table(name = "trades") // This class maps to a database table named "trades"
@Data
@NoArgsConstructor
public class TradeEntity {

    @Id // Marks this field as the primary key
    private Long tradeId;
    private Long buyOrderId;
    private Long sellOrderId;
    private Long price;
    private Long quantity;
    private Long timestamp;

    // Constructor to easily convert from a TradeEvent to a TradeEntity
    public TradeEntity(TradeEvent event) {
        this.tradeId = event.getTradeId();
        this.buyOrderId = event.getBuyOrderId();
        this.sellOrderId = event.getSellOrderId();
        this.price = event.getPrice();
        this.quantity = event.getQuantity();
        this.timestamp = event.getTimestamp();
    }
}