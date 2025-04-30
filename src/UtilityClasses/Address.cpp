#include "Address.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

Address::Address(int UID) :
    userID(UID), street{""}, unit{""}, city{""}, state{""}, zipCode{00000}{}

Address::Address(int UID, const string& str, const string& un, const string& cty, const string& st, const size_t& zip) {
    userID = UID;
    setStreetName(str);
    setUnit(un);
    setCityName(cty);
    setStateName(st);
    setZipCode(zip);
    addressID = 0; // Default value for new address
    
}
Address::Address(int UID, int addrsID, const string& street, const string& un, const string& cty, const string& state, const size_t& zip) {
    userID = UID;
    addressID = addrsID;
    setStreetName(street);
    setUnit(un);
    setCityName(cty);
    setStateName(state);
    setZipCode(zip);
}


//Setters
void Address::setStreetName(const string& str){
    street = formatString(str);
}
void Address::setUnit(const string& u){
    unit = formatString(u);
}
void Address::setCityName(const string& cty){
    city = formatString(cty);
}
void Address::setStateName(const string& st){
    state = formatString(st);
}
void Address::setZipCode(const size_t& zip){
    zipCode = zip;
}
void Address::setAddressID(int AID) {
    addressID = AID;
}


//Getters
int Address::getUserID() { return userID; }
string Address::getStreetName() {return street;}
string Address::getUnit() { return unit; }
string Address::getCityName() {return city;}
string Address::getStateName() {return state;}
size_t Address::getZipCode() {return zipCode;}
int Address::getAddressID() { return addressID; }


//operators
ostream& operator<<(ostream& os, Address& adrs) {
    os << left << setw(14) << "STREET" << ":" << setw(20) << right << adrs.street << "\n"
    << left << setw(14) << "UNIT" << ":" << setw(20) << right << adrs.unit << "\n"
    << left << setw(14) << "CITY" << ":" << setw(20) << right << adrs.city << "\n"
    << left << setw(14) << "STATE" << ":" << setw(20) << right << adrs.state << "\n"
    << left << setw(14) << "ZIP CODE" << ":" << setw(20) << right << adrs.zipCode << endl;
    return os;
}

istream& operator>>(istream& is, Address& adrs) {
    string str, un, cty, st;
    size_t zip;

    // Street Name
    while (true) {
        cout << "Enter Street Name and Number: ";
        if (!getline(is, str)) {
            cout << "Unexpected Error: Please try again.\n\n";
            is.clear();
            is.ignore(1000, '\n');
            continue;
        }
        if (str.empty()) {
            cout << "Error: Street cannot be empty. Please try again.\n\n";
            continue;
        }
        adrs.street = Address::formatString(str);
        break;
    }

    // Unit Number (optional)
    cout << "Enter Unit Number (Leave Blank for N/A): ";
    getline(is, un);
    adrs.unit = Address::formatString(un);

    // City
    while (true) {
        cout << "Enter City: ";
        if (!getline(is, cty)) {
            cout << "Unexpected Error: Please try again.\n\n";
            is.clear();
            is.ignore(1000, '\n');
            continue;
        }
        if (cty.empty()) {
            cout << "Error: City cannot be empty. Please try again.\n\n";
            continue;
        }
        adrs.city = Address::formatString(cty);
        break;
    }

    // State
    while (true) {
        cout << "Enter State: ";
        if (!getline(is, st)) {
            cout << "Unexpected Error: Please try again.\n\n";
            is.clear();
            is.ignore(1000, '\n');
            continue;
        }
        if (st.empty()) {
            cout << "Error: State cannot be empty. Please try again.\n\n";
            continue;
        }
        adrs.state = Address::formatString(st);
        break;
    }

    // Zip Code
    while (true) {
        cout << "Enter Zip Code: ";
        if (!(is >> zip)) {
            cout << "Unexpected Error: Please enter numbers only.\n\n";
            is.clear();
            is.ignore(1000, '\n');
            continue;
        }
        is.ignore(1000, '\n');
        adrs.zipCode = zip;
        break;
    }

    return is;
}


string Address::formatString(const string& str){
    string formatted = str;
    transform(formatted.begin(), formatted.end(), formatted.begin(), ::toupper);
    return formatted;
}

bool Address::operator==(const Address& other) const {
    return street == other.street && city == other.city &&
            state == other.state && zipCode == other.zipCode;
}