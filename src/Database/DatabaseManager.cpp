#include <string>
#include <vector>
#include <sqlite3.h>
#include "../userClasses/Seller.h"
#include "../UtilityClasses/PaymentMethod.h"
#include "../userClasses/Admin.h"
#include "../userClasses/User.h"
#include "../Products/Product.h"
#include "../userClasses/Customer.h"
#include "../UtilityClasses/Address.h"
#include <sstream>

using namespace std;
    

DatabaseManager::DatabaseManager(){// Initialize database connection
    int result = sqlite3_open(DATABASE_NAME, &db);//sqlite3_open returns an integer status code

    if (result != SQLITE_OK) {
        string errorMessage = sqlite3_errmsg(db);
        sqlite3_close(db);
        throw runtime_error("Failed to connect to database: " + errorMessage);
    }
    cout << "Database connection established successfully.\n\n";
}
DatabaseManager::~DatabaseManager(){// Close the database connection
    if (db) {
        sqlite3_close(db);
    }
}

User* DatabaseManager::authenticateUser(const string& uNameOrEmail, const string& password) {// Validate user credentials
    sqlite3_stmt* stmt;
    string sql = "SELECT DISTINCT user_id, username, email, password, first_name, last_name, user_type "
                 "FROM users WHERE (username = ? OR email = ?) AND password = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr; // couldn't prepare statement
    }

    sqlite3_bind_text(stmt, 1, uNameOrEmail.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, uNameOrEmail.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

    User* loggedInUser = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int UID = sqlite3_column_int(stmt, 0);
        string uName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string em = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string pword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string fName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        string lName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        string uType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        loggedInUser = new User(UID, fName, lName, uName, pword, em, uType);
    }

    sqlite3_finalize(stmt); // <<< important! finalize before returning
    return loggedInUser;
}


//Load Users
void DatabaseManager::loadCustomerData(Customer* cust){// Load customer data from database
    vector<PaymentMethod*> payMethods = getPaymentMethods(cust->getUID());
    vector<Address*> addressBook = getAddresses(cust->getUID());
    vector<CustomerOrder*> orders = getCustomerOrderHistory(cust->getUID());

    cust->importOrderHistory(orders);
    cust->importAddressBook(addressBook);
    cust->importPaymentMethods(payMethods);
}  

void DatabaseManager::loadSellerData(Seller* sllr){// Load seller data from database
    sqlite3_stmt* stmt;
    string sql = "SELECT deposit_account_balance FROM users WHERE user_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "Database Error: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, sllr->getUID());
    
    if (sqlite3_step(stmt) == SQLITE_ROW){
        double balance = sqlite3_column_double(stmt, 0);
        sllr->setBalance(balance);
    }
    sqlite3_finalize(stmt);

    vector<SellerOrder*> orders = getSellerOrders(sllr->getUID());
    vector<Product*> products = getProductsBySeller(sllr->getUID());

    sllr->importSellerOrders(orders);
    sllr->importSellerProducts(products);

} 



