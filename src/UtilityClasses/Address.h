#ifndef ADDRESS_H
#define ADDRESS_H
#include <string>
#include <iostream>



using namespace std;

class Address {
    friend ostream& operator<<(ostream& os, Address& adrs);
    friend istream& operator>>(istream& is, Address& adrs);

    private:
        int userID;
        int addressID;
        string street;
        string unit;
        string city;
        string state;
        size_t zipCode;
    public:
        Address(int UID);
        Address(int UID, const string& street, const string& un, const string& cty, const string& state, const size_t& zip);
        Address(int UID, int addressID, const string& street, const string& un, const string& cty, const string& state, const size_t& zip);
    //Setters
        void setAddressID(int AID);
        void setStreetName(const string& str);
        void setUnit(const string& u);
        void setCityName(const string& cty);
        void setStateName(const string& st);
        void setZipCode(const size_t& zip);
    //Getters
        int getUserID(); 
        string getStreetName();
        string getUnit();
        string getCityName();
        string getStateName();
        size_t getZipCode();
        int getAddressID();

    //Utility
        static string formatString(const string& str); // must be static so that friend functions can access
        bool operator==(const Address& other) const;

    


};
    






#endif
