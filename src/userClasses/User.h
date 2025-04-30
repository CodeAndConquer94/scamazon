#ifndef USER_H
#define USER_H
#include <string>
#include <iostream>

using namespace std;

class DatabaseManager;

class User {
    protected:
        string firstName; // User's first name
        string lastName;  // User's last name
        string username;  // Unique username for authentication
        string password;  // Hashed password for authentication
        string email;     // User's email address
        string role;      // User type (customer, seller, admin)
        int userID;       // Unique database identifier
        
    
    public:
        User(string fName, string lName, string uName, string pw, string em); // Constructor for new user
        User(int UID, string fName, string lName, string uName, string pw, string em, string rl); // Constructor for loading from database

        // Profile update methods
        void setFirstName(DatabaseManager& dbm); // Update first name with validation
        void setLastName(DatabaseManager& dbm);  // Update last name with validation
        void setUsername(DatabaseManager& dbm);   // Update username with availability check
        void setEmail(DatabaseManager& dbm);      // Update email with format validation
        void setPassword(DatabaseManager& dbm);   // Change password with validation
        void setUID(int UID);                     // Set user ID (typically from database)

        // Profile management
        virtual void viewProfile(DatabaseManager& dbm);                             // Display user information
        virtual void updateProfile(DatabaseManager& dbm); // Menu for updating profile

        // Accessor methods
        string getRole() const;      // Get user's role (customer/seller/admin)
        int getUID() const;          // Get user's unique ID
        string getUsername() const;   // Get user's username
        string getEmail() const;      // Get user's email address
        string getFirstName() const;  // Get user's first name
        string getLastName() const;   // Get user's last name
        string getPassword() const;
        
        
        // Validation methods
        static string hashPassword(const string& pwd);       // Convert password to secure hash
        static bool validateUsername(DatabaseManager& db, const string& uname);    // Verify username format and availability
        static bool validateEmail(DatabaseManager& db, const string& email);      // Validate email format and availability
        static bool validateName(const string& name);        // Verify name contains only valid characters
        static bool validatePassword(const string& pword);   // Check password complexity requirements

        virtual ~User() = default;
    };



#endif