#include "login.h"
#include <iostream>
#include <limits>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cctype>

using namespace std;

// Helper: check if password follows standard rules
static bool isValidPassword(const string& pwd) {
    if (pwd.size() < 8) return false;

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (unsigned char ch : pwd) {
        if (isupper(ch)) hasUpper = true;
        else if (islower(ch)) hasLower = true;
        else if (isdigit(ch)) hasDigit = true;
        else hasSpecial = true; // anything not letter/digit
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

// -----------------------------------------------------------------
// Low-level register for NEW USER (non-interactive)
// -----------------------------------------------------------------
bool registerNewLogin(sql::Connection* con, const string& email, const string& pass) {
    if (!con) {
        cerr << "No DB connection in registerNewLogin.\n";
        return false;
    }

    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement("INSERT INTO `LOGIN` (Email, Password) VALUES (?, ?)");

        pstmt->setString(1, email);
        pstmt->setString(2, pass);
        pstmt->executeUpdate();
        delete pstmt;

        return true;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in registerNewLogin: " << e.what() << endl;
        return false;
    }
}

// -----------------------------------------------------------------
// Interactive registration (not used in your flow now)
// -----------------------------------------------------------------
bool registerUser(sql::Connection* con) {
    if (!con) {
        cout << "❌ No active DB connection.\n";
        return false;
    }

    string email, pass;

    cout << "\n=== REGISTER NEW USER ===\n";
    cout << "Enter Email: ";
    cin >> email;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (true) {
        cout << "Create Password: ";
        getline(cin, pass);

        if (isValidPassword(pass)) break;

        cout << "\n❌ Password does not meet rules.\n";
        cout << "   Must contain uppercase, lowercase, digit, special, 8+ chars.\n\n";
    }

    if (!registerNewLogin(con, email, pass)) {
        cout << "❌ Registration failed.\n";
        return false;
    }

    cout << "✔ Registration successful.\n";
    return true;
}

// -----------------------------------------------------------------
// Authenticate an email+password pair from LOGIN table
// -----------------------------------------------------------------
bool authenticateLogin(sql::Connection* con, const string& email, const string& pass) {
    if (!con) return false;

    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement("SELECT Password FROM `LOGIN` WHERE Email = ?");
        pstmt->setString(1, email);

        sql::ResultSet* res = pstmt->executeQuery();

        bool ok = false;

        if (res->next()) {
            string stored = res->getString("Password");
            if (stored == pass) ok = true;  // plain-text compare
        }

        delete res;
        delete pstmt;

        return ok;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in authenticateLogin: " << e.what() << endl;
        return false;
    }
}

// -----------------------------------------------------------------
// Interactive LOGIN window for existing users
// -----------------------------------------------------------------
bool loginWindow(sql::Connection* con, std::string& loggedInEmail) {
    if (!con) {
        cout << "❌ No active DB connection.\n";
        return false;
    }

    string email;
    string password;

    cout << "=====================================\n";
    cout << "            LOGIN WINDOW             \n";
    cout << "=====================================\n\n";

    cout << "Enter Email: ";
    getline(cin, email);

    if (email.empty()) getline(cin, email);  // fix leftover newline issue

    cout << "Enter Password: ";
    getline(cin, password);

    if (!authenticateLogin(con, email, password)) {
        cout << "\n❌ Incorrect email or password.\n";
        return false;
    }

    loggedInEmail = email;
    return true;
}
