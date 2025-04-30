#include "CartItem.h"
#include "../Products/Product.h"
#include <iostream>
#include <iomanip>

using namespace std;

CartItem::CartItem(Product* prod, int quant):
    item(prod), quantity(quant){
        recalculateSubtotal();
    }

void CartItem::updateQuantity(int newQuant){
    quantity = newQuant;
    recalculateSubtotal();
}

int CartItem::getProductID() const{
    return item->getProductID();
}
void CartItem::recalculateSubtotal(){
    subtotal = quantity * item->getPrice();
}
void CartItem::displayCartItem() {
    cout << left << setw(15) << "Item Name" << " | " << left << setw(30) << item->getProductName() << '\n';
    cout << left << setw(15) << "Item Price" << " | $" << right << setw(8) << fixed << setprecision(2) << item->getPrice() << '\n';
    cout << left << setw(15) << "Quantity" << " | " << right << setw(8) << quantity << '\n';
    cout << left << setw(15) << "Subtotal" << " | $" << right << setw(8) << fixed << setprecision(2) << subtotal << "\n\n";
}
void CartItem::viewItemDetails(){
    item->displayProduct();
}







