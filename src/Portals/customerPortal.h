#ifndef CUSTOMER_PORTAL_H
#define CUSTOMER_PORTAL_H
#include "../userClasses/Customer.h"
#include "../Database/DatabaseManager.h"

void launchCustomerPortal(Customer& customer, DatabaseManager& db);
void displayLogoutScreen();
void displayWelcomeScreen();

#endif