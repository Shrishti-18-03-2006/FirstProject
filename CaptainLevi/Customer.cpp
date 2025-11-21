#include "Customer.h"

// Default Constructor
Customer::Customer() {
    id = 0;
    name = "";
    cont_num = "";
    email = "";
    address = "";
}

// Constructor used when loading product data from the database
Customer::Customer(int i, const std::string& n,
                   const std::string& num,
                   const std::string& e,
                   const std::string& add)
{
    id = i;
    name = n;
    cont_num = num;
    email = e;
    address = add;
}
