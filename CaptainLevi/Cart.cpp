#include "Cart.h"

Cart::Cart() : cart_id(0), customer_id(0), product_id(0), quantity(0) {}

Cart::Cart(int cid, int pid, int qty)
    : cart_id(0), customer_id(cid), product_id(pid), quantity(qty) {}
