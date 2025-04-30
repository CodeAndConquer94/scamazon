#include "CustomerOrder.h"
#include <iostream>
#include "../PaymentMethod.h"

using namespace std;
// Constructor 
CustomerOrder::CustomerOrder(const int custID, const PaymentMethod& payment, const int payID, Address& address, const int adrsID, double total) 
    : customerId(custID), paymentUsed(payment), shippingAddress(address), shippingAddressId(adrsID), paymentMethodId(payID) {
        this->total = total;
        this->orderDate = Date();
    }
//Constructor for loading from database
CustomerOrder::CustomerOrder(int orderID, int custID, const vector<OrderItem*>& ordItems, const PaymentMethod& payment, Address& address, const Date& date, double tot) 
    : customerId(custID), paymentUsed(payment), shippingAddress(address) {
        this->orderID = orderID;
        this->items = ordItems;
        this->orderDate = date;
        this->total = tot;
        shippingAddressId = address.getAddressID();
        paymentMethodId = payment.getPaymentID();
    }
// Setters
void CustomerOrder::setOrderID(int OID) {
    orderID = OID;
}
void CustomerOrder::setOrderItems(const vector<OrderItem*>& ordItems) {
    items = ordItems;
}
int CustomerOrder::getOrderID() const {
    return orderID;
}
int CustomerOrder::getCustomerID() const { return customerId; }
int CustomerOrder::getShippingAddressID() const { return shippingAddressId; }
int CustomerOrder::getPaymentID() const { return paymentMethodId; }

// Display order summary
void CustomerOrder::displayOrderSummary() {
    cout << "\n===== Customer Order Summary =====" << endl;
    cout << "Order ID: " << orderID << endl;
    cout << "Customer ID: " << customerId << endl;
    cout << "Total Amount: $" << total << endl;
    cout << "Order Date: " << orderDate.dateString() << endl;
    cout << "Payment Method: " << paymentUsed.getCardType() << paymentUsed.getCardNumber()/10000 << endl;
    cout << "Shipping Address: " << shippingAddress << endl;
    cout << string(30, '-') << endl;

    // Display order items
    displayOrderItems();
}

