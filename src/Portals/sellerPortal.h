#ifndef SELLERPORTAL_H
#define SELLERPORTAL_H

#include "../userClasses/Seller.h"
#include "../Database/DatabaseManager.h"

void launchSellerPortal(Seller& seller, DatabaseManager& db);
void displaySellerWelcomeScreen();
void displaySellerLogoutScreen();

#endif
