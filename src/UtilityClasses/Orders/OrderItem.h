#ifndef ORDER_ITEM_H
#define ORDER_ITEM_H
#include "../../Products/Product.h"

class OrderItem {
private:
    long orderItemID;
    int sellerID;
    int orderID;
    int productID;
    int quantity;
    Product* product;
    double priceAtPurchase;
    double subtotal;

public:
    // Constructor
    OrderItem(Product* product, int slrID, int ordID, int quantity, double price);
    ~OrderItem(); // Destructor

    // Getters
    int getSellerID() const;
    Product* getProduct() const;
    int getQuantity() const;
    double getSubtotal() const;
    int getOrderID() const;
    int getProductID() const;
    double getPriceAtPurchase() const;
    long getOrderItemID();
    void setOrderItemID(long OIID);

    // Utility
    void calculateSubtotal(); // price * quantity
};

#endif
