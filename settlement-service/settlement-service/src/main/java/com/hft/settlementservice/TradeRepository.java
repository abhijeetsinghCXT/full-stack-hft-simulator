// src/main/java/com/hft/settlementservice/TradeRepository.java
package com.hft.settlementservice;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface TradeRepository extends JpaRepository<TradeEntity, Long> {
    // By extending JpaRepository, we automatically get methods like:
    // save(), findById(), findAll(), delete(), etc.
    // We don't need to write any code here!
}