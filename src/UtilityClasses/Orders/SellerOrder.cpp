#include "SellerOrder.h"
#include "Order.h"

#include <iostream>

using namespace std;

// Constructor implementation
SellerOrder::SellerOrder(int oID, int sID, string date, double tot) : sellerID(sID) {
    orderID = oID;
    total = tot;
    orderDate = Date(date);
}



// Get the seller ID
int SellerOrder::getSellerID() const {
    return sellerID;
}

// Display order summary
void SellerOrder::displayOrderSummary() {
    cout << "\n===== Seller Order Summary =====" << endl;
    cout << "Order ID: " << orderID << endl;
    cout << "Seller ID: " << sellerID << endl;
    cout << "Total Amount: $" << total << endl;
    cout << "Order Date: " << orderDate.dateString() << endl;
    cout << string(30, '-') << endl;

    // Display order items
    displayOrderItems();
}

