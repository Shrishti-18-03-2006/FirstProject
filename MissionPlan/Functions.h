#ifndef FUNCTION_H
#define FUNCTIONS_H

#include <bits/stdc++.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "Supplier.h"

using namespace std;
using namespace sql;

// Function declarations
sql::Connection* createConnection();
Supplier inputSupplier();
void insertSupplier(sql::Connection* con, Supplier S);
void displayAllSupplier(sql::Connection* con);

#endif