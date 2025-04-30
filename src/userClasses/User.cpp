#include "User.h"
#include "../Database/DatabaseManager.h"
#include <string>
#include <algorithm>
#include <openssl/sha.h>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

// User class implementation
// Provides core functionality for user authentication and profile management

// Constructor for new users
User::User(string fName, string lName, string uName, string pw, string em){
    firstName = fName;
    lastName = lName;
    username = uName;
    password = pw;
    email = em;
}

// Constructor for loading existing users from database
User::User(int UID, string fName, string lName, string uName, string pw, string em, string rl){
    firstName = fName;
    lastName = lName;
    username = uName;
    password = pw;
    email = em;
    userID = UID;
    role = rl;
}

// Getter methods for accessing user attributes
string User::getUsername() const { return username; }
string User::getEmail() const { return email; }
string User::getFirstName() const { return firstName; }
string User::getLastName() const { return lastName; }
string User::getRole() const { return role; }
int User::getUID() const { return userID; }
string User::getPassword() const { return password; }

// Sets the user ID after database creation
void User::setUID(int UID){
    userID = UID;
}

// Change user password with verification of old password and validation of new password
void User::setPassword(DatabaseManager& dbm) {
    string oldPassword, newPassword, confirmPassword;

    // Step 1: Verify current password
    while (true) {
        cout << "Changing Password!\nEnter Current Password (Leave blank to cancel): ";
        getline(cin, oldPassword);
        cout << endl;

        if (oldPassword.empty()) {
            cout << "Password Change Canceled.\n\n";
            return;
        }

        string hashedOldPassword = hashPassword(oldPassword);
        if (hashedOldPassword != password) {
            cout << "Incorrect Password, please try again\n\n";
            continue;
        }

        break;
    }

    // Step 2: Enter and validate new password
    while (true) {
        cout << "Enter New Password (No spaces allowed): ";
        getline(cin, newPassword);
        cout << endl;

        if (!validatePassword(newPassword)) {
            cout << "Invalid password. Please try again.\n\n";
            continue;
        }

        cout << "Confirm New Password: ";
        getline(cin, confirmPassword);
        cout << endl;

        if (newPassword != confirmPassword) {
            cout << "Passwords do not match. Please try again.\n\n";
            continue;
        }

        break;
    }

    password = hashPassword(newPassword);
    dbm.updateUser(*this);
    cout << "Password successfully updated.\n\n";
}


// Display formatted user profile information
void User::viewProfile(DatabaseManager& dbm) {
    cout << setw(32) << setfill('_') << "_" << endl;
    cout << "|" << setw(7) << setfill(' ') << " " << "User Information" << setw(8) << right << "|" << endl;
    cout << "|" << setw(30) << setfill('-') << "-" << "|" << endl;
    cout << "|" << "First Name:" << setw(19) << setfill(' ') << right << firstName << "|" << endl;
    cout << "|" << setw(30) << setfill('-') << "-" << "|" << endl;
    cout << "|" << "Last Name:" << setw(20) << setfill(' ') << right << lastName << "|" << endl;
    cout << "|" << setw(30) << setfill('-') << "-" << "|" << endl;
    cout << "|" << "Username:" << setw(21) << setfill(' ') << right << username << "|" << endl;
    cout << "|" << setw(30) << setfill('-') << "-" << "|" << endl;
    cout << "|" << "Email:" << setw(24) << setfill(' ') << right << email << "|" << endl;
    cout << "|" << setw(30) << setfill('-') << "-" << "|" << endl;
    cout << "|" << "User ID:" << setw(22) << setfill(' ') << right << userID << "|" << endl;
    cout << setw(32) << setfill('-') << "-"  << endl;
}


