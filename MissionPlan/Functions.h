// Functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <cppconn/connection.h>  // for sql::Connection*

// Basic UI helpers
void clearScreen();
void pressEnterToContinue();
int getIntInput(const std::string &prompt);
void printFancyHeader(const std::string &title);
std::string color(const std::string &code, const std::string &text);

// Updated prototypes with customerId parameter
void productDetailsScreen(int productId, int customerId, sql::Connection* con);
void handleProductsFlow(sql::Connection *con,
                        const std::string &category,
                        const std::string &subcategory,
                        int customerId);
void handleSubcategoryFlow(sql::Connection *con,
                           const std::string &category,
                           int customerId);
void runCategoryMenu(sql::Connection *con, int customerId);

#endif
