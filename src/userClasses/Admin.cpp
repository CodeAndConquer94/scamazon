#include "../Database/DatabaseManager.h"
#include "Admin.h"
#include "../UtilityClasses/Orders/CustomerOrder.h"
#include "../UtilityClasses/Orders/OrderItem.h"
#include "../Products/Product.h"
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;


Admin::Admin(string fName, string lName, string uName, string pw, string em): User(fName, lName, uName, pw, em){
}

Admin::Admin(int UID, string fName, string lName, string uName, string pw, string em, string rl): User(UID, fName, lName, uName, pw, em, rl){
}

void Admin::resetUserPassword(DatabaseManager& db) {
    std::string uName, em, pw;

    while (true) {
        std::cout << "Enter Username of User you'd like to reset for: ";
        if (!(std::cin >> uName)) {
            std::cout << "Unexpected Error: Please try again.\n\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Enter Email of User you'd like to reset password for: ";
        if (!(std::cin >> em)) {
            std::cout << "Unexpected Error: Please try again.\n\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n'); 
            continue;
        }
        break;
    }

    pw = generatePassword();
    string hashedPW = hashPassword(pw);
    bool success = db.resetUserPassword(uName, em, hashedPW);

    if (success) {
        std::cout << "\nPassword reset successful.\n";
        std::cout << "New temporary password: " << pw << "\n\n";
    } else {
        std::cout << "\nNo matching user found with that username/email.\n\n";
    }

    std::cout << "Press any key to continue...";
    std::cin.ignore();
    std::cin.get();
}



void Admin::refundCustomer(DatabaseManager& db) {
    int ordID, itemID;
    cout << "Enter Order ID: ";
    while (true) {
        if (!(cin >> ordID)) {
            cout << "Unexpected Error: Please try again and enter only numerical values.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;
    }

    CustomerOrder* order = db.retrieveCustomerOrder(ordID);
    if (!order) {
        cout << "\nOrder not found.\n\n";
        return;
    }

    order->displayOrderSummary();

    cout << "\nEnter Item ID for the item being refunded: ";
    while (true) {
        if (!(cin >> itemID)) {
            cout << "Unexpected Error: Please try again and enter only numerical values.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        break;    
    }

    vector<OrderItem*> items = order->getItems();// returns order items by reference
    bool found = false;
    for (OrderItem* itm : items) {
        if (itm->getOrderItemID() == itemID) {
            db.updateSellerBalance(itm->getSellerID(), -itm->getSubtotal());
            cout << "\nRefunded customer and billed seller!\n\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\nItem ID not found in this order.\n\n";
    }

    delete order; //automatically cleans up order items.
    
    cout << "Press any key to continue...";
    cin.ignore();
    cin.get();
}





void Admin::deleteProduct(DatabaseManager& db){
    int prodID;
    cout << "Enter Product ID: ";
    while (true) {
        if (!(cin >> prodID)){
            cout << "Unexpected Error: Please try again and enter only numerical values.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        break;
    }
    Product* prod = db.getProductById(prodID);
    if (prod == nullptr) {
        cout << "Product not found. Cancelling.\n\n";
        return;
    }
    else {
        prod->displayProduct();
        cout << "Product found; Continue deleting?\n1. Yes\n2. No\n\nEnter Choice: ";
        int confirm;
        while (true) {
            if (!(cin >> confirm) || (confirm != 1 && confirm != 2)) {
                cout << "Invalid input. Please enter a number.\n\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            cin.ignore(1000, '\n');

            if (confirm == 1) {
                cout << "Deleting Product!\n\n";
                db.deleteProduct(prodID);
                cout << "Product Deleted.\n\n";
                break;
            } 
            else {
                cout << "Cancelled!\n\n";
                return;
            } 
        }

    }
    delete prod;

}

string generatePassword() {
    size_t length = 8;

    const string uppercase = "ABCDEFGHJKLMNPQRSTUVWXYZ";
    const string lowercase = "abcdefghijkmnpqrstuvwxyz";
    const string digits = "23456789";
    const string special = "!@#$%^&*()_+-=[]{}|:;<>,.?";

    unsigned seed = static_cast<unsigned>(time(nullptr));
    mt19937 generator(seed);

    string password;


    password += uppercase[generator() % uppercase.size()];
    password += lowercase[generator() % lowercase.size()];
    password += digits[generator() % digits.size()];
    password += special[generator() % special.size()];

    string allChars = uppercase + lowercase + digits + special;
    for (size_t i = 4; i < length; ++i) {
        password += allChars[generator() % allChars.size()];
    }

    shuffle(password.begin(), password.end(), generator);

    return password;
}