// Update user's first name with validation
void User::setFirstName(DatabaseManager& dbm){
    string newFName;
    while (true) {
        cout << "Enter new First Name: ";
        getline(cin, newFName);
        cout << endl;
        
        newFName.erase(0,newFName.find_first_not_of(" \t\n\r\f\v"));
        newFName.erase(newFName.find_last_not_of(" \t\n\r\f\v") + 1);

        if (newFName.empty()) {
            cout << "First name cannot be empty. Please try again \n\n";
            continue;
        }

        if (!validateName(newFName)) {
            cout << "ERROR: Name fields must contain only letters, spaces, hyphens, or apostrophes.\n\n";
            continue;
        }

        break;
    }
    firstName = newFName;
    dbm.updateUser(*this);
    cout << "First name updated to: " << firstName << "\n\n";

}

// Update user's last name with validation
void User::setLastName(DatabaseManager& dbm){
    string newLName;
    while (true) {
        cout << "Enter new Last Name: ";
        getline(cin, newLName);
        cout << endl;
        
        newLName.erase(0,newLName.find_first_not_of(" \t\n\r\f\v"));
        newLName.erase(newLName.find_last_not_of(" \t\n\r\f\v") + 1);

        if (newLName.empty()) {
            cout << "Last name cannot be empty. Please try again \n\n";
            continue;
        }

        if (!validateName(newLName)) {
            cout << "ERROR: Name fields must contain only letters, spaces, hyphens, or apostrophes.\n\n";
            continue;
        }

        break;
    }
    lastName = newLName;
    dbm.updateUser(*this);
    cout << "Last name updated to: " << lastName << "\n\n";

}

// Update user's email with format validation and duplicate checking
void User::setEmail(DatabaseManager& dbm){
    string newEmail;
    while (true) {
        cout << "Enter new Email (Leave blank to cancel): ";
        getline(cin, newEmail);
        cout << endl;
        
        newEmail.erase(0,newEmail.find_first_not_of(" \t\n\r\f\v"));
        newEmail.erase(newEmail.find_last_not_of(" \t\n\r\f\v") + 1);

        if (newEmail.empty()) {
            cout << "Canceled email update. \n\n";
            return;
        }

        if (!validateEmail(dbm, newEmail)) {
            cout << "ERROR: Invalid email format. Please enter a valid email address.\n\n";
            continue;
        }

        break;
    }
    email = newEmail;
    dbm.updateUser(*this);
    cout << "Email updated to: " << email << "\n\n";

}

// Update username with availability check and format validation
void User::setUsername(DatabaseManager& dbm) {
    string newUname;
    while (true) {
        cout << "Enter new username (Leave blank to cancel): ";
        getline(cin, newUname);
        cout << endl;

        if (newUname.empty()) { 
            cout << "Cancelled username update. \n\n";
            return; }

        newUname.erase(0,newUname.find_first_not_of(" \t\n\r\f\v"));
        newUname.erase(newUname.find_last_not_of(" \t\n\r\f\v") + 1);

        if (!validateUsername(dbm, newUname)) {
            cout << "ERROR: Invalid username. Please try again.\n\n";
            continue;
        }
        break;
    }
    username = newUname;
    dbm.updateUser(*this);
    cout << "Username updated to: " << username << "\n\n";
}

// Interactive menu for updating user profile information
void User::updateProfile(DatabaseManager& dbm) {
    int choice = -1;

    while (true) {
        cout << "\n==========================\n";
        cout << "      Update Profile      \n";
        cout << "==========================\n";
        cout << "1. Change First Name\n";
        cout << "2. Change Last Name\n";
        cout << "3. Change Username\n";
        cout << "4. Change Email\n";
        cout << "5. Change Password\n";
        cout << "6. More Options\n";
        cout << "--------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 6.\n\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        cin.ignore(1000, '\n'); // clear trailing newline

        switch (choice) {
            case 1:
                setFirstName(dbm);
                break;
            case 2:
                setLastName(dbm);
                break;
            case 3:
                setUsername(dbm);
                break;
            case 4:
                setEmail(dbm);
                break;
            case 5:
                setPassword(dbm);
                break;
            case 6:
                cout << "Retrieving more options...\n\n";
                return;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 6.\n";
        }
    }
}


