// client/src/App.jsx
import React, { useState, useEffect } from 'react';
import io from 'socket.io-client';
import './App.css'; // We'll use this for basic styling
import OrderBook from './OrderBook';
import OrderEntry from './components/OrderEntry'; // <-- Import OrderEntry

// Connect to the Node.js server
const socket = io('http://localhost:4000');

function App() {
  const [trades, setTrades] = useState([]);
  const [orderBook, setOrderBook] = useState({ bids: [], asks: [] }); // <-- Add state for order book

  useEffect(() => {
    const handleNewTrade = (trade) => {
      setTrades(prevTrades => [trade, ...prevTrades.slice(0, 19)]); // Keep last 20 trades
    };


     // --- ADD THIS HANDLER ---
    const handleOrderBookUpdate = (snapshot) => {
      // The snapshot from C++ is unsorted, so we sort it here
      const sortedBids = snapshot.bids ? snapshot.bids.sort((a, b) => b.price - a.price) : [];
      const sortedAsks = snapshot.asks ? snapshot.asks.sort((a, b) => a.price - b.price) : [];
      setOrderBook({ bids: sortedBids, asks: sortedAsks });
    };

    // Listen for 'new-trade' events from the server
    socket.on('new-trade', handleNewTrade);
    socket.on('orderbook-update', handleOrderBookUpdate); // <-- Listen for the new event

    // Cleanup function to remove the listener when the component unmounts
    return () => {
      socket.off('new-trade', handleNewTrade);
      socket.off('orderbook-update', handleOrderBookUpdate); // <-- Clean up the listener
    };
  }, []); // The empty dependency array ensures this effect runs only once

  return (
    <div className="App">
      <header className="App-header">
        <h1>🚀 High-Frequency Trading Simulator</h1>
      </header>
      <main className="container">
       <div className="dashboard-main">
            <OrderBook bids={orderBook.bids} asks={orderBook.asks} />
            <OrderEntry socket={socket} /> {/* <-- Add the form component */}
        </div>
        <div className="trade-history">
          <h2>Recent Trades</h2>
          <table>
            <thead>
              <tr>
                <th>Trade ID</th>
                <th>Price</th>
                <th>Quantity</th>
                <th>Buy Order ID</th>
                <th>Sell Order ID</th>
              </tr>
            </thead>
            <tbody>
              {trades.map((trade) => (
                <tr key={trade.tradeId}>
                  <td>{trade.tradeId}</td>
                  <td>${(trade.price / 100).toFixed(2)}</td>
                  <td>{trade.quantity}</td>
                  <td>{trade.buyOrderId}</td>
                  <td>{trade.sellOrderId}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </main>
    </div>
  );
}

export default App;