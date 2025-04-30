#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include "../userClasses/Seller.h"
#include "../UtilityClasses/PaymentMethod.h"
#include "../userClasses/Admin.h"
#include "../userClasses/User.h"
#include "../Products/Product.h"
#include "../UtilityClasses/Orders/Order.h"


using namespace std;

class Customer;
class OrderItem;
class CustomerOrder;

class DatabaseManager {
private:
    sqlite3* db;
    static constexpr const char* DATABASE_NAME = "src/Database/scamazon.db";

public:
    DatabaseManager();
    ~DatabaseManager();

    User* authenticateUser(const string& uNameOrEmail, const string& password); // Validate user credentials
    
    //Load Users
    void loadCustomerData(Customer* cust);  
    void loadSellerData(Seller* sllr); 
    void updateUser(const User& user); // Update user information
    void insertUser(User& newUser); // Add new user to database
    bool emailInUse(const string& email); // Check if email already exists
    bool usernameInUse(const string& uName); // Check if username already exists
    bool resetUserPassword(const string& uName, const string& em, const string& newPW);
    
    //Product Management
    vector<Product*> searchProducts(const string& keyword); // Find products by search term
    Product* getProductById(int productId); // Retrieve specific product
    void insertProduct(Product& newProd); // Add product to database
    void deleteProduct(int prodID); // Remove product from database
    void updateProduct(Product* prod);
    void updateProductInventory(int productId, int newInventory); // Update stock level

    vector<Product*> getProductsBySeller(int sellerId); // Get seller's products
    
    
    //Order Management
    void insertCustomerOrder(CustomerOrder* order); 
    vector<CustomerOrder*> getCustomerOrderHistory(int customerId);
    vector<OrderItem*> getOrderItems(const int& ordID); 
    CustomerOrder* retrieveCustomerOrder(int ordID); 
    vector<SellerOrder*> getSellerOrders(int sellerID); 
    vector<OrderItem*> getSellerOrderItems(int customerOrderID, int sellerID); // Get order items for a specific seller
    void insertOrderItem(OrderItem& newItem);
    void insertSellerOrder(int orderID, int sellerID, string orderDate, double total); // Add order to seller's history

    void updateSellerBalance(int sellerID, int earnings); 
    
    
    void insertPaymentMethod(PaymentMethod& pMethod); // Add payment method to database
    PaymentMethod* getPaymentMethod(int paymentID); // Retrieve specific payment method
    vector<PaymentMethod*> getPaymentMethods(int userID); // Get all payment methods for a user
    void deletePaymentMethod(int paymentID); // Remove payment method from database
    void updatePaymentMethod(PaymentMethod& payMeth); // Update payment method details
    vector<Address*> getAddresses(int userID); // Get all addresses for a user
    void deleteAddress(int addressID); // Remove address from database
    void insertAddress(Address& newAddress); // Add shipping address
    Address* getAddress(int addressID); // Retrieve specific address
    
    
    
    

private:
    string buildProductSearchQuery(const vector<string>& searchTerms);
    Product* createProductFromRow(
        const string& type,
        int id,
        const string& name,
        const string& description,
        double price,
        int inventory,
        int sellerId
    ); // Create product object from database row
};

#endif