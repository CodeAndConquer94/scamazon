#include "Seller.h"
#include "User.h"
#include "../Products/Product.h"
#include "../UtilityClasses/Orders/SellerOrder.h"
#include "../Database/DatabaseManager.h"
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;


Seller::Seller(string fName, string lName, string uName, string pw, string em): 
    User(fName, lName, uName, pw, em), products{}, orders{}, depositAccountBalance(0.0) {
    role = "seller"; // Set role to seller
    } // Constructor for new seller


Seller::Seller(int UID, string fName, string lName, string uName, string pw, string em, string role, double acctBalance):
    User(UID, fName, lName, uName, pw, em, role), products{}, orders{} {
        depositAccountBalance = acctBalance;
    }
Seller::~Seller(){
    Product::deleteProductVector(products);
    for (SellerOrder* so: orders){
        delete so;
    }
    orders.clear();
}
   
void Seller::addProduct(DatabaseManager& dbm) { // Create and add new product to inventory
    cout << "\n--- Adding New Product ---\n";

    Product* newProduct = new Product(getUID());
    cin >> *newProduct; // Use overloaded operator to input product details
    for (Product* p : products) {
        if (*p == *newProduct) {
            cout << "Product already exists. Cancelling.\n\n";
            delete newProduct;
            return;
        }
    }
    dbm.insertProduct(*newProduct); // Insert product into database
    products.push_back(newProduct);

    cout << "\nProduct \"" << newProduct->getProductName() << "\" added successfully!\n";
}

void Seller::importSellerProducts(const vector<Product*>& prods){
    products = prods;
}


Product* Seller::viewAllProducts(){
    // Display all products and return selected product
    Product* selectedProduct = Product::displayProductList(products);
    return selectedProduct;
}

void Seller::deleteProduct(DatabaseManager& db, int productId){
    // Remove product from database and local inventory
    db.deleteProduct(productId);
    for (auto it = products.begin(); it != products.end(); ++it){
        if ((*it)->getProductID() == productId) {
            delete *it;
            products.erase(it);
            break;
        }
    }
}


void Seller::viewInventory() const { // Display inventory overview with product counts
    cout << "==============================================" << endl;
    cout << "           Seller Inventory Overview          " << endl;
    cout << "==============================================" << endl;
    cout << left << setw(10) << "ID" << setw(25) << "Product Name" << setw(10) << "Inventory" << endl;
    cout << "----------------------------------------------" << endl;

    for (const Product* p : products) {
        cout << left << setw(10) << setfill(' ') << p->getProductID() << setw(25) << p->getProductName() << setw(10) << p->getInventory() << endl;
    }

    cout << "==============================================" << endl;
}


