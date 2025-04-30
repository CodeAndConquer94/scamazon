#include "Product.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;
// Constructor 
Product::Product(int sellerID) : sellerID(sellerID), productId(0), name(""), description(""), type(""), price(0.0), inventoryCount(0) {}
// Constructor for loading from database
Product::Product(int SID, int prodID, string nm, string dscrptn, string tp, double prc, int invtry) 
    : sellerID(SID), productId(prodID), name(nm), description(dscrptn), type(tp), price(prc), inventoryCount(invtry) {}
// Getters
string Product::getType() { return type; }
double Product::getPrice() const { return price; }
int Product::getInventory() const { return inventoryCount; }
int Product::getProductID() const { return productId; }
string Product::getProductName() const { return name; }
int Product::getSellerID() const { return sellerID; }
string Product::getProductDescription() { return description; }
// Setters
void Product::setProductName() {
    int choice;
    while (true) {
        cout << "Changing product name.\n\n1. Continue\n2. Cancel.\n\nEnter Choice: ";
        if(!(cin >> choice)) {
            cout << "Unexpected Error: Please Try again.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
            }
        else if (choice == 1) { break; }
        else if (choice == 2) {
            cout << "Canceled!\n\n";
            cin.ignore(1000, '\n');
            return;
            }
        else {
            cout << "Invalid input: Please try again and enter a 1 or a 2.\n\n.";
            cin.ignore(1000, '\n');
            continue;
            }
    }
    cout << "Enter product name: ";
    cin.ignore();
    getline(cin, name);
}
void Product::setProductID(int prodID) {
    productId = prodID;
}
void Product::setProductDescription() {
    int choice;
    while (true) {
        cout << "Changing product description.\n\n1. Continue\n2. Cancel.\n\nEnter Choice: ";
        if(!(cin >> choice)) {
            cout << "Unexpected Error: Please Try again.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
            }
        else if (choice == 1) { break; }
        else if (choice == 2) {
            cout << "Canceled!\n\n";
            cin.ignore(1000, '\n');
            return;
            }
        else {
            cout << "Invalid input: Please try again and enter a 1 or a 2.\n\n";
            cin.ignore(1000, '\n');
            continue;
            }
    }
    while (true) {
        int confirm;
        cout << "Enter product description: ";
        cin.ignore(1000, '\n');
        string temp;
        getline(cin, temp);
        temp = formatDescription(temp);
        while (true) {
            cout << "UPDATE DESCRIPTION TO:\n" << temp << "\n\n1. Confirm\n2. Try again.\n3. Cancel.\n";
            if (!(cin >> confirm) || confirm < 1 || confirm > 3) {
                cout << "Unexpected Error: Please try again and enter a number 1-3\n\n";
                cin.clear();
                cin.ignore(1000,'\n');
                continue;
            }
            else { break; }
        }
        if (confirm == 1) {
            description = temp;
            cout << "Description successfully updated!\n\n";
            cin.ignore(1000,'\n');
            return;
        }
        else if (confirm == 2) {
            cout << "Trying again!\n\n";
            cin.ignore(1000,'\n');
            continue;
        }
        else if (confirm == 3){
            cout << "Cancelled!\n\n";
            cin.ignore(1000, '\n');
            return;
        }

    }
}
void Product::setProductPrice() {
    int choice;
    while (true) {
        cout << "Changing product price.\n\n1. Continue\n2. Cancel.\n\nEnter Choice: ";
        if(!(cin >> choice)) {
            cout << "Unexpected Error: Please Try again.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
            }
        else if (choice == 1) { break; }
        else if (choice == 2) {
            cout << "Canceled!\n\n";
            cin.ignore(1000, '\n');
            return;
            }
        else {
            cout << "Invalid input: Please try again and enter a 1 or a 2.\n\n.";
            cin.ignore(1000, '\n');
            continue;
            }
    }
    while (true) {
        double newPrice;
        cout << "Enter new product price: $";
        if (!(cin >> newPrice)) {
            cout << "Unexpected Error: Enter Price in the format shown in the example \"29.99\"\n\n";
            cin.clear();
            cin.ignore(1000,'\n');
            continue;
        }
        else {
            price = newPrice;
            cout << "Success: Price updated to: $" << price << "\n\n";
            cin.ignore(1000, '\n');
            return;
        }

    }
}
void Product::updateStock(int qty) {
    if (inventoryCount + qty < 0) {
        cout << "Error: Cannot reduce inventory below 0" << endl;
        return;
    }
    inventoryCount += qty;
}
//Display name, type, and price of product in table
void Product::displayProductHeader() {
    cout << string(60, '-') << endl;
    cout << setw(30) << left << name
         << setw(15) << left << type 
         << "$" << setw(14) << right << fixed << setprecision(2) << price 
         << endl;
    cout << string(60, '-') << endl;
}
//Display product details
void Product::displayProduct() {
    cout << "Product ID: " << productId << endl;
    cout << "Seller ID: " << sellerID << endl;
    cout << "Product Name: " << name << endl;
    cout << "Type: " << type << endl;
    cout << "Price: $" << fixed << setprecision(2) << price << endl;
    cout << "Inventory Count: " << inventoryCount << endl;
    cout << "Description: " << description << endl;
}
//Format description to fit in 80 characters
string Product::formatDescription(string& str) {
    string formattedStr;
    int lineLength = 0;
    istringstream iss(str);
    string word;
    while (iss >> word) {
        if (lineLength + word.length() > 80) {
            formattedStr += "\n";
            lineLength = 0;
        }
        formattedStr += word + " ";
        lineLength += word.length() + 1; // +1 for space
    }
    return formattedStr;
}
// Overloaded operators
bool Product::operator==(Product& other) {
    return productId == other.productId &&
           name == other.name &&
           description == other.description &&
           type == other.type &&
           price == other.price;
}
// Input operator
istream& operator>>(istream& is, Product& prdct) {
    string name, description, type;
    double price = -1.0;
    int inventory = -1;
    // Input validation for product name, description, type, price, and inventory
    while (true) {
        cout << "Enter product name: ";
        getline(cin, name);
        if (name.empty()) {
            cout << "Product name cannot be empty.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Enter product description: ";
        getline(cin, description);
        if (description.empty()) {
            cout << "Product description cannot be empty.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Enter product type/category: ";
        getline(cin, type);
        if (type.empty()) {
            cout << "Product type cannot be empty.\n";
        } else {
            break;
        }
    }

    while (true) {
        cout << "Enter price: ";
        if (cin >> price && price >= 0) {
            break;
        } else {
            cout << "Invalid input. Price must be a non-negative number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    while (true) {
        cout << "Enter inventory count: ";
        if (cin >> inventory && inventory >= 0) {
            break;
        } else {
            cout << "Invalid input. Inventory must be a non-negative integer.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    
    // Update the product object with collected data
    prdct.name = name;
    prdct.description = prdct.formatDescription(description);
    prdct.type = type;
    prdct.price = price;
    prdct.inventoryCount = inventory;

    cin.ignore(1000, '\n');
    return is;
}

Product* Product::displayProductList(vector<Product*>& prdcts) {
    auto it = prdcts.begin();
    int prodNumber = 1;
    int page = 1;
    int pages = (prdcts.size() + 19) / 20;
    while (it < prdcts.end()){
        cout << "Page " << page << " of " << pages << endl;
        cout << left << setw(10) << setfill(' ') << "Prod. Num" << setw(30) << left << "Product Name" << setw(15) << left << "Type" << setw(15) << right << "Price" << endl;
        for (auto it2 = it; it2 < it + 20 && it2 != prdcts.end() ; ++it2) {
            cout << "Product Number: " << left << setw(10) << prodNumber++ << endl;
            (*it2)->displayProductHeader();
        }
        string selection;
        while (true){
            cout << "Select an Option:\n[N] Next Page\n[P] Previous Page\n[Prod Num] Select Product\n[Q] Quit\n\nEnter Selection: ";
            if (!(cin >> selection) || selection != "N" && selection != "P" && selection != "Q" &&
             !isDigits(selection) && selection != "n" && selection != "p" && selection != "q"){
                cout << "Unexpected Error: Please Try again and enter N, P, Q, or product number.\n\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            break;
        }

        if (selection == "N" || selection == "n"){
            if (page == pages) {
                cout << "Sorry! There are no more products to show!\n\n";
                cin.ignore(1000,'\n');
                continue;
            }
            it = it + 20;
            page++;
            continue;
        }
        else if (selection == "P" || selection == "p"){
            if (page == 1){
                cout << "Error: Already on the first page.\n\n";
                cin.ignore(1000, '\n');
                continue;
            }
            page--;
            prodNumber -= 40;
            it = it - 20;
            continue;
        }
        else if (selection == "Q" || selection == "q"){
            cout << "Quitting!\n\n";
            cin.ignore(1000, '\n');
            return nullptr;
        }
        else if (isDigits(selection) && stoi(selection) <= prodNumber && stoi(selection) > ((page-1) * 20)){
            prdcts[stoi(selection)-1]->displayProduct();
            int choice;
            while (true) {
                cout << "\nSelect an option\n1. Select Product\n2. Return to products\n\nEnter selection: ";
                if (!(cin >> choice) || choice < 1 || choice > 2){
                    cout << "Unexpected error: Please try again and enter 1 or 2.\n\n";
                    cin.clear();
                    cin.ignore(1000,'\n');
                    continue;
                }
                break;
            }
            if (choice == 1){
                return prdcts[stoi(selection)-1];
            }
            else if (choice == 2) {
                continue;
            }

        }
        
        

    }
    return nullptr;
}
void Product::deleteProductVector(vector<Product *> &prdcts){
    for (Product* p: prdcts){
        delete p;
    }
    prdcts.clear();
}

bool Product::isDigits(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}



