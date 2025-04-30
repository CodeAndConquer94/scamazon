#include "../Products/Product.h"
#include "ShoppingCart.h"
#include "PaymentMethod.h"
#include "CartItem.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <unordered_map>

using namespace std;

ShoppingCart::ShoppingCart(){
    total = 0.0;
}
ShoppingCart::~ShoppingCart(){
    clear();
}
void ShoppingCart::addItem(Product* prod, int quant){
    if (alreadyInCart(prod->getProductID())){
        int choice;
        while (true) {
            cout <<"Item already in cart!\n\nContinue adding to order?\n1. Yes\n2. No\n\nEnter Choice: ";
            if(!(cin >> choice)){
                cout << "Unexpected Error. Please try again.\n\n";
                cin.clear();
                cin.ignore(1000,'\n');
                continue;
            }
            if (choice == 1){
                for (CartItem* item : items) {
                    if (item->getProductID() == prod->getProductID()) {
                        int newQuant = item->quantity + quant;
                        if (!checkInventory(prod, newQuant)) {
                            cout << "Not enough inventory available. Cancelling.\n\n";
                            cin.clear();
                            cin.ignore(1000,'\n');
                            return;
                        }
                        item->updateQuantity(newQuant);
                        calculateTotal();
                        cout << "Item quantity updated!\n\n";
                        cin.clear();
                        cin.ignore(1000,'\n');
                        return;
                    }
                }
            }
            else if (choice == 2){
                cout << "Cancelled!\n\n";
                cin.clear();
                cin.ignore(1000,'\n');
                return;
            }
            else{
                cout << "Invalid Input. Please try again!\n\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
        }        
    }

    CartItem* newItem = new CartItem(prod, quant);
    items.push_back(newItem);
    calculateTotal();
    cout << "Item added to cart!\n\n";
}



void ShoppingCart::removeItem(int prodID){
    for (auto it = items.begin(); it != items.end(); ++it){
        if ((*it)->item->getProductID() == prodID){
            delete *it;
            items.erase(it);
            calculateTotal();
            cout << "Item removed from cart!\n\n";
            return;
        }
    }
    cout << "Item not found in cart.\n\n";
}
bool ShoppingCart::alreadyInCart(int prodID){
    for (CartItem* item : items) {
        if (item->getProductID() == prodID) {
            return true;
            }
        }
    return false;
}

void ShoppingCart::calculateTotal(){
    double newTotal = 0.0;
        for (CartItem* c: items){
            newTotal += c->subtotal;
        }
        total = newTotal;
    }
void ShoppingCart::clear(){
    for (CartItem* item : items) {
        delete item;
    }
    items.clear();
    total = 0.0;
}

bool ShoppingCart::isEmpty() const {
    return items.empty();
}

double ShoppingCart::getTotal() const { return total; }
int ShoppingCart::getNumItems(){
    return items.size();
}

void ShoppingCart::displayCart(){
    if (isEmpty()) {
        cout << "Shopping Cart is empty!\n\n";
        return;
    }
    int counter = 1;
    for (CartItem* item : items) {
        cout << "Item No: " << counter++ << " | Product ID: " << item->getProductID() << endl;
        item->displayCartItem();
        cout << "----------------------------------------\n";
    }
    cout << "Cart Total: $" << fixed << setprecision(2) << total << "\n\n";
}

void ShoppingCart::selectItem() {
    int choice;
    while (true) {
        cout << "Select Item from Cart\n";
        cout << "================================\n";
        cout << "Enter Item Number: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');

        if (choice < 1 || choice > items.size()) {
            cout << "Invalid choice. Please try again.\n\n";
            continue;
        }
        break;
    }

    cout << "Selected Item: \n";
    items[choice - 1]->displayCartItem();
    cout << "================================\n";
    cout << "Select an option: \n";
    cout << "1. Remove Item\n";
    cout << "2. Update Quantity\n";
    cout << "3. View Item Details\n";
    cout << "4. Go back.\n";
    cout << "================================\n";

    int option;
    while (true) {
        cout << "Enter Option: ";
        if (!(cin >> option)) {
            cout << "Invalid input. Please enter a number.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');

        if (option < 1 || option > 4) {
            cout << "Invalid choice. Please try again.\n\n";
            continue;
        }
        break;
    }

    switch (option) {
        case 1: {
            cout << "Are you sure you want to remove this item?\n";
            cout << "1. Yes\n";
            cout << "2. No\n";
            cout << "Enter Choice: ";

            int confirm;
            while (true) {
                if (!(cin >> confirm)) {
                    cout << "Invalid input. Please enter a number.\n\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }
                cin.ignore(1000, '\n');

                if (confirm == 1) {
                    removeItem(items[choice - 1]->getProductID());
                    cout << "Item removed from cart!\n\n";
                    break;
                } 
                else if (confirm == 2) {
                    cout << "Cancelled!\n\n";
                    break;
                } 
                else {
                    cout << "Invalid choice. Please try again.\n\n";
                    continue;
                }
            }
            break;
        }

        case 2: {
            int newQuant;
            cout << "Enter New Quantity: ";
            if (!(cin >> newQuant)) {
                cout << "Invalid input. Please enter a number.\n\n";
                cin.clear();
                cin.ignore(1000, '\n');
                break;
            }
            cin.ignore(1000, '\n');

            if (newQuant <= 0) {
                cout << "Invalid quantity. Must be greater than 0.\n\n";
                break;
            }
            if (!checkInventory(items[choice - 1]->item, newQuant)) {
                cout << "Not enough inventory available. Cancelling.\n\n";
                break;
            }

            items[choice - 1]->updateQuantity(newQuant);
            calculateTotal();
            cout << "Quantity updated!\n\n";
            break;
        }

        case 3:
            items[choice - 1]->viewItemDetails();
            cout << "\n\nPress Enter to continue...";
            cin >> ws;
            break;

        case 4:
            cout << "Cancelled!\n\n";
            break;
    }
}
CustomerOrder* ShoppingCart::checkout(DatabaseManager& db, int custID, PaymentMethod pMeth, Address shipAdrs) {
    if (isEmpty()){
        cout << "Shopping cart is empty. Cannot proceed to checkout.\n\n";
        return nullptr;
    }
    cout << "Confirm Checkout?\n1. Yes\n2. No\n\nEnter Choice: ";
    int confirm;
    while (true) {
        if (!(cin >> confirm)) {
            cout << "Invalid input. Please enter a number.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');

        if (confirm == 1) {
            cout << "Checkout confirmed!\n\n";
            break;
        } 
        else if (confirm == 2) {
            cout << "Cancelled!\n\n";
            return nullptr;
        } 
        else {
            cout << "Invalid choice. Please try again.\n\n";
            continue;
        }
    }
    cout << "Checkout in progress...\n\n";

    //Create new customer order, insert into database, convert to order items and insert into database and set order items in customer order
    CustomerOrder* newOrder = new CustomerOrder(custID, pMeth, pMeth.getPaymentID(), shipAdrs, shipAdrs.getAddressID(), total);
    db.insertCustomerOrder(newOrder);
    int orderID = newOrder->getOrderID();
    vector<OrderItem*> orderItems;
    for (CartItem* item : items) {
        OrderItem* newOrderItem = new OrderItem(item->item, item->item->getSellerID(), orderID, item->quantity, item->item->getPrice());
        db.insertOrderItem(*newOrderItem);
        orderItems.push_back(newOrderItem);
    }
    newOrder->setOrderItems(orderItems);
    Date today;
    newOrder->setOrderDate(today);

    // Update seller orders and balances

    unordered_map<int, vector<OrderItem*>> slrOrderMap;
    for (OrderItem* item : orderItems) {
        slrOrderMap[item->getSellerID()].push_back(item);
    }
    for (auto& [sellerID, sellerItems] : slrOrderMap) {
        double sellerTotal = 0.0;
        for (OrderItem* item : sellerItems) {
            sellerTotal += item->getSubtotal();
        }
        db.insertSellerOrder(orderID, sellerID, today.dateString(), sellerTotal);
        db.updateSellerBalance(sellerID, sellerTotal);
        }
    for (OrderItem* item : orderItems) {
        db.updateProductInventory(item->getProduct()->getProductID(), item->getProduct()->getInventory() - item->getQuantity());
        }

    clear();
    return newOrder;   
}

// Add this method to check inventory before adding to cart
bool ShoppingCart::checkInventory(Product* prod, int quant) {
    if (prod->getInventory() >= quant) {
        return true;
    }
    
    cout << "Not enough inventory available. Only " << prod->getInventory() 
         << " units available.\n\n";
    return false;
}



