#ifndef DBFUNCTIONS_H
#define DBFUNCTIONS_H

#include <iostream>
#include <string>
#include <vector>

// MySQL Connector/C++ headers
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "Customer.h"


using namespace std;

// -------------------------
// DB Connection
// -------------------------
sql::Connection* createConnection();

// -------------------------
// Display functions (read-only)
// -------------------------
void displayAllProducts(sql::Connection *con);
void displayByCategory(sql::Connection* con, string cat);
void displayBySubcategory(sql::Connection* con, string cat, string subcat);

// -------------------------
// Dynamic loaders (vector-based)
// -------------------------
vector<string> loadCategories(sql::Connection* con);
vector<string> loadSubcategories(sql::Connection* con, const string &category);

//-----------------
// Customer details 
//-----------------
void inputCustomerDetails(sql::Connection* con, const std::string& emailFromLogin);
void insertCustomerDetails(sql::Connection* con, const Customer& c);

void displayProductDetailsById(int productId);
void displaySupplierForProduct(int productId);

int getLastInsertedCustomerID(sql::Connection* con);

int getLastInsertedCustomerID(sql::Connection* con);
int getCustomerIdByEmail(sql::Connection* con, const std::string& email);
int getCustomerIdByEmail(sql::Connection* con, const std::string& email);

#endif // DBFUNCTIONS_H
