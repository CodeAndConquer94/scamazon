#ifndef CUSTOMER_ORDER_H
#define CUSTOMER_ORDER_H
#include "../../Products/Product.h"
#include "../PaymentMethod.h"
#include "OrderItem.h"
#include "../Date.h"
#include "Order.h"
#include <vector>

using namespace std;

class CustomerOrder : public Order {
    private:
        PaymentMethod paymentUsed;
        Address shippingAddress;
        int customerId;
        int shippingAddressId;
        int paymentMethodId;
    
    public:
        CustomerOrder(const int custID, const PaymentMethod& payment, const int payID, Address& address, const int adrsID, double total);
        CustomerOrder(int orderID, int custID, const vector<OrderItem*>& ordItems, const PaymentMethod& payment, Address& address, const Date& date, double total);
        void setOrderID(int OID);
        void setOrderItems(const vector<OrderItem*>& ordItems);
        int getOrderID() const;
        int getCustomerID() const;
        int getShippingAddressID() const;
        int getPaymentID() const;
        void displayOrderSummary() override;

    };

#endif