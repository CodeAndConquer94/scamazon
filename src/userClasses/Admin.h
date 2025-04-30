#ifndef ADMIN_H
#define ADMIN_H
#include "User.h"
#include "../UtilityClasses/Orders/Order.h"

class DatabaseManager;
class CustomerOrder;

class Admin : public User {
    public:
        Admin(string fName, string lName, string uName, string pw, string em);
        Admin(int UID, string fName, string lName, string uName, string pw, string em, string rl);
        void resetUserPassword(DatabaseManager& db);
        void refundCustomer(DatabaseManager& db);
        void deleteProduct(DatabaseManager& db);

    };


string generatePassword();

#endif