void DatabaseManager::updateUser(const User& user){// Update user information
    sqlite3_stmt* stmt;
    string sql = "UPDATE users SET username = ?, email = ?, password = ?, first_name = ?, last_name = ? "
                 "WHERE user_id = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_text(stmt, 1, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user.getEmail().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.getFirstName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, user.getLastName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, user.getUID());

    if (sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "DATABASE ERROR: SQL step error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    sqlite3_finalize(stmt);
    return;
} 



void DatabaseManager::insertUser(User& newUser){// Add new user to database
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO users (username, email, password, first_name, last_name, user_type)"
                 "VALUES (?, ?, ?, ?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_text(stmt, 1, newUser.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, newUser.getEmail().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, newUser.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, newUser.getFirstName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, newUser.getLastName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, newUser.getRole().c_str(), -1, SQLITE_TRANSIENT);
    
    if (sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "DATABASE ERROR: SQL step error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    else {
        int UID = static_cast<int>(sqlite3_last_insert_rowid(db));
        newUser.setUID(UID);
    }
    sqlite3_finalize(stmt);       
    return;
} 

bool DatabaseManager::emailInUse(const string& email){ // Check if email already exists
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT 1 FROM users WHERE email = ? LIMIT 1;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    bool emailExists = false;

    if (sqlite3_step(stmt) == SQLITE_ROW){
        emailExists = true;
    }
    sqlite3_finalize(stmt);
    return emailExists;
}

bool DatabaseManager::usernameInUse(const string& uName){// Check if username already exists
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT 1 FROM users WHERE username = ? LIMIT 1;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_bind_text(stmt, 1, uName.c_str(), -1, SQLITE_TRANSIENT);
    bool uNameExists = false;

    if (sqlite3_step(stmt) == SQLITE_ROW){
        uNameExists = true;
    }
    sqlite3_finalize(stmt);
    return uNameExists;
}
 
bool DatabaseManager::resetUserPassword(const string& uName, const string& em, const string& newPW){//admin reset password
    sqlite3_stmt* stmt = nullptr;
    string sql = "UPDATE users SET password = ? WHERE username = ? AND email = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again\n\n";
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_bind_text(stmt, 1, newPW.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, uName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, em.c_str(), -1, SQLITE_TRANSIENT);

    bool pwUpdated = false;
    if (sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "DATABASE ERROR: SQL step error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    else {
        if (sqlite3_changes(db) > 0){
            pwUpdated = true;
        }
    }
    sqlite3_finalize(stmt);
    return pwUpdated;
}

//Product Management
vector<Product*> DatabaseManager::searchProducts(const string& keyword){ // Find products by search term
    vector<string> searchTerms;
    vector<Product*> matches;
    string searchTerm;
    istringstream iss(keyword);
    while (iss >> searchTerm) {
        searchTerms.push_back(searchTerm);
    }

    sqlite3_stmt* stmt = nullptr;
    string sql = buildProductSearchQuery(searchTerms);

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return matches; // return empty if prepare failed
    }

 
    int paramIndex = 1;
    for (string term : searchTerms){
        string likePattern = "%" + term + "%";
        sqlite3_bind_text(stmt, paramIndex++, likePattern.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, paramIndex++, likePattern.c_str(), -1, SQLITE_TRANSIENT);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW){
        int prodID = sqlite3_column_int(stmt, 0);
        int sllrID = sqlite3_column_int(stmt, 1);
        string prodName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string prodType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string prodDescription = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        double price = sqlite3_column_double(stmt, 5);
        int inventory = sqlite3_column_int(stmt, 6);

        Product* match = createProductFromRow(prodType, prodID, prodName, prodDescription, price, inventory, sllrID);
        matches.push_back(match);
    }

    sqlite3_finalize(stmt);
    return matches; 
}

Product* DatabaseManager::getProductById(int productId){ // Retrieve specific product
    sqlite3_stmt* stmt = nullptr;
    Product* match = nullptr;
    string sql = "SELECT * FROM products WHERE product_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return match;
    }
    sqlite3_bind_int(stmt, 1, productId);
    if (sqlite3_step(stmt) == SQLITE_ROW){
        int prodID = sqlite3_column_int(stmt, 0);
        int sllrID = sqlite3_column_int(stmt, 1);
        string prodName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string prodType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string prodDescription = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        double price = sqlite3_column_double(stmt, 5);
        int inventory = sqlite3_column_int(stmt, 6);

        match = createProductFromRow(prodType, prodID, prodName, prodDescription, price, inventory, sllrID);
    }
    sqlite3_finalize(stmt);
    return match;
}

