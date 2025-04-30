#include "User.h"
#include "../Products/Product.h"
#include "../UtilityClasses/ShoppingCart.h"
#include "../UtilityClasses/Orders/CustomerOrder.h"
#include "../UtilityClasses/PaymentMethod.h"
#include "Customer.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "../Database/DatabaseManager.h"

using namespace std;




       

Customer::Customer(string fName, string lName, string uName, string pw, string em): 
    User(fName, lName, uName, pw, em){
        role = "customer";
        paymentMethods = {};
        addresses = {};
        orderHistory = {};
        cart = ShoppingCart();
}

Customer::Customer(int UID, string fName, string lName, string uName, string pw, string em, string role ):
    User(UID, fName, lName, uName, pw, em, role), paymentMethods(), addresses(), orderHistory(){}

Customer::~Customer(){
    for (PaymentMethod* pm : paymentMethods){
        delete pm;
    }
    paymentMethods.clear();
    for (CustomerOrder* co : orderHistory){
        delete co;
    }
    orderHistory.clear();
    for (Address* adrs : addresses){
        delete adrs;
    }
    addresses.clear();
    cart.clear();
}

void Customer::importPaymentMethods(const vector<PaymentMethod*>& payMethods){
    paymentMethods = payMethods;
}
void Customer::addPaymentMethod(DatabaseManager& db){
    while (true) {//confirm
        int confirm;
        cout << "Scamazon only accepts Visa, Mastercard, Discover and American Express.\n\n";
        cout << "Enter new Payment Method?\n1. Confirm\n2. Cancel \n\nEnter Choice: ";

        if (cin >> confirm) {
            cin.ignore(1000, '\n');

            if (confirm == 1) break;
            else if (confirm == 2) return;
            else {
                cout << "Invalid Input: Please enter 1 or 2.\n\n";
            }
        } 
        else {
            cout << "Unexpected Error: Please enter a number.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    //get payment method details and check to make sure its new
    PaymentMethod* newPM = new PaymentMethod(userID);
    while (true) { // get payment details
        if (!(cin >> *newPM)){
            cout << "Payment input failed unexpectedly. Please try again.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;
    }
    if (payMethExists(newPM)){ //check to ensure new payment method
        cout << "Payment Method Already exists. Cancelling\n\n";
        delete newPM;
        return;
    }

    //choose to use existing address or new address
    int choice;
    while (true) {        
        cout << "Select an option:\n\n1. Choose Billing Address from my Address Book\n2. Enter new Address\n\nEnter Choice: ";
        if (!(cin >> choice)){
            cout << "Unexpected Error: Please try again.\n\n";
            cin.clear();
            cin.ignore(1000,'\n');
            continue;
        }
        if (choice != 1 && choice != 2) {
            cout << "Invalid Entry. Please try again and enter either 1 or  2.\n\n";
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        if (choice == 1){
            Address* bllAddress = selectAddress();
            if (bllAddress == nullptr){
                cout << "Cancelled. Try again.\n\n";
                continue;
            }
            else{
                newPM->setBillingAddress(*bllAddress);
                newPM->setBillingAddressID(bllAddress->getAddressID());
                cout << "Address selected successfully.\n\n";     
                break;
            }  
    
        }
        if (choice == 2) {
            Address* bllAddress = addAddress(db);
            if (bllAddress == nullptr){
                cout << "Cancelled. Try again.\n\n";
                continue;
            }
            else {
                newPM->setBillingAddress(*bllAddress);
                newPM->setBillingAddressID(bllAddress->getAddressID());
                cout << "Address selected successfully.\n\n";
                break;  
            }           
            
        }
    }
    
    db.insertPaymentMethod(*newPM);
    if (newPM->isDefault()){
        for (PaymentMethod* pm : paymentMethods){
            if (pm->isDefault()){
                pm->setDefaultPayment(false);
                db.updatePaymentMethod(*pm);
            }
        }
    }
    paymentMethods.push_back(newPM);
    cout << "Payment Method added successfully.\n\n";
    cout << *newPM;
    cout << "\n\n";
}

 

bool Customer::payMethExists(PaymentMethod* pm){
    for (PaymentMethod* payMeth: paymentMethods){
        if (*pm == *payMeth){
            return true;
        }
    }
    return false;
}
void Customer::removePaymentMethod(DatabaseManager& db){
    PaymentMethod* selectedMethod = selectPayment();
    if (selectedMethod != nullptr) {
        db.deletePaymentMethod(selectedMethod->getPaymentID());
        for (auto it = paymentMethods.begin(); it != paymentMethods.end(); ++it ){
            if ((*it) == selectedMethod){
                delete (*it);
                paymentMethods.erase(it);
                break;
            }
        }
    }
}
void Customer::importAddressBook(const vector<Address*>& addrsBk){
    addresses = addrsBk;
}

Address* Customer::addAddress(DatabaseManager& db){
    while (true) {//confirm
        int confirm;
        cout << "Enter new Address?\n1. Confirm\n2. Cancel \n\nEnter Choice: ";

        if (cin >> confirm) {
            cin.ignore(1000, '\n');

            if (confirm == 1) break;
            else if (confirm == 2) return nullptr;
            else {
                cout << "Invalid Input: Please enter 1 or 2.\n\n";
            }
        } 
        else {
            cout << "Unexpected Error: Please enter a number.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    Address* newAddress = new Address(userID);
    while (true) {
        if (!(cin >> *newAddress)){
            cout << "Address input failed unexpectedly. Please try again.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;
    }

    if (addressExists(*newAddress)){
        cout << "Address already exists. Cancelling\n\n";
        delete newAddress;
        return nullptr;
    }
    else{
        db.insertAddress(*newAddress);
        addresses.push_back(newAddress);
    }
    return newAddress;


}

bool Customer::addressExists(const Address& newAdrs){
    for (Address* a : addresses){
        if (newAdrs == *a){
            return true;
        }
    }
    return false;
}
void Customer::removeAddress(DatabaseManager& db){
    Address* selectedAdrs = selectAddress();
    if (selectedAdrs != nullptr) {
        db.deleteAddress(selectedAdrs->getAddressID());
        for (auto it = addresses.begin(); it != addresses.end(); ++it ){
            if ((*it) == selectedAdrs){
                delete (*it);
                addresses.erase(it);
                break;
            }
        }
    }

}

Address* Customer::selectAddress(){
    if (addresses.empty()){
        cout << "No addresses found. Please add an address.\n\n";
        return nullptr;
    }
    cout << "Select an Address:\n\n";
    for (int i = 0; i < addresses.size(); i++){
        cout << "[" << i+1 << "] " << *addresses[i] << "\n\n";
    }
    int choice;
    while (true) {
        cout << "Enter Choice: ";
        if (!(cin >> choice) || choice < 1 || choice > addresses.size()){
            cout << "Unexpected Error: Please try again and enter a number between 1 and " << addresses.size() << ".\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;
    }
    cin.ignore(1000, '\n');
    return addresses[choice-1];
}

void Customer::addToCart(Product* prod){
    cout << "Adding to cart...\n\n";
    cout << "Please select a quantity: ";
    int quant;
    while (true) {
        if (!(cin >> quant)) {
            cout << "Invalid input. Please enter a number.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if (quant <= 0) {
            cout << "Invalid quantity. Must be greater than 0.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if (quant > prod->getInventory()){
            cout << "Not enough inventory available. Cancelling.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
        break;
    }
    cin.ignore(1000, '\n');
    cart.addItem(prod, quant);
    cout << "Item added to cart!\n\n";
    cout << "Cart Total: $" << fixed << setprecision(2) << cart.getTotal() << "\n\n";
    return;    
}


void Customer::viewCart(DatabaseManager& db){
    if (cart.isEmpty()){
        cout << "Shopping cart is empty!\n\n";
        return;
    }
    cart.displayCart();
    cout << "Select an option:\n1. Checkout\n2. Clear Cart\n3. Select an Item\n4. Return to Main Menu\n\nEnter Choice: ";
    int choice;
    while (true) {
        if (!(cin >> choice) || choice < 1 || choice > 4){
            cout << "Unexpected Error: Please try again and enter a number between 1 and 4.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        break;
    }
    switch (choice){
        case 1:
            cout << "Checking out...\n\n";
            placeOrder(db);
            break;
        case 2:
            cart.clear();
            cout << "Cart cleared!\n\n";
            break;
        case 3:
            cart.selectItem();
            viewCart(db);
            break;
        case 4:
            cout << "Returning to Main Menu...\n\n";
            break;
    }
}
void Customer::placeOrder(DatabaseManager& db){
    if (cart.isEmpty()){
        cout << "Shopping cart is empty. Cannot proceed to checkout.\n\n";
        return;
    }
    cout << "Select a payment method (Enter 0 for new payment method):\n\n";
    for (int i = 0; i < paymentMethods.size(); i++){
        cout << "[" << i+1 << "] " << *paymentMethods[i] << "\n\n";
    }
    int choice;
    while (true) {
        cout << "Enter Choice: ";
        if (!(cin >> choice) || choice < 0 || choice > paymentMethods.size()){
            cout << "Unexpected Error: Please try again and enter a number between 0 and " << paymentMethods.size() << ".\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        break;
    }
    PaymentMethod* selectedPM = nullptr;
    if (choice == 0){
        cout << "Adding new payment method...\n\n";
        addPaymentMethod(db);
        selectedPM = paymentMethods.back();
    }
    else {
        selectedPM = paymentMethods[choice-1];
    }
    cout << "Selected Payment Method:\n" << *selectedPM << "\n\n";
    cout << "Select a shipping address (Enter 0 for new address):\n\n";
    for (int i = 0; i < addresses.size(); i++){
        cout << "[" << i+1 << "] " << *addresses[i] << "\n\n";
    }
    Address* selectedAdrs = nullptr;
    while (true) {
        cout << "Enter Choice: ";
        if (!(cin >> choice) || choice < 0 || choice > addresses.size()){
            cout << "Unexpected Error: Please try again and enter a number between 0 and " << addresses.size() << ".\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        if (choice == 0){
            cout << "Adding new address...\n\n";
            Address* newAddress = addAddress(db);
            if (newAddress == nullptr){
                cout << "Cancelled. Try again.\n\n";
                continue;
            }
            selectedAdrs = newAddress;
            cout << "Selected Address:\n" << *selectedAdrs << "\n\n";
        }
        else{
            selectedAdrs = addresses[choice-1];
            cout << "Selected Address:\n" << *selectedAdrs << "\n\n";
        }
        break;
    }
    CustomerOrder* newOrder = cart.checkout(db, userID, *selectedPM, *selectedAdrs);
    if (newOrder) {
        orderHistory.push_back(newOrder);
        cout << "Order placed successfully!\n\n";
    }
    else {
        cout << "Order placement failed or was cancelled.\n\n";
    }
}

void Customer::viewOrderHistory() const{
    if (orderHistory.empty()){
        cout << "No order history to display.\n\n";
        return;
    }
    for (CustomerOrder* co: orderHistory){
        co->displayOrderSummary();
        char selection;
        cout << "Select an Option: \n\n[N] Next Order\n[Q] Return to Previous Menu.\n\nEnter selection: ";
        while (true) {
            if (!(cin >> selection) || selection != 'N' && selection != 'n' && selection != 'Q' && selection != 'q'){
                cout << "Unexpected Error: Please try again and enter [N] or [Q]\n\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            break;
        }
        if (selection == 'N' || selection == 'n') {
            if (co == orderHistory.back()){
                cout << "End of order history. \n\n";
                return;
            }
            continue;
        }
        if (selection == 'Q' || selection == 'q'){
            return;
        }
    }
}

void Customer::importOrderHistory(const vector<CustomerOrder*>& orders){
    orderHistory = orders;
}

void Customer::updateProfile(DatabaseManager& dbm) {
    User::updateProfile(dbm);
    cout << "===========================\n";
    cout << "  Customer Profile Update  \n";
    cout << "===========================\n";
    cout << "| 1.    Add an Address    |\n";
    cout << "| 2.  Remove an Address   |\n";
    cout << "| 3.  Add Payment Method  |\n";
    cout << "| 4. Remove Payment Method|\n";
    cout << "| 5.    Cancel Update     |\n";
    cout << "===========================\n";
    int selection;
    while (true) {
        cout << "Enter Selection: ";
        if (!(cin >> selection) || selection < 1 || selection > 5){
            cout << "Unexpected Error: Please Try again and enter a number 1-5.\n\n";
            cin.clear();
            cin.ignore(1000,'\n');
            continue;
        }
        cin.ignore(1000,'\n');
        break;
    }
    
    switch (selection) {
        case 1: 
            addAddress(dbm); 
            break;
        case 2: 
            removeAddress(dbm); 
            break;
        case 3: 
            addPaymentMethod(dbm); 
            break;
        case 4: 
            removePaymentMethod(dbm); 
            break;
        case 5: 
            cout << "Update cancelled.\n\n";
            break;
    }
}
void Customer::viewProfile(DatabaseManager& dbm) {
    User::viewProfile(dbm);
    
    cout << "\n===== Customer Summary =====\n\n";
    cout << "Payment Methods: " << paymentMethods.size() << "\n";
    cout << "Saved Addresses: " << addresses.size() << "\n";
    cout << "Order History: " << orderHistory.size() << " orders\n";
    cout << "Shopping Cart: " << (cart.isEmpty() ? 0 : cart.getNumItems()) << " items\n\n";
    
    cout << "===== Customer Options =====\n";
    cout << "1. View Payment Methods\n";
    cout << "2. View Address Book\n";
    cout << "3. View Order History\n";
    cout << "4. View Shopping Cart\n";
    cout << "5. Return to Main Menu\n\n";
    
    int choice;
    while (true) {
        cout << "Enter choice: ";
        if (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << "Invalid choice. Please enter a number between 1 and 5.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        break;
    }
    
    switch (choice) {
        case 1:
            viewPaymentMethods();
            break;
        case 2:
            viewAddressBook();
            break;
        case 3:
            viewOrderHistory();
            break;
        case 4:
            viewCart(dbm);
            break;
        case 5:
            cout << "Returning to main menu...\n\n";
            break;
    }
}

void Customer::viewPaymentMethods() {
    cout << "\n===== Payment Methods =====\n\n";
    
    if (paymentMethods.empty()) {
        cout << "You have no payment methods saved.\n\n";
        return;
    }
    
    for (int i = 0; i < paymentMethods.size(); i++) {
        cout << "Method " << (i+1) << ":\n" << *paymentMethods[i] << "\n\n";
    }
    
    cout << "Press Enter to continue...";
    cin.ignore(1000, '\n');
}

void Customer::viewAddressBook() {
    cout << "\n===== Address Book =====\n\n";
    
    if (addresses.empty()) {
        cout << "You have no addresses saved.\n\n";
        return;
    }
    
    for (int i = 0; i < addresses.size(); i++) {
        cout << "Address " << (i+1) << ":\n" << *addresses[i] << "\n\n";
    }
    
    cout << "Press Enter to continue...";
    cin.ignore(1000, '\n');
}

PaymentMethod* Customer::selectPayment(){
    if (paymentMethods.empty()){
        cout << "No Payment Methods found. Please add a Payment Method.\n\n";
        return nullptr;
    }
    cout << "Select a Payment Method:\n\n";
    for (int i = 0; i < paymentMethods.size(); i++){
        cout << "[" << i+1 << "] " << *paymentMethods[i] << "\n\n";
    }
    int choice;
    while (true) {
        cout << "Enter Choice: ";
        if (!(cin >> choice) || choice < 1 || choice > paymentMethods.size()){
            cout << "Unexpected Error: Please try again and enter a number between 1 and " << paymentMethods.size() << ".\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;
    }
    cin.ignore(1000, '\n');
    return paymentMethods[choice-1];
}