void Seller::viewSalesHistory() { // Show sales history and allow selection of specific order
    if (orders.empty()) {
        cout << "No sales have been recorded yet." << endl;
        return;
    }

    auto it = orders.begin();
    int orderNumber = 1;
    int page = 1;
    int pages = (orders.size() + 9) / 10;
    while (it < orders.end()){
        cout << "Page " << page << " of " << pages << endl;
        for (auto it2 = it; it2 < it + 10 && it2 != orders.end() ; ++it2) {

            cout << left << "|---------Order Number: " << setw(4) << setfill(' ') << orderNumber++ << "---------|\n";
            cout << "Order ID: " << (*it2)->getOrderID() << " Order Date: " << (*it2)->getOrderDate().dateString() << " Total: " << (*it2)->getTotal() << endl;
        }
        string selection;
        while (true){
            cout << "Select an Option:\n[N] Next Page\n[P] Previous Page\n[Ord Num] View Order Summary\n[Q] Quit\n\nEnter Selection: ";
            if (!(cin >> selection) || selection != "N" && selection != "P" && selection != "Q" &&
             !Product::isDigits(selection) && selection != "n" && selection != "p" && selection != "q"){
                cout << "Unexpected Error: Please Try again and enter N, P, Q, or product number.\n\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            break;
        }
        if (selection == "N" || selection == "n"){
            if (page == pages) {
                cout << "Sorry! There are no more orders to show!\n\n";
                cin.ignore(1000,'\n');
                continue;
            }
            it = it + 10;
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
            orderNumber = (page-1) * 10 +1;
            it = it - 10;
            continue;
        }
        else if (selection == "Q" || selection == "q"){
            cout << "Quitting!\n\n";
            cin.ignore(1000, '\n');
            return;
        }
        else if (Product::isDigits(selection) && stoi(selection) <= orderNumber && stoi(selection) > ((page-1) * 10)){
            orders[stoi(selection)-1]->displayOrderSummary();
            char cont;
            cout << "Press any key to continue.\n\n";
            cin >> cont;
            cin.clear();
            cin.ignore();
            continue;          

        }
        
        

    }
    return;
}

void Seller::importSellerOrders(const vector<SellerOrder*>& ords){
    orders = ords;
}
    



void Seller::withdraw(DatabaseManager& dbm, double amount){
    // Remove funds from account if sufficient balance exists
    if (amount <= depositAccountBalance){
    depositAccountBalance -= amount;
    }
    else {
        cout << "Insufficient funds: aborting.";
    }
    
    dbm.updateSellerBalance(getUID(), -amount);
    dbm.updateUser(*this);
    cout << "Withdrawal of $" << fixed << setprecision(2) << amount << " successful.\n";
    cout << "New balance: $" << fixed << setprecision(2) << depositAccountBalance << "\n\n";
    cout << "==============================================" << endl;
}

double Seller::getBalance() const{ return depositAccountBalance; } // Return current account balance

void Seller::setBalance(double balance){
    depositAccountBalance = balance;
}

void Seller::viewProfile(DatabaseManager& dbm) {
    User::viewProfile(dbm);
    
    cout << "\n===== Seller Summary =====\n\n";
    cout << "Product Listings: " << products.size() << " products\n";
    cout << "Sales History: " << orders.size() << " orders\n";
    cout << "Account Balance: $" << fixed << setprecision(2) << depositAccountBalance << "\n\n";
    
    cout << "===== Seller Options =====\n";
    cout << "1. View Inventory\n";
    cout << "2. Update Product\n";
    cout << "3. View Sales History\n";
    cout << "4. Withdraw Funds\n";
    cout << "5. Return to Main Menu\n\n";
    
    int choice;
    while (true) {
        cout << "Enter choice: ";
        if (!(cin >> choice) || choice < 1 || choice > 4) {
            cout << "Invalid choice. Please enter a number between 1 and 4.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        break;
    }
    
    switch (choice) {
        case 1:
            viewInventory();
            break;

        case 2: {
            Product* selectedProduct = viewAllProducts();
            if (selectedProduct != nullptr) {
                updateProduct(selectedProduct, dbm);
            } else {
                cout << "No product selected.\n";
            }
            break;
        }
        case 3:
            viewSalesHistory();
            break;
        case 4: {
            double amount;
            cout << "\nCurrent balance: $" << fixed << setprecision(2) << depositAccountBalance << "\n";
            cout << "Enter amount to withdraw: $";
            while (true) {
                if (!(cin >> amount) || amount <= 0) {
                    cout << "Invalid amount. Please enter a positive number.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }
                if (amount > depositAccountBalance) {
                    cout << "Insufficient funds. Maximum withdrawal: $" 
                         << fixed << setprecision(2) << depositAccountBalance << "\n";
                    continue;
                }
                cin.ignore(1000, '\n');
                break;
            }
            withdraw(dbm, amount);
            cout << "Withdrawal successful. New balance: $" 
                 << fixed << setprecision(2) << depositAccountBalance << "\n\n";
            break;
        }
        case 5:
            cout << "Returning to main menu...\n\n";
            break;
    }
}

void Seller::updateProfile(DatabaseManager& dbm) {
    User::updateProfile(dbm);
    
    cout << "===========================\n";
    cout << "   Seller Profile Update   \n";
    cout << "===========================\n";
    cout << "| 1.    Add Product       |\n";
    cout << "| 2.   Delete Product     |\n";
    cout << "| 3.   Withdraw Funds     |\n";
    cout << "| 4.    Cancel Update     |\n";
    cout << "===========================\n";
    
    int selection;
    while (true) {
        cout << "Enter Selection: ";
        if (!(cin >> selection) || selection < 1 || selection > 4) {
            cout << "Unexpected Error: Please Try again and enter a number 1-4.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        break;
    }
    
    switch (selection) {
        case 1: 
            addProduct(dbm);
            break;
        case 2: {
            Product* toDelete = viewAllProducts();
            if (toDelete != nullptr) {
                cout << "Are you sure you want to delete: " << toDelete->getProductName() << "?\n";
                cout << "1. Yes\n2. No\n\nEnter Choice: ";
                int confirm;
                if (cin >> confirm && confirm == 1) {
                    deleteProduct(dbm, toDelete->getProductID());
                    cout << "Product deleted successfully.\n\n";
                } else {
                    cout << "Deletion cancelled.\n\n";
                }
                cin.ignore(1000, '\n');
            }
            break;
        }
        case 3: {
            double amount;
            cout << "\nCurrent balance: $" << fixed << setprecision(2) << depositAccountBalance << "\n";
            cout << "Enter amount to withdraw: $";
            while (true) {
                if (!(cin >> amount) || amount <= 0) {
                    cout << "Invalid amount. Please enter a positive number.\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }
                if (amount > depositAccountBalance) {
                    cout << "Insufficient funds. Maximum withdrawal: $" 
                         << fixed << setprecision(2) << depositAccountBalance << "\n";
                    continue;
                }
                cin.ignore(1000, '\n');
                break;
            }
            withdraw(dbm, amount);
            cout << "Withdrawal successful. New balance: $" 
                 << fixed << setprecision(2) << depositAccountBalance << "\n\n";
            break;
        }
        case 4:
            cout << "Update cancelled.\n\n";
            break;
    }
}

void Seller::updateProduct(Product* prod, DatabaseManager& db){
    int choice;
    while (true) {
        prod->displayProduct();
        cout << "================================\n";
        cout << "       Update Product Menu      \n";
        cout << "================================\n";
        cout << "1.     Update Product Name      \n";
        cout << "2.  Update Product Description  \n";
        cout << "3.         Update Price         \n";
        cout << "4.         Update Stock         \n";
        cout << "5.         Done/Cancel          \n";
        cout << "================================\n";
        cout << "Enter selection: ";
        if (!(cin >> choice) || choice < 1 || choice > 5){
            cout << "Unexpected Error: Please enter a number 1-5.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        switch (choice){
            case 1: 
                prod->setProductName(); 
                db.updateProduct(prod); 
                cout << "Product Name Updated to: " << prod->getProductName() << endl;
                break;
            case 2: 
                prod->setProductDescription(); 
                db.updateProduct(prod); 
                cout << "Product Description updated to: \n";
                cout << prod->getProductDescription();
                break;
            case 3: 
                prod->setProductPrice();
                db.updateProduct(prod);
                cout << "Product Price Updated to: $" << fixed << setprecision(2) << prod->getPrice() << endl;
                break;
            case 4:
                int qty;
                while (true){
                    cout << "Enter Quantity to add: ";
                    if (!(cin >> qty) || qty < 1){
                        cout << "Unexpected Error: Please Try again and enter a positive whole number.\n\n";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        continue;
                    }
                    break;                 
                }
                prod->updateStock(qty);
                db.updateProductInventory(prod->getProductID(), prod->getInventory());
                cout << "Updated Product inventory: " << prod->getInventory() << endl;
                break;
            case 5:
                cout << "Returning to previous menu...\n\n";
                return;
        }
        cout << "\n";
    }
    
}
