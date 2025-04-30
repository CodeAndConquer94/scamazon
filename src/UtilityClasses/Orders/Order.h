#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <string>
#include "../Date.h"
#include "OrderItem.h"

using namespace std;

class Order {
protected:
    int orderID;
    vector<OrderItem*> items;
    Date orderDate;
    double total;

public:
    // Constructor
    Order();
    Order(int OID, string date, double tot);
    virtual ~Order();
    
    // Setters
    void setOrderID(int OID);
    void setOrderItems(const vector<OrderItem*>& ordItems);
    void setOrderDate(const Date& date);
    void setTotal(double tot);
    
    // Getters
    int getOrderID() const;
    const vector<OrderItem*>& getItems();
    Date getOrderDate() const;
    double getTotal() const;
    
    // Other shared methods
    virtual void displayOrderSummary() = 0;
    void displayOrderItems();
};

#endif