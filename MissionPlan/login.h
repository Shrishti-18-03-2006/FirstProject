#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <cppconn/connection.h>

// Returns true if login saved successfully.
// Also gives back the email used for login in loggedInEmail.
bool loginWindow(sql::Connection* con, std::string& loggedInEmail);

#endif
