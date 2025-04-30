#ifndef CART_ITEM_H
#define CART_ITEM_H
#include "../Products/Product.h"




class CartItem{
    public:
    Product* item;
    int quantity;
    double subtotal;

    CartItem(Product* prod, int quant);


    void displayCartItem();
    void viewItemDetails();
    void updateQuantity(int newQuantity);
    void recalculateSubtotal();
    int getProductID() const;


};

#endif