void DatabaseManager::insertProduct(Product& newProd){// Add product to database
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO products (seller_id, name, type, description, price, inventory)"
                 "VALUES (?, ?, ?, ?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, newProd.getSellerID());
    sqlite3_bind_text(stmt, 2, newProd.getProductName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, newProd.getType().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, newProd.getProductDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, newProd.getPrice());
    sqlite3_bind_int(stmt, 6, newProd.getInventory());
    
    if (sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "DATABASE ERROR: SQL step error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    else {
        int prodID = static_cast<int>(sqlite3_last_insert_rowid(db));
        newProd.setProductID(prodID);

    }
    sqlite3_finalize(stmt);       
    return;
}

void DatabaseManager::deleteProduct(int prodID){ // Remove product from database
    sqlite3_stmt* stmt = nullptr;
    string sql = "DELETE FROM products WHERE product_id = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, prodID);
    if (sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "DATABASE ERROR: SQL step error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n"; 
    }
    sqlite3_finalize(stmt);
    return;
}

void DatabaseManager::updateProduct(Product* prod){// Update product information
    sqlite3_stmt* stmt;
    string sql = "UPDATE products SET name = ?, type = ?, description = ?, price = ?, inventory = ? "
                 "WHERE product_id = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_text(stmt, 1, prod->getProductName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, prod->getType().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, prod->getProductDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, prod->getPrice());
    sqlite3_bind_int(stmt, 5, prod->getInventory());
    sqlite3_bind_int(stmt, 6, prod->getProductID());

    if (sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "DATABASE ERROR: SQL step error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    sqlite3_finalize(stmt);
    return;
}

void DatabaseManager::updateProductInventory(int productId, int newInventory){// Update stock level
    sqlite3_stmt* stmt = nullptr;
    string sql = "UPDATE products SET inventory = ? WHERE product_id = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, newInventory);
    sqlite3_bind_int(stmt, 2, productId);
    if (sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "DATABASE ERROR: SQL Step Error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    sqlite3_finalize(stmt);
    return;
} 
    
vector<Product*> DatabaseManager::getProductsBySeller(int sellerId){ // Get seller's products
    vector<Product*> products;
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT * FROM products WHERE seller_id = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return products;
    }
    sqlite3_bind_int(stmt, 1, sellerId);
    while (sqlite3_step(stmt) == SQLITE_ROW){
        int prodID = sqlite3_column_int(stmt, 0);
        int sllrID = sqlite3_column_int(stmt, 1);
        string prodName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string prodType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string prodDescription = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        double price = sqlite3_column_double(stmt, 5);
        int inventory = sqlite3_column_int(stmt, 6);

        Product* match = createProductFromRow(prodType, prodID, prodName, prodDescription, price, inventory, sllrID);
        products.push_back(match);
    }
    sqlite3_finalize(stmt);
    return products;
}


//Order Management
void DatabaseManager::insertCustomerOrder(CustomerOrder* order){// Add new customer order to database
    sqlite3_stmt* stmt = nullptr;
    string sql = "INSERT INTO customer_orders (customer_id, order_date, shipping_address_id, payment_id, total)"
                 "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, order->getCustomerID());
    sqlite3_bind_text(stmt, 2, order->getOrderDate().dateString().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, order->getShippingAddressID());
    sqlite3_bind_int(stmt, 4, order->getPaymentID());
    sqlite3_bind_double(stmt, 5, order->getTotal());

    if (sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "DATABASE ERROR: SQL Step Error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    else{
        int orderID = static_cast<int>(sqlite3_last_insert_rowid(db));
        order->setOrderID(orderID);
    }
    sqlite3_finalize(stmt);
    return;

} 

vector<CustomerOrder*> DatabaseManager::getCustomerOrderHistory(int customerId){ // Get all orders for a specific customer
    vector<CustomerOrder*> orders;
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT * FROM customer_orders WHERE customer_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return orders;
    }
    sqlite3_bind_int(stmt, 1, customerId);
    while (sqlite3_step(stmt) == SQLITE_ROW){
        int orderID = sqlite3_column_int(stmt, 0);
        int custID = sqlite3_column_int(stmt, 1);
        vector<OrderItem*> orderItems = getOrderItems(orderID);
        string orderDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Date orderDateObj(orderDate);
        int shippingAddressID = sqlite3_column_int(stmt, 3);
        Address* shippingAddress = getAddress(shippingAddressID);
        int paymentID = sqlite3_column_int(stmt, 4);
        PaymentMethod* paymentUsed = getPaymentMethod(paymentID);
        double total = sqlite3_column_double(stmt, 5);
        CustomerOrder* order = new CustomerOrder(orderID, custID, orderItems, *paymentUsed, *shippingAddress, orderDateObj, total);
        delete shippingAddress;
        delete paymentUsed;
        orders.push_back(order);
    }
    sqlite3_finalize(stmt);
    return orders;
    
}

