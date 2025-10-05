// client/src/OrderBook.jsx
import React from 'react';

const OrderBook = ({ bids, asks }) => {
  const renderOrders = (orders, className) => (
    <div className={className}>
      <h2>{className === 'bids' ? 'Bids (BUY)' : 'Asks (SELL)'}</h2>
      <table>
        <thead>
          <tr>
            <th>Price</th>
            <th>Quantity</th>
          </tr>
        </thead>
        <tbody>
          {orders.map((order, index) => (
            <tr key={index}>
              <td>${(order.price / 100).toFixed(2)}</td>
              <td>{order.quantity}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );

  return (
    <div className="order-book-container">
      {renderOrders(bids, 'bids')}
      {renderOrders(asks, 'asks')}
    </div>
  );
};

export default OrderBook;