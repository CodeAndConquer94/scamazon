#include "PaymentMethod.h"
#include "Date.h"
#include "Address.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

PaymentMethod::PaymentMethod(int UID): // Constructor for new payment method
    userID(UID), paymentID(0), billAddressID(0), cardType(""), cardNumber(0),
    cardholderName(""), expiration(Date()), defaultPayment(false), billingAddress(UID) {}

PaymentMethod::PaymentMethod(int UID, int pmntID, int blAdrsID, string type,
    long cdNum, string name, string date, bool dfltPmt, Address& bAdrs)
            : userID(UID), paymentID(pmntID), billAddressID(blAdrsID), cardType(type),
            cardNumber(cdNum), cardholderName(name), expiration(Date(date)),
            defaultPayment(dfltPmt), billingAddress(bAdrs){}

string PaymentMethod::getCardType() const { return cardType; }
long PaymentMethod::getCardNumber() const { return cardNumber; }

int PaymentMethod::getMonthEnd(int month, int year) const {
    switch (month) {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            // Leap year logic: divisible by 4, but not by 100 unless also divisible by 400
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                return 29;
            else
                return 28;
        default:
            return -1; // Invalid month
    }
}

int PaymentMethod::getUserID() const { return userID; }
string PaymentMethod::getCardholderName() const { return cardholderName; }
string PaymentMethod::getExpiration() { return expiration.dateString(); }
Address& PaymentMethod::getBillingAddress() { return billingAddress; }
int PaymentMethod::getBillingAddressID() const { return billAddressID; }
int PaymentMethod::getPaymentID() const { return paymentID; }

void PaymentMethod::setExpiration(int mm, int yy) {
    int dd = getMonthEnd(mm, yy);
    expiration = Date(mm, dd, yy);
}

void PaymentMethod::setPaymentID(int PID) {
    paymentID = PID;
}

void PaymentMethod::setDefaultPayment(bool val) {
    defaultPayment = val;
}
void PaymentMethod::setCardholderName(const string& name){
    cardholderName = name;
}
void PaymentMethod::setBillingAddress(const Address& bAdrs) {
    billingAddress = bAdrs;
}
void PaymentMethod::setBillingAddressID(const int bAdrsID){
    billAddressID = bAdrsID;
}
void PaymentMethod::setCardType(string type) {
    cardType = type;
}
void PaymentMethod::setCardNumber(long cardNum) {
    cardNumber = cardNum;
}

bool PaymentMethod::isExpired() const {
    Date today;
    return today > expiration;
}

bool PaymentMethod::isDefault() const { return defaultPayment; }

