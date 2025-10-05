// src/main/java/com/hft/settlementservice/SnapshotRequestListener.java
package com.hft.settlementservice;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.stereotype.Service;

@Service
public class SnapshotRequestListener {

    @Autowired
    private OrderBookStateRepository repository;

    @Autowired
    private KafkaProducerService producerService;

    @KafkaListener(topics = "snapshot-requests", groupId = "snapshot-request-group", containerFactory = "orderBookListenerContainerFactory")
    public void handleSnapshotRequest(String message) {
        System.out.println("Received snapshot request from C++ engine.");

        // Find the last saved state from the database
        repository.findById(1L).ifPresent(state -> {
            System.out.println("Found saved snapshot. Publishing to 'orderbook-snapshots' topic.");
            // Send the saved JSON back to the orderbook-snapshots topic
            producerService.send("orderbook-snapshots", state.getSnapshotJson());
        });
    }
}