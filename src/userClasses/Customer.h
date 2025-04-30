#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "User.h"
#include "../Products/Product.h"
#include "../UtilityClasses/ShoppingCart.h"
#include "../UtilityClasses/Orders/CustomerOrder.h"
#include "../UtilityClasses/PaymentMethod.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;



class Customer : public User {
    private:
        vector<PaymentMethod*> paymentMethods;  // e.g., "Visa ****1234"
        ShoppingCart cart;
        vector<CustomerOrder*> orderHistory;
        vector<Address*> addresses;
    
    public:
        Customer(string fName, string lName, string uName, string pw, string em);//constructor to create a new customer
        Customer(int UID, string fName, string lName, string uName, string pw, string em, string role ); // constructor to create customer from database
        ~Customer();
    
        void addPaymentMethod(DatabaseManager& db);
        void viewPaymentMethods();
        void importPaymentMethods(const vector<PaymentMethod*>& payMethods);
        void removePaymentMethod(DatabaseManager& db);
        bool payMethExists(PaymentMethod* pm);
        PaymentMethod* selectPayment();

        Address* selectAddress();
        void viewAddressBook();
        void importAddressBook(const vector<Address*>& addrsBk);
        bool addressExists(const Address& newAdrs);
        Address* addAddress(DatabaseManager& db);
        void removeAddress(DatabaseManager& db);

        void addToCart(Product*);
        void viewCart(DatabaseManager& db);
        void placeOrder(DatabaseManager& db);

        void viewOrderHistory() const;
        void importOrderHistory(const vector<CustomerOrder*>& orders);

        void updateProfile(DatabaseManager& dbm) override;
        void viewProfile(DatabaseManager& dbm) override;
    };


#endif
    
    