vector<OrderItem*> DatabaseManager::getOrderItems(const int& ordID){ // Get order items for a specific order
    vector<OrderItem*> orderItems;
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT * FROM order_items WHERE order_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return orderItems;
    }
    sqlite3_bind_int(stmt, 1, ordID);
    while (sqlite3_step(stmt) == SQLITE_ROW){
        int orderItemID = sqlite3_column_int(stmt, 0);
        int orderID = sqlite3_column_int(stmt, 1);
        int productID = sqlite3_column_int(stmt, 2);
        int sellerID = sqlite3_column_int(stmt, 3);
        int quantity = sqlite3_column_int(stmt, 4);
        double priceAtPurchase = sqlite3_column_double(stmt, 5);
        Product* product = getProductById(productID);
        if (product == nullptr) {
            cerr << "DATABASE ERROR: Product no longer available for order item ID: " << orderItemID << "\n";
        }
        OrderItem* item = new OrderItem(product, sellerID, ordID, quantity, priceAtPurchase);
        item->setOrderItemID(orderItemID);
        orderItems.push_back(item);
    }
    sqlite3_finalize(stmt);
    return orderItems;
}

CustomerOrder* DatabaseManager::retrieveCustomerOrder(int ordID){// Retrieve a specific customer order
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT * FROM customer_orders WHERE order_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return nullptr;
    }
    sqlite3_bind_int(stmt, 1, ordID);
    CustomerOrder* order = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW){
        int orderID = sqlite3_column_int(stmt, 0);
        int custID = sqlite3_column_int(stmt, 1);
        vector<OrderItem*> orderItems = getOrderItems(orderID);
        string orderDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        Date orderDateObj(orderDate);
        int shippingAddressID = sqlite3_column_int(stmt, 3);
        Address* shippingAddress = getAddress(shippingAddressID);
        int paymentID = sqlite3_column_int(stmt, 4);
        PaymentMethod* paymentUsed = getPaymentMethod(paymentID);
        double total = sqlite3_column_double(stmt, 5);
        order = new CustomerOrder(orderID, custID, orderItems, *paymentUsed, *shippingAddress, orderDateObj, total);
        delete shippingAddress;
        delete paymentUsed;
    }
    
    sqlite3_finalize(stmt);
    return order;
} 

vector<SellerOrder*> DatabaseManager::getSellerOrders(int sellerID) {// Get all orders for a specific seller
    vector<SellerOrder*> orders;
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT * FROM seller_orders WHERE seller_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return orders;
    }
    sqlite3_bind_int(stmt, 1, sellerID);

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int sellerID = sqlite3_column_int(stmt, 0);
        int customerOrderID = sqlite3_column_int(stmt, 1);
        string orderDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double total = sqlite3_column_double(stmt, 3);
        SellerOrder* order = new SellerOrder(customerOrderID, sellerID, orderDate, total);
        vector<OrderItem*> orderItems = getSellerOrderItems(customerOrderID, sellerID);
        order->setOrderItems(orderItems);
        orders.push_back(order);
    }
    sqlite3_finalize(stmt);
    return orders;
}

