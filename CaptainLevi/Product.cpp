#include "Product.h"

// Default Constructor
Product::Product() {
    Product_id = 0;
    Product_name = "";
    Category = "";
    Subcategory = "";
    Price = 0.0f;
    Stock_Qtn = 0;
    Company_name = "";
    ExpiryDate = Date();  // default Date constructor
}

// Constructor used when loading product data from the database
Product::Product(int id, string name, string cat, string subc,
                 float price, int qty, string comp, Date exp)
{
    Product_id = id;
    Product_name = name;
    Category = cat;
    Subcategory = subc;
    Price = price;
    Stock_Qtn = qty;
    Company_name = comp;
    ExpiryDate = exp;
}
