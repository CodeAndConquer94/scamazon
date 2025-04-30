#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>
#include <vector>

using namespace std;



class Product {
    friend istream& operator>>(istream& is, Product& prdct);

    protected:
        int sellerID;
        int productId;
        string name;
        string description;
        string type;
        double price;
        int inventoryCount;

    
    public:
        Product(int sellerID);
        Product(int SID, int prodID, string nm,string dscrptn, string tp, double price, int invtry);
        ~Product() = default;

        string getType();
        double getPrice() const;
        int getInventory() const;
        int getProductID() const;
        string getProductName() const;
        int getSellerID() const;
        string getProductDescription();

        void displayProductHeader();
        void displayProduct();
        void updateStock(int qty);

        void setProductName();
        void setProductID(int prodID);
        void setProductDescription();
        void setProductPrice();

        string formatDescription(string& str);

        //Utility
        
        static Product* displayProductList(vector<Product*>& prdcts);
        static void deleteProductVector(vector<Product*>& prdcts);
        static bool isDigits(const string& s);

        bool operator==(Product& other);
    };


#endif