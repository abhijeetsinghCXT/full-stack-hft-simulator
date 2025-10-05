// src/main/java/com/hft/settlementservice/OrderBookStateRepository.java
package com.hft.settlementservice;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface OrderBookStateRepository extends JpaRepository<OrderBookStateEntity, Long> {
}