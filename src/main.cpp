#include "Database/DatabaseManager.h"
#include "userClasses/User.h"
#include "userClasses/Seller.h"
#include "userClasses/Admin.h"
#include "userClasses/Customer.h"
#include "Portals/customerPortal.h"
#include "Portals/sellerPortal.h"
#include "Portals/adminPortal.h"
#include "main.h"
#include <iostream>
#include <string>

using namespace std;

void displayWelcome() {
    cout << R"(
   ____                                                                  
  / __/____ ___ _ __ _  ___ _ ___ ___   ___                              
 _\ \ / __// _ `//  ' \/ _ `//_ // _ \ / _ \ _                           
/___/ \__/ \_,_//_/_/_/\_,_/ /__/\___//_//_/( )                          
  __   __          __                __     |/                           
 / /_ / /  ___    / /___  ___ _ ___ / /_                                 
/ __// _ \/ -_)  / // -_)/ _ `/(_-</ __/                                 
\__//_//_/\__/  /_/ \__/ \_,_//___/\__/                                  
  __                  __           __                                    
 / /_ ____ __ __ ___ / /_ ___  ___/ /                                    
/ __// __// // /(_-</ __// -_)/ _  /                                     
\__//_/   \_,_//___/\__/ \__/ \_,_/                                      
        ___ ___ __ _                        __         _  __           __
 ___   / _// _// /(_)___  ___    ____ ___  / /_ ___ _ (_)/ /___  ____ / /
/ _ \ / _// _// // // _ \/ -_)  / __// -_)/ __// _ `// // // -_)/ __//_/ 
\___//_/ /_/ /_//_//_//_/\__/  /_/   \__/ \__/ \_,_//_//_/ \__//_/  (_)  
)" << endl;
}

void mainMenu(DatabaseManager& dbm) {
    int selecton;
    cout << "=============================\n";
    cout << "       Main Menu            \n";
    cout << "=============================\n";
    cout << "| 1.     Login             |\n";
    cout << "| 2.     Register          |\n";
    cout << "| 3.     Exit              |\n";
    cout << "=============================\n";
    cout << "Enter selection: ";
    while (true) {
        if (!(cin >> selecton) || selecton < 1 || selecton > 3) {
            cout << "Invalid selection. Please enter a number between 1 and 3.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore(1000, '\n');
        break;
    }
    switch (selecton) {
        case 1:
            cout << "Logging in...\n";
            login(dbm);
            break;
        case 2:
            cout << "Registering...\n";
            registerUser(dbm);
            break;
        case 3:
            cout << "Exiting...\n";
            exit(0);
            break;
    }
}

void login(DatabaseManager& dbm) {
    string uNameOrEmail, password;
    cout << "=============================\n";
    cout << "       Login Menu           \n";
    cout << "=============================\n";
    cout << "Enter username or email: ";
    getline(cin, uNameOrEmail);
    if (uNameOrEmail.empty()) {
        cout << "Error: Username or email cannot be empty.\n";
        return;
    }
    cout << "=============================\n";
    cout << "Enter password: ";
    getline(cin, password);
    if (password.empty()) {
        cout << "Error: Password cannot be empty.\n";
        return;
    }
    cout << "=============================\n";
    cout << "Logging in...\n";
    string hashedPassword = User::hashPassword(password);

    User* loggedInUser = dbm.authenticateUser(uNameOrEmail, hashedPassword);
    if (loggedInUser == nullptr) {
        cout << "Invalid username/email or password. Please try again.\n\n";
        return;
    }

    if (loggedInUser->getRole() == "customer") {
        Customer* loggedInCustomer = new Customer(loggedInUser->getUID(), loggedInUser->getFirstName(), 
            loggedInUser->getLastName(), loggedInUser->getUsername(), loggedInUser->getPassword(),
            loggedInUser->getEmail(), loggedInUser->getRole());
        delete loggedInUser; // Clean up the User object
        dbm.loadCustomerData(loggedInCustomer);
        launchCustomerPortal(*loggedInCustomer, dbm);
        delete loggedInCustomer; // Clean up the Customer object

    } else if (loggedInUser->getRole() == "seller") {
        Seller loggedInSeller(loggedInUser->getFirstName(), 
            loggedInUser->getLastName(), loggedInUser->getUsername(), loggedInUser->getPassword(),
            loggedInUser->getEmail());
        loggedInSeller.setUID(loggedInUser->getUID());
        delete loggedInUser;
        dbm.loadSellerData(&loggedInSeller);
        launchSellerPortal(loggedInSeller, dbm);
        return;
    } else if (loggedInUser->getRole() == "admin") {
        Admin loggedInAdmin(loggedInUser->getUID(), loggedInUser->getFirstName(), 
            loggedInUser->getLastName(), loggedInUser->getUsername(), loggedInUser->getPassword(),
            loggedInUser->getEmail(), loggedInUser->getRole());
        delete loggedInUser;
        launchAdminPortal(loggedInAdmin, dbm);
        return;
    } else {
        cout << "Error: Unknown user role.\n";
        return;
    }
}


void registerUser(DatabaseManager& dbm) {
    string fName, lName, uName, pw, em;
    cout << "=============================\n";
    cout << "       Register Menu        \n";
    cout << "=============================\n";

    while (true) {
        cout << "Enter first name: ";
        getline(cin, fName);

        if (fName.empty()) {
            cout << "Error: First name cannot be empty.\n";
            continue;
        }
        if (!User::validateName(fName)) {
            cout << "Error: First name contains invalid characters. Please try again.\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Enter last name: ";
        getline(cin, lName);

        if (lName.empty()) {
            cout << "Error: Last name cannot be empty.\n";
            continue;
        }
        if (!User::validateName(lName)) {
            cout << "Error: Last name contains invalid characters. Please try again.\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Enter username: ";
        getline(cin, uName);

        if (uName.empty()) {
            cout << "Error: Username cannot be empty.\n";
            continue;
        }
        if (!User::validateUsername(dbm, uName)) {
            cout << "Error: Username invalid or already taken. Please choose a different username.\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Enter password: ";
        getline(cin, pw);

        if (pw.empty()) {
            cout << "Error: Password cannot be empty.\n";
            continue;
        }
        if (!User::validatePassword(pw)) {
            cout << "Error: Password must be at least 8 characters long and contain at least one uppercase letter, "
                    "one lowercase letter, one digit, and one special character.\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Enter email: ";
        getline(cin, em);

        if (em.empty()) {
            cout << "Error: Email cannot be empty.\n";
            continue;
        }
        if (!User::validateEmail(dbm, em)) {
            cout << "Error: Invalid or already registered email. Please try again.\n";
            continue;
        }
        break;
    }

    char role;
    cout << "Would you like to register as a Customer or Seller? (C/S): ";
    while (true) {
        cin >> role;
        cin.ignore(1000, '\n');

        if (role == 'C' || role == 'c') {
            cout << "\nRegistering as Customer...\n";
            Customer newUser(fName, lName, uName, User::hashPassword(pw), em);
            dbm.insertUser(newUser);
            launchCustomerPortal(newUser, dbm);
            break;
        } 
        else if (role == 'S' || role == 's') {
            cout << "\nRegistering as Seller...\n";
            Seller newUser(fName, lName, uName, User::hashPassword(pw), em);
            dbm.insertUser(newUser);
            launchSellerPortal(newUser, dbm);
            break;
        } 
        else {
            cout << "Invalid input. Please enter C for Customer or S for Seller: ";
        }
    }
}

int main() {
    DatabaseManager dbm;
    displayWelcome();
    while (true) {
        mainMenu(dbm);
    }
    return 0;
}
