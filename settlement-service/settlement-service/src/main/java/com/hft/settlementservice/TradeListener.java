// src/main/java/com/hft/settlementservice/TradeListener.java
package com.hft.settlementservice;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Service;

@Service
public class TradeListener {

    @Autowired
    private TradeRepository tradeRepository;

    // This annotation marks this method as a Kafka message listener
    @KafkaListener(topics = "trades", groupId = "settlement-group", containerFactory = "tradeEventListenerContainerFactory")
    public void listenToTrades(TradeEvent tradeEvent) {
        System.out.println("Received Trade Event: " + tradeEvent.getTradeId());

        // 1. Convert the incoming event DTO to a database entity
        TradeEntity tradeEntity = new TradeEntity(tradeEvent);

        // 2. Use the repository to save the entity to the PostgreSQL database
        tradeRepository.save(tradeEntity);

        System.out.println("Successfully saved Trade " + tradeEntity.getTradeId() + " to the database.");
    }
}