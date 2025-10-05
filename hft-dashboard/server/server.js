// server/server.js
const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const { Kafka } = require('kafkajs');
const cors = require('cors');

const app = express();
app.use(cors()); // Enable CORS for all routes

const server = http.createServer(app);
const io = new Server(server, {
    cors: {
        origin: "*", // Allow connections from any origin
    }
});

const PORT = 4000;

// --- Kafka Setup ---
const kafka = new Kafka({
    clientId: 'dashboard-kafka-client',
    brokers: ['localhost:9092']
});

const consumer = kafka.consumer({ groupId: 'dashboard-group' });
const producer = kafka.producer(); // <-- ADD A KAFKA PRODUCER

const runKafkaConsumer = async () => {
    await consumer.connect();
    await producer.connect();
    // Subscribe to both topics
    await consumer.subscribe({ topic: 'trades', fromBeginning: true });
    await consumer.subscribe({ topic: 'orderbook-snapshots', fromBeginning: true });
    console.log("Kafka consumer connected and subscribed to topics.");

    await consumer.run({
        eachMessage: async ({ topic, partition, message }) => {
            const payload = JSON.parse(message.value.toString());

            // Check which topic the message came from
            if (topic === 'trades') {
                console.log('Received trade from Kafka:', payload);
                io.emit('new-trade', payload);
            } else if (topic === 'orderbook-snapshots') {
                console.log('Received order book snapshot from Kafka');
                io.emit('orderbook-update', payload);
            }
        },
    });
};

runKafkaConsumer().catch(e => console.error("Kafka consumer error:", e));


// --- WebSocket (Socket.io) Setup ---
io.on('connection', (socket) => {
    console.log('A user connected with socket id:', socket.id);

    // Listen for new orders from the client
    socket.on('new-order', async (order) => {
        console.log('Received new order from web client:', order);
        try {
            await producer.send({
                topic: 'orders',
                messages: [{ value: JSON.stringify(order) }],
            });
            console.log('Forwarded order to Kafka "orders" topic.');
        } catch (e) {
            console.error('Failed to forward order to Kafka:', e);
        }
    });
    
    socket.on('disconnect', () => {
        console.log('User disconnected:', socket.id);
    });
});


server.listen(PORT, () => {
    console.log(`🚀 Dashboard server listening on port ${PORT}`);
});