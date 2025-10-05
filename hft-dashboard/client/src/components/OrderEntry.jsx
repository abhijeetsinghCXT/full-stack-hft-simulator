// client/src/components/OrderEntry.jsx
import React, { useState } from 'react';

const OrderEntry = ({ socket }) => {
  const [side, setSide] = useState('BUY');
  const [price, setPrice] = useState('');
  const [quantity, setQuantity] = useState('');

  const handleSubmit = (e) => {
    e.preventDefault();
    if (!price || !quantity) {
      alert('Please enter a price and quantity.');
      return;
    }

    const order = {
      price: parseInt(price * 100), // Convert to cents
      quantity: parseInt(quantity),
      side: side,
    };

    // Emit the new order to the server
    socket.emit('new-order', order);
    console.log('Submitted new order:', order);

    // Clear the form
    setPrice('');
    setQuantity('');
  };

  return (
    <div className="order-entry">
      <h2>Submit New Order</h2>
      <form onSubmit={handleSubmit}>
        <div className="form-group">
          <label>Side:</label>
          <select value={side} onChange={(e) => setSide(e.target.value)}>
            <option value="BUY">BUY</option>
            <option value="SELL">SELL</option>
          </select>
        </div>
        <div className="form-group">
          <label>Price ($):</label>
          <input
            type="number"
            value={price}
            onChange={(e) => setPrice(e.target.value)}
            placeholder="e.g., 100.50"
            step="0.01"
          />
        </div>
        <div className="form-group">
          <label>Quantity:</label>
          <input
            type="number"
            value={quantity}
            onChange={(e) => setQuantity(e.target.value)}
            placeholder="e.g., 10"
          />
        </div>
        <button type="submit" className={side === 'BUY' ? 'buy-btn' : 'sell-btn'}>
          Submit {side}
        </button>
      </form>
    </div>
  );
};

export default OrderEntry;