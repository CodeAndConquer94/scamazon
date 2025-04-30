#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H
#include "../Products/Product.h"
#include "../Database/DatabaseManager.h"
#include "../UtilityClasses/Orders/CustomerOrder.h"
#include "CartItem.h"
#include <vector>
#include <iostream>

using namespace std;


class ShoppingCart {
    private:
        vector<CartItem*> items;
        double total;

    public:
        ShoppingCart();
        ~ShoppingCart();
    //Core functions
        void addItem(Product* product, int quantity = 1);
        void removeItem(int productId);
        bool alreadyInCart(int productID);
        void calculateTotal();
        void clear();
        bool isEmpty() const;
        double getTotal() const;
        int getNumItems();
        void displayCart();
        void selectItem();
        bool checkInventory(Product* prod, int quant);

    //
        CustomerOrder* checkout(DatabaseManager& db, int custID, PaymentMethod pMeth , Address shipAdrs);


};





#endif