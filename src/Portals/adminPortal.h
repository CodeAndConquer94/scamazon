#ifndef ADMINPORTAL_H
#define ADMINPORTAL_H

#include "../userClasses/Admin.h"
#include "../Database/DatabaseManager.h"

// Function prototypes for admin portal
void launchAdminPortal(Admin& admin, DatabaseManager& db);
void displayAdminWelcomeScreen();
void displayAdminLogoutScreen();

#endif // ADMINPORTAL_H
