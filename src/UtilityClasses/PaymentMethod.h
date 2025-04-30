#ifndef PAYMENT_METHOD_H
#define PAYMENT_METHOD_H
#include "Address.h"
#include "Date.h"
#include <string>

using namespace std;



class PaymentMethod {
    friend ostream& operator<<(ostream& os, PaymentMethod& pm);
    friend istream& operator>>(istream& is, PaymentMethod& pm);

    private:
        int userID;
        int paymentID;
        int billAddressID;
        string cardType;
        long cardNumber;
        string cardholderName;
        Date expiration;
        bool defaultPayment;

        
        Address billingAddress;

    public:
        //Takes mm and dd and calls set billing
        PaymentMethod(int UID);
        PaymentMethod(int UID, int pmntID, int blAdrsID, string type, long cdNum, string name, string date, bool dfltPmt, Address& bAdrs);//for constructing from database
        
        int getUserID() const;
        string getCardType() const;
        int getPaymentID() const;
        long getCardNumber() const;
        int getMonthEnd(int mm, int yy) const;
        string getCardholderName() const;
        string getExpiration();
        Address& getBillingAddress();
        int getBillingAddressID() const;

        void setExpiration(int mm, int yy);
        void setCardType(string type);
        void setCardNumber(long cardNum);
        void setPaymentID(int PID);
        void setDefaultPayment(bool);
        void setCardholderName(const string& name);
        void setBillingAddress(const Address& bllAddress);
        void setBillingAddressID(const int billAddressID);

        bool isDefault() const;
        bool isExpired() const;

        static long validateCardNum(string strCrdNum, string crdTp);

        bool operator==(PaymentMethod& other);

        

};


#endif