vector<OrderItem*> DatabaseManager::getSellerOrderItems(int customerOrderID, int sellerID) {// Get order items for a specific seller order
    vector<OrderItem*> orderItems;
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT * FROM order_items WHERE order_id = ? AND seller_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR (getSellerOrderItems): " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return orderItems;
    }
    sqlite3_bind_int(stmt, 1, customerOrderID);
    sqlite3_bind_int(stmt, 2, sellerID);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int orderItemID = sqlite3_column_int(stmt, 0);
        int productID = sqlite3_column_int(stmt, 2);
        int quantity = sqlite3_column_int(stmt, 4);
        double priceAtPurchase = sqlite3_column_double(stmt, 5);
        Product* product = getProductById(productID);

        if (product == nullptr) {
            cerr << "WARNING (getSellerOrderItems): Product ID " << productID
                 << " not found for OrderItem ID " << orderItemID
                 << ". OrderItem will be created without full product details." << endl;
        }

        OrderItem* item = new OrderItem(product, sellerID, customerOrderID, quantity, priceAtPurchase);
        item->setOrderItemID(orderItemID);
        orderItems.push_back(item);
    }
    sqlite3_finalize(stmt);
    return orderItems;
}

void DatabaseManager::insertOrderItem(OrderItem& newItem){// Add order item to database
    sqlite3_stmt* stmt = nullptr;
    string sql = "INSERT INTO order_items (order_id, product_id, seller_id, quantity, price_at_purchase)"
                 "VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, newItem.getOrderID());
    sqlite3_bind_int(stmt, 2, newItem.getProduct()->getProductID());
    sqlite3_bind_int(stmt, 3, newItem.getSellerID());
    sqlite3_bind_int(stmt, 4, newItem.getQuantity());
    sqlite3_bind_double(stmt, 5, newItem.getPriceAtPurchase());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "DATABASE ERROR: SQL Step Error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    else {
        int orderItemID = static_cast<int>(sqlite3_last_insert_rowid(db));
        newItem.setOrderItemID(orderItemID);
    }
    sqlite3_finalize(stmt);
}

void DatabaseManager::insertSellerOrder(int orderID, int sellerID, string orderDate, double total){// Add seller order to database
    sqlite3_stmt* stmt = nullptr;
    string sql = "INSERT INTO seller_orders (seller_id, order_id, order_date, total)"
                 "VALUES (?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, sellerID);
    sqlite3_bind_int(stmt, 2, orderID);
    sqlite3_bind_text(stmt, 3, orderDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, total);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "DATABASE ERROR: SQL Step Error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    sqlite3_finalize(stmt);
    return;
} 
    
void DatabaseManager::updateSellerBalance(int sellerID, int earnings) {
    sqlite3_stmt* stmt = nullptr;
    string sql = "UPDATE users SET deposit_account_balance = IFNULL(deposit_account_balance, 0) + ? WHERE user_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_double(stmt, 1, earnings);
    sqlite3_bind_int(stmt, 2, sellerID);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "DATABASE ERROR: SQL Step Error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    sqlite3_finalize(stmt);
}



void DatabaseManager::insertPaymentMethod(PaymentMethod& pMethod){ // Add payment method to database
    sqlite3_stmt* stmt = nullptr;
    string sql = "INSERT INTO payment_methods (user_id, card_number, card_type, expiry_date, billing_address_id, cardholder_name, default_payment_method)"
                 "VALUES (?, ?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, pMethod.getUserID());
    sqlite3_bind_int64(stmt, 2, pMethod.getCardNumber());
    sqlite3_bind_text(stmt, 3, pMethod.getCardType().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, pMethod.getExpiration().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, pMethod.getBillingAddressID());
    sqlite3_bind_text(stmt, 6, pMethod.getCardholderName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 7, pMethod.isDefault());
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "DATABASE ERROR: SQL Step Error: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
    }
    else {
        int paymentID = static_cast<int>(sqlite3_last_insert_rowid(db));
        pMethod.setPaymentID(paymentID);
    }
    sqlite3_finalize(stmt);
    return;

}

