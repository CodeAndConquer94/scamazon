#ifndef SELLER_H
#define SELLER_H
#include "User.h"
#include "../Products/Product.h"
#include "../UtilityClasses/Orders/SellerOrder.h"
#include <string>
#include <vector>

using namespace std;

class SellerOrder;

class Seller : public User {
    private:
        vector<Product*> products; // Seller's product inventory
        vector<SellerOrder*> orders; // Seller's order history
        double depositAccountBalance; // Seller's account balance
    
    public:
        Seller(string fName, string lName, string uName, string pw, string em);
        Seller(int UID, string fName, string lName, string uName, string pw, string em, string role, double acctBalance);
        ~Seller();
    
        void addProduct(DatabaseManager& db);
        void importSellerProducts(const vector<Product*>& prods);
        Product* viewAllProducts();
        void deleteProduct(DatabaseManager& db, int productId);
        void updateProduct(Product* prod, DatabaseManager& db);
        void viewInventory() const;
        void viewSalesHistory();
        void importSellerOrders(const vector<SellerOrder*>& orders);
        void withdraw(DatabaseManager& dbm, double amount);
        double getBalance() const;
        void setBalance(double balance);
        void viewProfile(DatabaseManager& dbm) override;
        void updateProfile(DatabaseManager& dbm) override;

    };

#endif