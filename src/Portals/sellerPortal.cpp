#include <iostream>
#include <ios>
#include "sellerPortal.h"
#include "../userClasses/Seller.h"
#include "../Database/DatabaseManager.h"


using namespace std;


void launchSellerPortal(Seller& seller, DatabaseManager& db) {
    if (seller.getRole() != "seller") {
        cerr << "Error: Cannot launch seller portal without a valid seller.\n";
        return;
    }
    displaySellerWelcomeScreen();

    int choice = -1;
    while (choice != 4) {
        cout << "\n==========================\n";
        cout << "       Seller Portal      \n";
        cout << "==========================\n";
        cout << " Welcome, " << seller.getFirstName() << "!\n";
        cout << "--------------------------\n";
        cout << "1. View Profile & Manage Store\n"; // Combines view profile, inventory, sales, withdraw
        cout << "2. Update Profile\n";
        cout << "3. Add New Product\n";
        cout << "4. Logout\n";
        cout << "--------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 4.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
            continue;
        }

        cin.ignore(1000, '\n');

        switch (choice) {
            case 1:
                seller.viewProfile(db); // This method handles its own sub-menu and pauses
                break;
            case 2:
                seller.updateProfile(db);// separate sub-menu for this handles within seller class.
                cout << "\nPress Enter to return to the portal menu...";
                cin.get();
                break;
            case 3:
                seller.addProduct(db);
                cout << "\nPress Enter to return to the portal menu...";
                cin.get();
                break;
            case 4:
                cout << "Logging out...\n";
                displaySellerLogoutScreen();
                return;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4.\n";
                break;
        }
         cout << endl;
    }
}

//display a welcome screen for the seller portal
void displaySellerWelcomeScreen() {
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


//display a logout screen for the seller portal
void displaySellerLogoutScreen() {
    cout << "\n\n";
    cout << " ________             __                        ___        \n";
    cout << "/_  __/ /  ___ ____  / /__   __ _____  __ __   / _/__  ____\n";
    cout << " / / / _ \\/ _ `/ _ \\/  '_/  / // / _ \\/ // /  / _/ _ \\/ __/\n";
    cout << "/_/ /_//_/\\_,_/_//_/_/\\_\\   \\_, /\\___/\\_,_/  /_/ \\___/_/   \n";
    cout << "  __               __  _   /___/                           \n";
    cout << " / /_______ _____ / /_(_)__  ___ _                         \n";
    cout << "/ __/ __/ // (_-</ __/ / _ \\/ _ `/                         \n";
    cout << "\\__/_/  \\_,_/___/\\__/_/_//_/\\_, /                          \n";
    cout << "   ____                    /___/         ______            \n";
    cout << "  / __/______ ___ _  ___ ____ ___  ___  / / / /            \n";
    cout << " _\\ \\/ __/ _ `/  ' \\/ _ `/_ // _ \\/ _ \\/_/_/_/             \n";
    cout << "/___/\\__/\\_,_/_/_/_/\\_,_//__/\\___/_//_(_|_|_)              \n\n";
    cout << "(Thank you for trusting Scamazon, you really shouldn't have.)\n";
    cout << "You have been logged out. Thank you!\n";
    cout << "Press Enter to continue...";

    cin.get();
}
