# High-Frequency Trading (HFT) Simulator

A full-stack, real-time, high-performance order matching engine and trading simulator built from scratch. This project demonstrates a complete, vertically integrated system featuring a C++ core, a Java persistence layer, a Kafka data pipeline, and a real-time MERN stack dashboard.



## 📋 Features

* **High-Performance Matching Engine:** A core C++ engine designed for low latency, capable of handling a high volume of orders.
* **Real-Time Data Pipeline:** Utilizes Apache Kafka to stream order, trade, and order book data between services in real time.
* **Persistent State:** A robust Java & Spring Boot service consumes and saves all trades and order book snapshots to a PostgreSQL database, allowing the engine to be resilient to restarts.
* **Live Web Dashboard:** A MERN-stack (MongoDB, Express, React, Node.js) application provides a live view of the market.
    * Real-time updates of Bids, Asks, and recent Trades.
    * Interactive form to submit new BUY and SELL orders.
* **Containerized Infrastructure:** All backend services (PostgreSQL, Kafka, Zookeeper) are containerized with Docker for easy setup and deployment.

## 🏛️ System Architecture

The project is built on a microservices-oriented architecture, with each component designed for a specific task.

1.  **Order Submission:**
    * A user submits an order via the **React** frontend.
    * The order is sent over a **WebSocket** to the **Node.js** server.
    * The Node.js server acts as a producer and sends the order to the `orders` Kafka topic.

2.  **Order Matching:**
    * The **C++ Matching Engine** is the only consumer of the `orders` topic.
    * It processes the order, matches it against its in-memory order book, and generates trades.
    * Upon a match, it produces a message to the `trades` Kafka topic.
    * Periodically, it produces a full snapshot of its order book to the `orderbook-snapshots` Kafka topic.

3.  **Data Persistence & Display:**
    * The **Java Service** consumes from both the `trades` and `orderbook-snapshots` topics and saves the data to the **PostgreSQL** database.
    * The **Node.js Server** *also* consumes from these topics and broadcasts the data in real-time to all connected **React** clients via WebSockets, updating the UI.

4.  **State Recovery:**
    * On startup, the C++ engine sends a message to a `snapshot-requests` topic.
    * The Java service hears this, reads the last saved snapshot from PostgreSQL, and publishes it to the `orderbook-snapshots` topic for the C++ engine to consume and restore its state.

## 💻 Tech Stack

* **Matching Engine:** C++, CMake
* **Persistence Service:** Java, Spring Boot, Spring Data JPA, Spring for Apache Kafka
* **Web Backend:** Node.js, Express.js, Socket.IO, Kafkajs
* **Web Frontend:** React (Vite), Socket.IO Client
* **Database:** PostgreSQL (for trades/order book), MongoDB (for user accounts - planned)
* **Message Broker:** Apache Kafka
* **Infrastructure:** Docker, Docker Compose

## 🚀 Getting Started

### Prerequisites

* Git
* Docker & Docker Compose
* A C++ compiler and build environment (MSYS2 on Windows with MinGW-w64, `g++`, `make`, `cmake`)
* Node.js and npm
* Java JDK (version 17 or higher) and Maven

### How to Run

You will need approximately 4-5 terminal windows.

1.  **Start Infrastructure (Docker):**
    * Navigate to the `settlement-service` directory.
    * Run `docker compose up -d`.

2.  **Start the Java Service:**
    * Navigate to the `settlement-service/settlement-service` directory.
    * Run `.\mvnw.cmd clean install -DskipTests` to build the project.
    * Run the application: `java "-Duser.timezone=UTC" -jar target/settlement-service-0.0.1-SNAPSHOT.jar --spring.datasource.password=password`.

3.  **Start the Dashboard Backend (Node.js):**
    * Navigate to the `hft-dashboard/server` directory.
    * Run `npm install` (only the first time).
    * Run `npm start`.

4.  **Start the Dashboard Frontend (React):**
    * Navigate to the `hft-dashboard/client` directory.
    * Run `npm install` (only the first time).
    * Run `npm run dev`. This will provide a URL for your browser.

5.  **Start the Matching Engine (C++):**
    * Navigate to the `hft-simulator/build` directory.
    * (If you haven't built it, run `cmake ...` and `mingw32-make` as described in our conversation).
    * Run the executable: `./engine.exe`.

The full system is now running! Open the React app URL in your browser to interact with the simulator.