PaymentMethod* DatabaseManager::getPaymentMethod(int paymentID){ // Retrieve specific payment method
    sqlite3_stmt* stmt = nullptr;
    PaymentMethod* paymentMethod = nullptr;
    string sql = "SELECT * FROM payment_methods WHERE payment_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR: " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return paymentMethod;
    }
    sqlite3_bind_int(stmt, 1, paymentID);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int retrievedPaymentID = sqlite3_column_int(stmt, 0);
        int userID = sqlite3_column_int(stmt, 1);
        long cardNumber = sqlite3_column_int64(stmt, 2);
        string cardType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string expiryDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        int billingAddressID = sqlite3_column_int(stmt, 5);
        string cardholderName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        bool isDefault = sqlite3_column_int(stmt, 7) != 0;

        Address* pBillingAddress = getAddress(billingAddressID);
        if (!pBillingAddress) {
             cerr << "WARNING: Billing Address ID " << billingAddressID << " not found for Payment Method ID " << retrievedPaymentID << ". Cannot create PaymentMethod object." << endl;
             sqlite3_finalize(stmt);
             return nullptr;
        }


        paymentMethod = new PaymentMethod(userID, retrievedPaymentID, billingAddressID, cardType, cardNumber, cardholderName, expiryDate, isDefault, *pBillingAddress);
        delete pBillingAddress; // Clean up the Address object
    }
    sqlite3_finalize(stmt);
    return paymentMethod;
}

vector<PaymentMethod*> DatabaseManager::getPaymentMethods(int userID){// Get all payment methods for a user
    vector<PaymentMethod*> paymentMethods;
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT * FROM payment_methods WHERE user_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR (getPaymentMethods): " << sqlite3_errmsg(db) << "\nPlease try again.\n\n";
        if (stmt) sqlite3_finalize(stmt);
        return paymentMethods;
    }
    sqlite3_bind_int(stmt, 1, userID);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int paymentID = sqlite3_column_int(stmt, 0);
        //User ID is already bound
        long cardNumber = sqlite3_column_int64(stmt, 2);
        string cardType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string expiryDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        int billingAddressID = sqlite3_column_int(stmt, 5);
        string cardholderName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        bool isDefault = sqlite3_column_int(stmt, 7) != 0;

        Address* pBillingAddress = getAddress(billingAddressID);

        PaymentMethod* paymentMethod = new PaymentMethod(userID, paymentID, billingAddressID, cardType, cardNumber, cardholderName, expiryDate, isDefault, *pBillingAddress);
        paymentMethods.push_back(paymentMethod);
        delete pBillingAddress; // Clean up the Address object
    }

    sqlite3_finalize(stmt);
    return paymentMethods;
} 

void DatabaseManager::deletePaymentMethod(int paymentID){// Remove payment method from database
    sqlite3_stmt* stmt = nullptr;
    string sql = "DELETE FROM payment_methods WHERE payment_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR (deletePaymentMethod): " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, paymentID);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "DATABASE ERROR (deletePaymentMethod): SQL Step Error: " << sqlite3_errmsg(db) << "\n";
    }


    sqlite3_finalize(stmt);
}


void DatabaseManager::updatePaymentMethod(PaymentMethod& pMethod){ // Update payment method in database
    sqlite3_stmt* stmt = nullptr;
    string sql = "UPDATE payment_methods SET user_id = ?, card_number = ?, card_type = ?, expiry_date = ?, "
                 "billing_address_id = ?, cardholder_name = ?, default_payment_method = ? "
                 "WHERE payment_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR (updatePaymentMethod): " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }


    sqlite3_bind_int(stmt, 1, pMethod.getUserID());
    sqlite3_bind_int64(stmt, 2, pMethod.getCardNumber());
    sqlite3_bind_text(stmt, 3, pMethod.getCardType().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, pMethod.getExpiration().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, pMethod.getBillingAddressID());
    sqlite3_bind_text(stmt, 6, pMethod.getCardholderName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 7, pMethod.isDefault());
    sqlite3_bind_int(stmt, 8, pMethod.getPaymentID());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "DATABASE ERROR (updatePaymentMethod): SQL Step Error: " << sqlite3_errmsg(db) << "\n";
    }


    sqlite3_finalize(stmt);
}

