#include "OrderItem.h"
#include "../../Products/Product.h"

// Constructor
OrderItem::OrderItem(Product* product, int slrID, int ordID, int quantity, double price) 
    : product(product), sellerID(slrID), orderID(ordID), quantity(quantity), priceAtPurchase(price) {
        calculateSubtotal();
}
// Getters
int OrderItem::getSellerID() const {
    return sellerID;
}
Product* OrderItem::getProduct() const {
    return product;
}
int OrderItem::getQuantity() const {
    return quantity;
}
double OrderItem::getSubtotal() const {
    return subtotal;
}
// Utility
void OrderItem::calculateSubtotal() {
    subtotal = priceAtPurchase * quantity;
}
long OrderItem::getOrderItemID(){ return orderItemID; }
void OrderItem::setOrderItemID(long OIID){
    orderItemID = OIID;
}
int OrderItem::getOrderID() const {
    return orderID;
}
int OrderItem::getProductID() const {
    return product->getProductID();
}
double OrderItem::getPriceAtPurchase() const {
    return priceAtPurchase;
}
// Destructor
OrderItem::~OrderItem() {
    delete product; // Assuming ownership of the product pointer
}