#include "login.h"
#include <iostream>
#include <limits>
#include <cppconn/prepared_statement.h>
#include <cctype>
using namespace std;


// Helper: check if password follows standard rules
static bool isValidPassword(const string& pwd) {
    if (pwd.size() < 8) return false;

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (char ch : pwd) {
        if (isupper(static_cast<unsigned char>(ch))) hasUpper = true;
        else if (islower(static_cast<unsigned char>(ch))) hasLower = true;
        else if (isdigit(static_cast<unsigned char>(ch))) hasDigit = true;
        else hasSpecial = true; // anything not letter/digit
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

bool loginWindow(sql::Connection* con, std::string& loggedInEmail) {
    if (!con) {
        cout << "❌ No active DB connection.\n";
        return false;
    }

    string email, password;

    cout << "=====================================\n";
    cout << "            LOGIN WINDOW             \n";
    cout << "=====================================\n\n";

    cout << "Enter Email: ";
    getline(cin, email);

    // Ask for password until it is valid
    while (true) {
        cout << "Enter Password: ";
        getline(cin, password);

        if (isValidPassword(password)) {
            break;
        }

        cout << "\n❌ Password does not meet rules.\n";
        cout << "   It must be at least 8 characters,\n";
        cout << "   contain uppercase, lowercase, digit, and special character.\n\n";
    }

    try {
        sql::PreparedStatement* pstmt = nullptr;

        pstmt = con->prepareStatement(
            "INSERT INTO LOGIN (Email, Password) VALUES (?, ?)"
        );
        pstmt->setString(1, email);
        pstmt->setString(2, password);

        int rows = pstmt->executeUpdate();
        delete pstmt;

        if (rows <= 0) {
            cout << "❌ Failed to save login details.\n";
            return false;
        }

        cout << "\n✅ Login details saved successfully.\n";

        // Give email back to caller for Customer details later
        loggedInEmail = email;
        return true;
    }
    catch (sql::SQLException &e) {
        cout << "SQL Error while saving login: " << e.what() << endl;
        return false;
    }
}
