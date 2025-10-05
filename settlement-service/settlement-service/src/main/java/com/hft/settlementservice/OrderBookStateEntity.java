// src/main/java/com/hft/settlementservice/OrderBookStateEntity.java
package com.hft.settlementservice;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import lombok.Data;

@Entity
@Table(name = "order_book_state")
@Data
public class OrderBookStateEntity {

    @Id
    private Long id; // We will always use a fixed ID, like 1

    @Column(columnDefinition = "TEXT") // Use a TEXT column for potentially large JSON
    private String snapshotJson;
}