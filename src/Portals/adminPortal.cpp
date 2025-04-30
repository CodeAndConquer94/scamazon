#include <iostream>
#include "adminPortal.h"
#include "../userClasses/Admin.h"
#include "../Database/DatabaseManager.h"

using namespace std;

// display the admin portal menu and handle user interaction
void launchAdminPortal(Admin& admin, DatabaseManager& db) {
    if (admin.getRole() != "admin") {
        cerr << "Error: Cannot launch admin portal without a valid admin user.\n";
        return;
    }
    displayAdminWelcomeScreen();

    int choice = -1;
    while (choice != 6) { 
        cout << "\n==========================\n";
        cout << "       Admin Portal       \n";
        cout << "==========================\n";
        cout << " Welcome, " << admin.getFirstName() << "!\n";
        cout << "--------------------------\n";
        cout << "1. View Profile\n";
        cout << "2. Update Profile\n";
        cout << "3. Reset User Password\n";
        cout << "4. Refund Customer Order Item\n";
        cout << "5. Delete Product\n";
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
                admin.viewProfile(db);
                cout << "\nPress Enter to return to the portal menu...";
                cin.get();
                break;
            case 2:
                admin.updateProfile(db);
                break;
            case 3:
                admin.resetUserPassword(db);
                break;
            case 4:
                admin.refundCustomer(db);
                break;
            case 5:
                admin.deleteProduct(db);
                break;
            case 6:
                cout << "Logging out...\n";
                displayAdminLogoutScreen();
                return;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 6.\n";
                break;
        }
        cout << endl;
    }
}


void displayAdminWelcomeScreen() {
    cout << "\n\n";
cout << "  _      __    __                             \n";
cout << " | | /| / /__ / /______  __ _  ___            \n";
cout << " | |/ |/ / -_) / __/ _ \\/  ' \\/ -_)           \n";
cout << " |__/|__/\\__/_/\\__/\\___/_/_/_/\\__/            \n";
cout << "   ____                                       \n";
cout << "  / __/______ ___ _  ___ ____ ___  ___        \n";
cout << " _\\ \\/ __/ _ `/  ' \\/ _ `/_ // _ \\/ _ \\       \n";
cout << "/___/\\__/\\_,_/_/_/_/\\_,_//__/\\___/_//_/       \n";
cout << "   ___     __      _      ______              \n";
cout << "  / _ |___/ /_ _  (_)__  / / / /              \n";
cout << " / __ / _  /  ' \\/ / _ \\/_/_/_/               \n";
cout << "/_/ |_\\_,_/_/_/_/_/_//_(_|_|_)                \n\n\n";
    cout << "Press enter to continue...";
    // Use ignore before get to clear potential leftover newline
    cin.get(); // Wait for user to press Enter
}

// Display a logout screen for the admin portal
void displayAdminLogoutScreen() {

    cout << "\n\n";
    cout << "  _____    __     __            __       \n";
    cout << " / ___/__ / /_   / /  ___ _____/ /__     \n";
    cout << "/ (_ / -_) __/  / _ \\/ _ `/ __/  '_/     \n";
    cout << "\\___/\\__/\\__/  /_.__/\\_,_/\\__/_/\\_\\      \n";
    cout << "  __                          __   ______\n";
    cout << " / /____     _    _____  ____/ /__/ / / /\n";
    cout << "/ __/ _ \\   | |/|/ / _ \\/ __/  '_/_/_/_/ \n";
    cout << "\\__/\\___/   |__,__/\\___/_/ /_/\\_(_|_|_)  \n\n\n";
    cout << "You have been logged out. Get back to work!\n";
    cout << "Press Enter to continue...";
    cin.get();
}