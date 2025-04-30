#ifndef MAIN_H
#define MAIN_H
#include "Database/DatabaseManager.h"
#include "userClasses/User.h"
#include "userClasses/Seller.h"
#include "userClasses/Admin.h"
#include "userClasses/Customer.h"
#include "Portals/customerPortal.h"
#include "Portals/sellerPortal.h"
#include "Portals/adminPortal.h"

void displayWelcome();
void mainMenu(DatabaseManager& dbm);
void login(DatabaseManager& dbm);
void registerUser(DatabaseManager& dbm);


#endif