// Validate that name contains only allowed characters (letters, hyphens, apostrophes, spaces)
bool User::validateName(const string& name) {
    return all_of(name.begin(), name.end(), [](char c) {
        return isalpha(c) || c == '-' || c == '\'' || c == ' ';
    });
}

// Validate email format and check for duplicates in the database
bool User::validateEmail(DatabaseManager& db, const std::string& em) {
    // First check if email is already taken
    if (db.emailInUse(em)){
        cout << "Email already in use. Please try another email.\n\n";
        return false;
    }
    
    // Check for @ symbol in correct position
    auto atItr = std::find(em.begin(), em.end(), '@');

    if (atItr == em.end() || atItr == em.begin() || atItr + 1 == em.end()) {
        return false;
    }

    // Validate characters before @ symbol
    bool characterCheck = std::all_of(em.begin(), atItr, [](char c) {
        return std::isalnum(c) || c == '.' || c == '_' || c == '-';
    });


    bool domainCheck1 = std::all_of(atItr + 1, em.end(), [](char c) {
        return std::isalnum(c) || c == '.' || c == '-';
    });

    // Domain must contain at least one dot (e.g., .com, .org)
    bool domainCheck2 = std::any_of(atItr + 1, em.end(), [](char c) {
        return c == '.';
    });

    // All checks must pass
    return characterCheck && domainCheck1 && domainCheck2;
}

// Validate username for format rules and uniqueness
bool User::validateUsername(DatabaseManager& db, const string& uname) {
    // Check if username already exists
    if (db.usernameInUse(uname)) {
        cout << "Username already in use. Please try another username. \n\n";
        return false;
    }
    
    // Check length requirements
    if (uname.length() < 3 || uname.length() > 20) return false;

    //must start and end with a number or letter
    if (!std::isalnum(uname.front()) || !std::isalnum(uname.back())){
        cout << "Username must start and end with either a number or a letter.\n\n";
        return false;
    }

    // Check allowed characters and disallow consecutive . or _
    for (size_t i = 0; i < uname.length(); ++i) {
        char c = uname[i];

        // Only allow alphanumeric, underscore, and period
        if (!(std::isalnum(c) || c == '_' || c == '.')){
            return false;
        }

        // Prevent consecutive special characters
        if (i > 0) {
            char prev = uname[i - 1];
            if ((c == '.' && prev == '.') || (c == '_' && prev == '_')){
                cout << "Consecutive \'.\' or \'_\' is not allowed. \n\n";
                return false;
            }
        }
    }

    return true;
}

// Validate password meets security requirements (length, complexity)
bool User::validatePassword(const string& pwd) {
    // Check minimum length requirement
    if (pwd.length() < 8) {
        std::cout << "ERROR: Password must be at least 8 characters long.\n";
        return false;
    }

    // No spaces allowed in password
    if (pwd.find(' ') != std::string::npos) {
        std::cout << "ERROR: Password cannot contain spaces.\n";
        return false;
    }

    // Track different character types
    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    // Check each character to ensure complexity requirements
    for (char c : pwd) {
        if (std::isupper(c)) hasUpper = true;
        else if (std::islower(c)) hasLower = true;
        else if (std::isdigit(c)) hasDigit = true;
        else if (std::ispunct(c)) hasSpecial = true;
    }

    // Verify all complexity requirements are met
    if (!hasUpper) {
        std::cout << "ERROR: Password must contain at least one uppercase letter.\n";
        return false;
    }

    if (!hasLower) {
        std::cout << "ERROR: Password must contain at least one lowercase letter.\n";
        return false;
    }

    if (!hasDigit) {
        std::cout << "ERROR: Password must contain at least one digit.\n";
        return false;
    }

    if (!hasSpecial) {
        std::cout << "ERROR: Password must contain at least one special character (e.g., !, @, #).\n";
        return false;
    }

    // All validation checks passed
    return true;
}

// Generate SHA-256 hash of the password
string User::hashPassword(const string& pwd) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(pwd.c_str()), pwd.size(), hash);

    // Convert binary hash to hexadecimal string representation
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i){
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}