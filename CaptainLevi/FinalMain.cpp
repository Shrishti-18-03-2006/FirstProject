#include "Customer.h"
#include "login.h"
#include "DBFunctions.h"
#include "Functions.h"
#include "CartFunctions.h"

#include <iostream>
#include <string>
#include <limits>

using namespace std;
int main() {
    sql::Connection* con = nullptr;
    try {
        con = createConnection();
    } catch (sql::SQLException &e) {
        cout << "Failed to connect to DB: " << e.what() << endl;
        return 1;
    }

    int userType;
    string email, password;
    int customerId = -1;

    clearScreen();
    cout << "==============================\n";
    cout << "     WELCOME TO PROJECT1\n";
    cout << "==============================\n\n";
    cout << "1) New User\n";
    cout << "2) Existing User\n";
    cout << "0) Exit\n\n";

    userType = getIntInput("Enter your choice: ");

    if (userType == 0) {
        cout << "Goodbye!\n";
        return 0;
    }

    // =================================================
    // NEW USER FLOW
    // =================================================
    if (userType == 1) {

        cout << "\n=== CREATE NEW LOGIN ===\n";
        cout << "Enter Email: ";
        cin >> email;
        cout << "Create Password: ";
        cin >> password;

        // Create login record for new user
        if (!registerNewLogin(con, email, password)) {
            cout << "❌ Failed to create login.\n";
            return 1;
        }

        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        clearScreen();

        // Enter customer details
        inputCustomerDetails(con, email);

        // Fetch the newly inserted customer ID
        customerId = getLastInsertedCustomerID(con);
        if (customerId == -1) {
            cout << "❌ Could not get Customer ID.\n";
            return 1;
        }

        cout << "✔ Profile created! Customer ID = " << customerId << "\n";
        pressEnterToContinue();
    }

    // =================================================
    // EXISTING USER FLOW
    // =================================================
    else if (userType == 2) {

        cout << "\n=== LOGIN ===\n";
        if (!loginWindow(con, email)) {
            cout << "❌ Login failed.\n";
            return 1;
        }

        // Fetch existing customer ID
        customerId = getCustomerIdByEmail(con, email);
        if (customerId == -1) {
            cout << "❌ No customer details found.\n";
            cout << "Please create a new account.\n";
            return 1;
        }

        cout << "✔ Login successful! Customer ID = " << customerId << "\n";
        pressEnterToContinue();
    }

    // =================================================
    // INTO THE MAIN APPLICATION (PRODUCTS / CART)
    // =================================================
    runCategoryMenu(con, customerId);

    // Close DB
    if (con) {
        con->close();
        delete con;
    }

    return 0;
}