vector<Address*> DatabaseManager::getAddresses(int userID){ // Get all addresses for a user
    vector<Address*> addresses;
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT * FROM addresses WHERE user_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR (getAddresses): " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return addresses;
    }
    sqlite3_bind_int(stmt, 1, userID);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int addressID = sqlite3_column_int(stmt, 0);
        // userID is column 1, already known
        string street = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const unsigned char* unit = sqlite3_column_text(stmt, 3);
        string unitStr = unit ? reinterpret_cast<const char*>(unit) : "";

        string city = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        string state = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        int zipCode = sqlite3_column_int(stmt, 6);

        Address* address = new Address(userID, addressID, street, unitStr, city, state, zipCode);
        addresses.push_back(address);
    }

    sqlite3_finalize(stmt);
    return addresses;
}

void DatabaseManager::deleteAddress(int addressID){ // Remove address from database
    sqlite3_stmt* stmt = nullptr;
    string sql = "DELETE FROM addresses WHERE address_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR (deleteAddress): " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }
    sqlite3_bind_int(stmt, 1, addressID);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "DATABASE ERROR (deleteAddress): SQL Step Error: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(stmt);
}


void DatabaseManager::insertAddress(Address& newAddress){ // Add address to database
    sqlite3_stmt* stmt = nullptr;
    string sql = "INSERT INTO addresses (user_id, street, unit, city, state, zip_code)"
                 "VALUES (?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR (insertAddress): " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return;
    }

    // Bind values from the Address object
    sqlite3_bind_int(stmt, 1, newAddress.getUserID());
    sqlite3_bind_text(stmt, 2, newAddress.getStreetName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, newAddress.getUnit().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, newAddress.getCityName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, newAddress.getStateName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, newAddress.getZipCode());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "DATABASE ERROR (insertAddress): SQL Step Error: " << sqlite3_errmsg(db) << "\n";
    }
    else {
        // Set the address ID on the object passed by reference
        int addressID = static_cast<int>(sqlite3_last_insert_rowid(db));
        newAddress.setAddressID(addressID);
    }

    sqlite3_finalize(stmt);
}

Address* DatabaseManager::getAddress(int addressID){ // Retrieve specific address
    sqlite3_stmt* stmt = nullptr;
    Address* address = nullptr;
    string sql = "SELECT * FROM addresses WHERE address_id = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DATABASE ERROR (getAddress): " << sqlite3_errmsg(db) << "\n";
        if (stmt) sqlite3_finalize(stmt);
        return nullptr; // Return nullptr on error
    }
    sqlite3_bind_int(stmt, 1, addressID);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // addressID is column 0, already known
        int userID = sqlite3_column_int(stmt, 1);
        string street = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const unsigned char* unit = sqlite3_column_text(stmt, 3);
        string unitStr = unit ? reinterpret_cast<const char*>(unit) : ""; // Handle NULL unit
        string city = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        string state = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        int zipCode = sqlite3_column_int(stmt, 6);

        address = new Address(userID, addressID, street, unitStr, city, state, zipCode);
    }

    sqlite3_finalize(stmt);
    return address;
}

string DatabaseManager::buildProductSearchQuery(const vector<string>& searchTerms){
    ostringstream oss;
    oss << "SELECT * FROM products WHERE ";

    for (int i = 0; i < searchTerms.size(); ++i){
        oss << "(name LIKE ? OR description LIKE ?)";
        if (i != searchTerms.size() - 1){
            oss << " AND ";
        }
    }

    oss << ";";
    return oss.str();
}

Product* DatabaseManager::createProductFromRow(
    const string& type,
    int id,
    const string& name,
    const string& description,
    double price,
    int inventory,
    int sellerId){

        return new Product(sellerId, id, name, description, type, price, inventory);
    }