istream& operator>>(istream& is, PaymentMethod& pm) {
    string cardType, crdHldrName, strCardNum;
    int mm, yy;
    bool dfltPmt;
    long cardNum;

    while (true) { //Get card type
        int cdTp;
        cout << "Enter Card Type.\n1. Visa\n2. Mastercard\n3. Discover\n4. American Express.\n\n";
        if (!(is >> cdTp)) {
            cout << "Unexpected Error: Please Try again.\n\n";
            is.clear();
            is.ignore(1000,'\n');
            continue;
        }
        switch (cdTp){
            case 1: cardType = "Visa"; break;
            case 2: cardType = "Mastercard"; break;
            case 3: cardType = "Discover"; break;
            case 4: cardType = "American Express"; break;
            default: cout << "Invalid input: Please enter a number between 1-4.\n\n"; continue;
        }
        break;
    }
    is.ignore(1000, '\n');
    while (true) {//get card number
        cout << "Enter Card Number: ";
        getline(is, strCardNum);
        try{
            cardNum = PaymentMethod::validateCardNum(strCardNum, cardType);
        }
        catch (const char* m) {
            cout << "ERROR: " << m << endl;
            cout << "Please try again.\n\n";
            continue;
        }
        break;    
    }

    while (true) { //get cardholder name
        cout << "Enter Cardholder Name: ";
        if (!getline(is, crdHldrName)){
            cout << "Unexpected Error: please try again.\n\n";
            is.clear();
            is.ignore(1000, '\n');
            continue;
        }
        if (crdHldrName.empty()) {
            cout << "Error: Name cannot be empty. Please try again\n\n";
            continue;
        }
        else break;

    }

    
    while (true) {// get expiration
        cout << "Entering Expiration date.\n\nEnter Month\n2 Digit Month: ";
        if (!(is >> mm)){
            cout << "Unexpected Error: Please try again.\n\n";
            is.clear();
            is.ignore(1000, '\n');
            continue;
        }
        is.ignore(1000, '\n');
        if (mm > 12 || mm < 1) {
            cout << "Error month must be between 1 and 12. Please try again.\n\n";
            continue;
        }
        
        cout << "Enter 4 digit year: ";
        if (!(is >> yy)){
            cout << "Unexpected Error: Please try again.\n\n";
            is.clear();
            is.ignore(1000, '\n');
            continue;
        }
        is.ignore(1000, '\n');
        break;
        
    }
    while (true) {
        int selection;
        cout << "Set as default?\n1. Yes\n2. No\n\nEnter Choice: ";
        if (!(is >> selection)){
            cout << "Unexpected Error: Please try again.\n\n";
            is.clear();
            is.ignore(1000, '\n');
            continue;
        }
        else if (selection == 1) { dfltPmt = true; break; }
        else if (selection == 2) { dfltPmt = false; break; }
        else {
            cout << "Invalid input. Please try again and enter either a 1 or a 2.\n\n";
            is.ignore(1000,'\n');
            continue;
        }
    }
    pm.setCardType(cardType);
    pm.setCardNumber(cardNum);
    pm.setExpiration(mm, yy);
    pm.setDefaultPayment(dfltPmt);
    pm.setCardholderName(crdHldrName);

    return is;
}

ostream& operator<<(ostream& os, PaymentMethod& pm) {
    os << left << setw(20) << "CARD TYPE"         << ":" << setw(30) << right << pm.cardType << "\n"
       << left << setw(20) << "CARD NUMBER"       << ":" << setw(30) << right << pm.cardNumber << "\n"
       << left << setw(20) << "EXPIRATION"        << ":" << setw(30) << right << pm.expiration.dateString() << "\n"
       << left << setw(20) << "CARDHOLDER NAME"   << ":" << setw(30) << right << pm.cardholderName << "\n";
    return os;
}

bool PaymentMethod::operator==(PaymentMethod& other) {
    return cardNumber == other.cardNumber &&
           cardType == other.cardType &&
           cardholderName == other.cardholderName &&
           expiration.dateString() == other.expiration.dateString();
}

long PaymentMethod::validateCardNum(string strCrdNum, string crdTp) {
    // Remove spaces and dashes from the card number
    strCrdNum.erase(remove_if(strCrdNum.begin(), strCrdNum.end(),
    [](char c) { return c == ' ' || c == '-'; }), strCrdNum.end());

    // Check if the card number is numeric
    if (!all_of(strCrdNum.begin(), strCrdNum.end(), ::isdigit)) {
        throw "Card Number must be numeric.";
    }

    // Convert string to integer
    long cardNum = stol(strCrdNum);

    if (crdTp == "Visa" && (strCrdNum.length() != 16)) {
        throw "Visa card number must be 16 digits";
    }
    else if (crdTp == "Mastercard" && (strCrdNum.length() != 16)) {
        throw "Mastercard number must be 16 digits";
    }
    else if (crdTp == "Discover" && (strCrdNum.length() != 16 || strCrdNum.substr(0, 4) != "6011")) {
        throw "Discover card number must be 16 digits";
    }
    else if (crdTp == "American Express" && (strCrdNum.length() != 15)) {
        throw "American Express card number must be 15 digits";
    }

    return cardNum;
}