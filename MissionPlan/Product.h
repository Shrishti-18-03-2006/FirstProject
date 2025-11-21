#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include "Date.h"
using namespace std;

class Product {
private:
    int Product_id;
    string Product_name;
    string Category;
    string Subcategory;
    float Price;
    int Stock_Qtn;
    string Company_name;
    Date ExpiryDate;

public:
    // Default Constructor
    Product();

    // Constructor used when loading from DB
    Product(int id, string name, string cat, string subc,
            float price, int qty, string comp, Date exp);

    // Getters
    int getId() const { return Product_id; }
    string getName() const { return Product_name; }
    string getCategory() const { return Category; }
    string getSubcategory() const { return Subcategory; }
    float getPrice() const { return Price; }
    int getStock() const { return Stock_Qtn; }
    string getCompany() const { return Company_name; }
    Date getExpiry() const { return ExpiryDate; }

    // Setters
    void setId(int id) { Product_id = id; }
    void setName(const string& name) { Product_name = name; }
    void setCategory(const string& cat) { Category = cat; }
    void setSubcategory(const string& subc) { Subcategory = subc; }
    void setPrice(float price) { Price = price; }
    void setStock(int qty) { Stock_Qtn = qty; }
    void setCompany(const string& comp) { Company_name = comp; }
    void setExpiry(const Date& exp) { ExpiryDate = exp; }
};

#endif
