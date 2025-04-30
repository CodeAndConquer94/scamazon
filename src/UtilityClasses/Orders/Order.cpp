#include "Order.h"
#include "OrderItem.h"
#include "../../Products/Product.h"
#include "../Date.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

Order::Order() : orderID(0), total(0.0) {}
Order::Order(int oid, string date, double tot):
    items{}{
    setOrderID(oid);
    setOrderDate(Date(date));
    setTotal(tot);
}
Order::~Order(){
    for (OrderItem* oi: items){
        delete oi;
    }
    items.clear();
}

void Order::setOrderID(int OID) {
    orderID = OID;
}
void Order::setOrderItems(const vector<OrderItem*>& ordItems) {
    items = ordItems;
}
void Order::setOrderDate(const Date& date) {
    orderDate = date;
}
void Order::setTotal(double tot) {
    total = tot;
}
int Order::getOrderID() const {
    return orderID;
}
const vector<OrderItem*>& Order::getItems() {
    return items;
}
Date Order::getOrderDate() const {
    return orderDate;
}
double Order::getTotal() const {
    return total;
}
void Order::displayOrderItems() {
    cout << "\n===== Order Items =====" << endl;
    cout << setw(12) << left << "Item ID" 
         << setw(30) << left << "Product Name"
         << setw(15) << left << "Quantity" 
         << setw(15) << right << "Price" 
         << setw(15) << right << "Subtotal" << endl;
    cout << string(87, '-') << endl;
    
    for (int i = 0; i < items.size(); i++) {
        Product* prod = items[i]->getProduct();
        cout << setw(12) << left << items[i]->getOrderItemID() << "."
             << setw(30) << left << prod->getProductName()
             << setw(15) << left << items[i]->getQuantity()
             << "$" << setw(14) << right << fixed << setprecision(2) << items[i]->getPriceAtPurchase()
             << "$" << setw(14) << right << fixed << setprecision(2) << items[i]->getSubtotal()
             << endl;
    }
    cout << string(87, '-') << endl;
}
