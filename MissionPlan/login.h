#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <cppconn/connection.h>

// Interactive login window: prompts user for email+password and authenticates.
// On success it sets `email` to the logged-in email and returns true.
bool loginWindow(sql::Connection* con, std::string &email);

// Interactive registration helper: prompts user for email+password and inserts a new login.
// Returns true on success.
bool registerUser(sql::Connection* con);

// Low-level insert: create a login record for given email+password (non-interactive).
bool registerNewLogin(sql::Connection* con, const std::string& email, const std::string& pass);

// Authenticate a given email+password against the Login table (returns true if match).
bool authenticateLogin(sql::Connection* con, const std::string& email, const std::string& pass);

#endif // LOGIN_H
