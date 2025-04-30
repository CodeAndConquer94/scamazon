#ifndef SELLER_ORDER_H
#define SELLER_ORDER_H
#include "../Date.h"
#include <vector>
#include <string>
#include "OrderItem.h"
#include "../../Products/Product.h"
#include "Order.h"

using namespace std;
class SellerOrder : public Order {
    private:
        int sellerID;
    
    public:
        // Constructor
        SellerOrder(int orderID, int sellerID, string date, double tot);

    
        // Getters - some are now inherited from Order
        int getSellerID() const;
        
        // Override methods from Order
        void displayOrderSummary() override;
    };
    
#endif
