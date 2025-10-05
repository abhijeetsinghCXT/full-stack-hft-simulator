// src/main/java/com/hft/settlementservice/OrderBookListener.java
package com.hft.settlementservice;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Service;

@Service
public class OrderBookListener {

    @Autowired
    private OrderBookStateRepository repository;

    private final ObjectMapper objectMapper = new ObjectMapper();

    @KafkaListener(topics = "orderbook-snapshots", groupId = "orderbook-group", containerFactory = "orderBookListenerContainerFactory")
    public void listenToOrderBook(String message) {
        System.out.println("Received order book snapshot.");

        // Find the existing state by its ID (1L), or create a new one if it doesn't exist
        OrderBookStateEntity state = repository.findById(1L)
                .orElse(new OrderBookStateEntity());

        state.setId(1L); // Ensure ID is set, especially for a new entity
        state.setSnapshotJson(message);

        repository.save(state); // This will now perform an UPDATE if the row exists
        System.out.println("Successfully saved/updated order book snapshot in the database.");
    }
}