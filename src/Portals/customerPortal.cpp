#include <iostream>
#include <vector>
#include <ios>    
#include "../userClasses/Customer.h"
#include "../Database/DatabaseManager.h"
#include "../Products/Product.h" 
#include "customerPortal.h"

using namespace std;

// Function to display the customer portal menu and handle user interaction
void launchCustomerPortal(Customer& customer, DatabaseManager& db) {
    if (customer.getRole() != "customer") {
        cerr << "Error: Cannot launch customer portal without a valid customer.\n";
        return;
    }
    displayWelcomeScreen();

    int choice = -1;
    while (choice != 6) {
        cout << "\n==========================\n";
        cout << "     Customer Portal      \n";
        cout << "==========================\n";
        cout << " Welcome, " << customer.getFirstName() << "!\n";
        cout << "--------------------------\n";
        cout << "1. View Profile\n";
        cout << "2. Update Profile\n";
        cout << "3. Browse Products\n";
        cout << "4. View Shopping Cart\n";
        cout << "5. View Order History\n";
        cout << "6. Logout\n";
        cout << "--------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 6.\n\n";
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            choice = -1; 
            continue;
        }

        cin.ignore(1000, '\n'); 

        switch (choice) {
            case 1:
                customer.viewProfile(db);
                cout << "\nPress Enter to return to the portal menu...";
                cin.get();
                break;
            case 2:
                customer.updateProfile(db);
                break;
            case 3: {
                string searchTerm;
                cout << "Enter search term (leave blank to browse all): ";
                getline(cin, searchTerm);

                vector<Product*> products = db.searchProducts(searchTerm);
                if (products.empty()) {
                    cout << "No products found matching your search.\n";
                } 
                else {
                    while (true){
                        Product* selectedProduct = Product::displayProductList(products);
                        if (selectedProduct) { //Check for nullptr
                            int addChoice;
                            cout << "\nAdd " << selectedProduct->getProductName() << " to cart?\n";
                            cout << "1. Yes\n2. No\n";
                            cout << "Enter choice: ";
                            if (cin >> addChoice && addChoice == 1) {
                                cin.ignore(1000, '\n');
                                customer.addToCart(selectedProduct);
                                break;
                            } 
                            else {
                                if (!cin) { 
                                    cin.clear();
                                    continue;
                                }
                            cin.ignore(1000, '\n');
                            cout << "Returning to product list...\n";
                            continue;
                            }
                        }
                    Product::deleteProductVector(products);
                    break;
                }
                cout << "\nPress Enter to return to the Main Menu...";
                cin.get();
                break;
            }
            }
            case 4:
                customer.viewCart(db); // viewCart handles interaction including checkout option
                break;
            case 5:
                customer.viewOrderHistory();
                 cout << "\nPress Enter to return to the portal menu...";
                 cin.get(); // Pause after viewing history
                break;
            case 6:
                cout << "Logging out...\n";
                displayLogoutScreen();
                return;
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 6.\n";
                break;
        }
        cout << endl; 
    }
}

void displayWelcomeScreen(){
    cout << "\n\n";
    cout << "  _      __    __                     __       \n";
    cout << " | | /| / /__ / /______  __ _  ___   / /____   \n";
    cout << " | |/ |/ / -_) / __/ _ \\/  ' \\/ -_) / __/ _ \\  \n";
    cout << " |__/|__/\\__/_/\\__/\\___/_/_/_/\\__/  \\__/\\___/  \n";
    cout << "   ____                                  ______\n";
    cout << "  / __/______ ___ _  ___ ____ ___  ___  / / / / \n";
    cout << " _\\ \\/ __/ _ `/  ' \\/ _ `/_ // _ \\/ _ \\/_/_/_/  \n";
    cout << "/___/\\__/\\_,_/_/_/_/\\_,_//__/\\___/_//_(_|_|_)   \n\n";
    
    cout << "Press enter to continue...\n\n";
    cin.get();
}

void displayLogoutScreen(){
    cout << "\n\n";
    cout << " ________             __                        ___        \n";
    cout << "/_  __/ /  ___ ____  / /__   __ _____  __ __   / _/__  ____\n";
    cout << " / / / _ \\/ _ `/ _ \\/  '_/  / // / _ \\/ // /  / _/ _ \\/ __/\n";
    cout << "/_/ /_//_/\\_,_/_//_/_/\\_\\   \\_, /\\___/\\_,_/  /_/ \\___/_/   \n";
    cout << "       __                  /___/                _ __  __   \n";
    cout << "  ___ / /  ___  ___  ___  (_)__  ___ _  _    __(_) /_/ /   \n";
    cout << " (_-</ _ \\/ _ \\/ _ \\/ _ \\/ / _ \\/ _ `/ | |/|/ / / __/ _ \\  \n";
    cout << "/___/_//_/\\___/ .__/ .__/_/_//_/\\_, /  |__,__/_/\\__/_//_/  \n";
    cout << "   ____      /_/  /_/          /___/     ______            \n";
    cout << "  / __/______ ___ _  ___ ____ ___  ___  / / / /            \n";
    cout << " _\\ \\/ __/ _ `/  ' \\/ _ `/_ // _ \\/ _ \\/_/_/_/             \n";
    cout << "/___/\\__/\\_,_/_/_/_/\\_,_//__/\\___/_//_(_|_|_)              \n\n";

    cout << "Press Enter to continue...";


    cin.get(); 
}
