// src/main/java/com/hft/settlementservice/KafkaConsumerConfig.java
package com.hft.settlementservice;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.common.serialization.StringDeserializer;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.kafka.config.ConcurrentKafkaListenerContainerFactory;
import org.springframework.kafka.core.ConsumerFactory;
import org.springframework.kafka.core.DefaultKafkaConsumerFactory;
import org.springframework.kafka.support.serializer.JsonDeserializer;

import java.util.HashMap;
import java.util.Map;

@Configuration
public class KafkaConsumerConfig {

    @Value("${spring.kafka.consumer.bootstrap-servers}")
    private String bootstrapServers;

    // --- Factory for TradeEvent messages ---
    @Bean
    public ConsumerFactory<String, TradeEvent> tradeEventConsumerFactory() {
        Map<String, Object> props = new HashMap<>();
        props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
        // Configure the JsonDeserializer for TradeEvent
        JsonDeserializer<TradeEvent> deserializer = new JsonDeserializer<>(TradeEvent.class);
        deserializer.setRemoveTypeHeaders(false);
        deserializer.addTrustedPackages("*");
        deserializer.setUseTypeMapperForKey(true);

        return new DefaultKafkaConsumerFactory<>(props, new StringDeserializer(), deserializer);
    }

    @Bean
    public ConcurrentKafkaListenerContainerFactory<String, TradeEvent> tradeEventListenerContainerFactory() {
        ConcurrentKafkaListenerContainerFactory<String, TradeEvent> factory = new ConcurrentKafkaListenerContainerFactory<>();
        factory.setConsumerFactory(tradeEventConsumerFactory());
        return factory;
    }

    // --- Factory for simple String messages (for the order book) ---
    @Bean
    public ConsumerFactory<String, String> stringConsumerFactory() {
        Map<String, Object> props = new HashMap<>();
        props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
        return new DefaultKafkaConsumerFactory<>(props, new StringDeserializer(), new StringDeserializer());
    }

    @Bean
    public ConcurrentKafkaListenerContainerFactory<String, String> orderBookListenerContainerFactory() {
        ConcurrentKafkaListenerContainerFactory<String, String> factory = new ConcurrentKafkaListenerContainerFactory<>();
        factory.setConsumerFactory(stringConsumerFactory());
        return factory;
    }
}