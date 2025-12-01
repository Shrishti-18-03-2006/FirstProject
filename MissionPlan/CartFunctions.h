#ifndef CARTFUNCTIONS_H
#define CARTFUNCTIONS_H

#include "Cart.h"
#include <vector>
#include <cppconn/connection.h>

bool addToCart(sql::Connection* con, int customer_id, int product_id, int qty);
std::vector<Cart> viewCart(sql::Connection* con, int customer_id);
bool removeFromCart(sql::Connection* con, int customer_id, int product_id);
bool updateCartQuantity(sql::Connection* con, int customer_id, int product_id, int newQty);
bool checkoutCart(sql::Connection* con, int customer_id);

void runCartMenu(sql::Connection* con, int customer_id);

#endif
