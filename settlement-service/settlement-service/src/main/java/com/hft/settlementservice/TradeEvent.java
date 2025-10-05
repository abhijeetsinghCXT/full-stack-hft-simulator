// src/main/java/com/hft/settlementservice/TradeEvent.java
package com.hft.settlementservice;

// Lombok annotations to automatically create getters, setters, constructors
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class TradeEvent {
    private long tradeId;
    private long buyOrderId;
    private long sellOrderId;
    private long price;
    private long quantity;
    private long timestamp;
}