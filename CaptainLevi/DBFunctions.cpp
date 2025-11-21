// Functions/DBFunctions.cpp
// Clean DB functions: connection, display (read-only) and dynamic loaders

#include "DBFunctions.h"
#include "Customer.h"
#include <iomanip>
#include <limits>


// -----------------------------
// 1) CONNECT TO DATABASE
// -----------------------------
sql::Connection* createConnection() {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        // update username/password as necessary
        con = driver->connect("tcp://127.0.0.1:3306", "root", "***");
        con->setSchema("PROJECT1");
    } catch (sql::SQLException &e) {
        cout << "Unable to connect to DB: " << e.what() << endl;
        throw;
    }

    return con;
}

// -----------------------------
// 2) DISPLAY BY CATEGORY
// -----------------------------
void displayByCategory(sql::Connection* con, string cat) {
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
        pstmt = con->prepareStatement(
            "SELECT * FROM PRODUCT WHERE Category = ?"
        );
        pstmt->setString(1, cat);
        res = pstmt->executeQuery();

        cout << "\n--- Products in Category: " << cat << " ---\n\n";

        cout << left
             << setw(10) << "Product_ID"
             << setw(28) << "Product_Name"
             << setw(18) << "Category"
             << setw(18) << "Subcategory"
             << setw(10) << "Price"
             << setw(10) << "Stock"
             << setw(20) << "Company"
             << setw(12) << "Expiry"
             << endl;

        cout << string(126, '-') << endl;

        while (res->next()) {
            cout << left
                 << setw(10) << res->getInt("Product_ID")
                 << setw(28) << res->getString("Product_name")
                 << setw(18) << res->getString("Category")
                 << setw(18) << res->getString("Subcategory")
                 << setw(10) << res->getDouble("Price")
                 << setw(10) << res->getInt("Stock_Qtn")
                 << setw(20) << res->getString("Company_name")
                 << setw(12) << res->getString("ExpiryDate")
                 << endl;
        }
    } catch (sql::SQLException &e) {
        cout << "SQL Error in displayByCategory: " << e.what() << endl;
    }

    if (res) delete res;
    if (pstmt) delete pstmt;
}

// -----------------------------
// 3) DISPLAY BY CATEGORY + SUBCATEGORY
// -----------------------------
void displayBySubcategory(sql::Connection* con, string cat, string subcat) {
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
          pstmt = con->prepareStatement(
                "SELECT Product_ID, Product_Name, Price "
                "FROM PRODUCT "
                "WHERE Category = ? AND Subcategory = ? "
                "ORDER BY Product_ID"
);

        pstmt->setString(1, cat);
        pstmt->setString(2, subcat);
        res = pstmt->executeQuery();

        cout << "\n--- Products in " << cat << " → " << subcat << " ---\n\n";

        cout << left
             << setw(6)  << "ID"
             << setw(35) << "Name"
             << setw(10) << "Price"
             << '\n';

        cout << string(6 + 35 + 10, '-') << '\n';


        while (res->next()) {
        cout << left
            << setw(6)  << res->getInt("Product_ID")
            << setw(35) << res->getString("Product_Name")
            << setw(10) << res->getDouble("Price")
            << '\n';
}

    } catch (sql::SQLException &e) {
        cout << "SQL Error in displayBySubcategory: " << e.what() << endl;
    }

    if (res) delete res;
    if (pstmt) delete pstmt;
}

// -----------------------------
// 4) DISPLAY ALL PRODUCTS
// -----------------------------
void displayAllProducts(sql::Connection *con) {
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;

    try {
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM PRODUCT");

        cout << left
             << setw(10) << "Product_ID"
             << setw(28) << "Product_Name"
             << setw(18) << "Category"
             << setw(18) << "Subcategory"
             << setw(10) << "Price"
             << setw(10) << "Stock_Qtn"
             << setw(20) << "Company_Name"
             << setw(12) << "ExpiryDate"
             << endl;

        cout << string(126, '-') << endl;

        while (res->next()) {
            cout << left
                 << setw(10) << res->getInt("Product_ID")
                 << setw(28) << res->getString("Product_name")
                 << setw(18) << res->getString("Category")
                 << setw(18) << res->getString("Subcategory")
                 << setw(10) << res->getDouble("Price")
                 << setw(10) << res->getInt("Stock_Qtn")
                 << setw(20) << res->getString("Company_name")
                 << setw(12) << res->getString("ExpiryDate")
                 << endl;
        }
    } catch (sql::SQLException &e) {
        cout << "SQL Error in displayAllProducts: " << e.what() << endl;
    }

    if (res) delete res;
    if (stmt) delete stmt;
}

// -----------------------------
// 5) LOAD DISTINCT CATEGORIES (vector-based)
// -----------------------------
vector<string> loadCategories(sql::Connection* con) {
    vector<string> categories;
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;

    try {
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT DISTINCT Category FROM PRODUCT ORDER BY Category ASC");

        while (res->next()) {
            string cat = res->getString("Category");
            if (!cat.empty())
                categories.push_back(cat);
        }
    } catch (sql::SQLException &e) {
        cout << "SQL Error in loadCategories: " << e.what() << endl;
    }

    if (res) delete res;
    if (stmt) delete stmt;

    return categories;
}

// -----------------------------
// 6) LOAD DISTINCT SUBCATEGORIES (vector-based)
// -----------------------------
vector<string> loadSubcategories(sql::Connection* con, const string &category) {
    vector<string> subcats;
    sql::PreparedStatement *pstmt = nullptr;
    sql::ResultSet *res = nullptr;

    try {
        pstmt = con->prepareStatement(
            "SELECT DISTINCT Subcategory FROM PRODUCT WHERE Category = ? ORDER BY Subcategory ASC"
        );
        pstmt->setString(1, category);
        res = pstmt->executeQuery();

        while (res->next()) {
            string s = res->getString("Subcategory");
            if (!s.empty())
                subcats.push_back(s);
        }
    } catch (sql::SQLException &e) {
        cout << "SQL Error in loadSubcategories: " << e.what() << endl;
    }

    if (res) delete res;
    if (pstmt) delete pstmt;

    return subcats;
}

// --------------------------
// 7) Take Customer details & save to CUSTOMER_DETAILS
// --------------------------
void inputCustomerDetails(sql::Connection* con, const std::string& emailFromLogin) {
    if (!con) {
        cout << "❌ No active DB connection.\n";
        return;
    }

    Customer c;
    int id;
    std::string name, contact, address;

    cout << "\n=====================================\n";
    cout << "         SAVE CUSTOMER DETAILS       \n";
    cout << "=====================================\n\n";

    cout << "Enter Customer ID: ";
    cin >> id;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "Enter Full Name: ";
    std::getline(cin, name);

    cout << "Enter Contact Number: ";
    std::getline(cin, contact);

    // We do NOT ask for email again.
    // We use the same email that was used during login.
    cout << "Using Email from login: " << emailFromLogin << "\n";

    cout << "Enter Address: ";
    std::getline(cin, address);

    c.setId(id);
    c.setName(name);
    c.setContactnumber(contact);
    c.setEmail(emailFromLogin);
    c.setAddress(address);

    // Now insert this customer into the DB
    insertCustomerDetails(con, c);
}

void insertCustomerDetails(sql::Connection* con, const Customer& c) {
    if (!con) {
        cout << "❌ No active DB connection.\n";
        return;
    }

    sql::PreparedStatement *pstmt = nullptr;

    try {
        pstmt = con->prepareStatement(
            "INSERT INTO CUSTOMER_DETAILS (ID, Name, Contact_Num, Email, Address) "
            "VALUES (?, ?, ?, ?, ?)"
        );

        pstmt->setInt(1, c.getId());
        pstmt->setString(2, c.getName());
        pstmt->setString(3, c.getContactnumber());
        pstmt->setString(4, c.getEmail());
        pstmt->setString(5, c.getAddress());

        int rows = pstmt->executeUpdate();

        if (rows > 0) {
            cout << "\n✅ Customer details saved successfully in CUSTOMER_DETAILS.\n";
        } else {
            cout << "\n⚠️ No rows inserted. Something went wrong.\n";
        }
    }
    catch (sql::SQLException &e) {
        cout << "SQL Error in insertCustomerDetails: " << e.what() << endl;
    }

    if (pstmt) delete pstmt;
}


// (we will NOT print Product_ID or SID to the user)
void displayProductDetailsById(int productId) {
    sql::Connection* con = nullptr;
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
        con = createConnection();

        // We use Product_ID internally, but we don't print it
        pstmt = con->prepareStatement(
            "SELECT Product_Name, Category, Subcategory, "
            "       Price, Stock_Qtn, Company_name, ExpiryDate "
            "FROM PRODUCT "
            "WHERE Product_ID = ?"
        );
        pstmt->setInt(1, productId);   // we pass the ID from C++ (not from user typing)

        res = pstmt->executeQuery();

        if (res->next()) {
            cout << "\n========== PRODUCT DETAILS ==========\n";
            cout << "Product Name   : " << res->getString("Product_Name") << '\n';
            cout << "Category       : " << res->getString("Category") << '\n';
            cout << "Subcategory    : " << res->getString("Subcategory") << '\n';
            cout << "Price          : " << res->getDouble("Price") << '\n';
            cout << "Stock Quantity : " << res->getInt("Stock_Qtn") << '\n';
            cout << "Brand/Company  : " << res->getString("Company_name") << '\n';
            cout << "Expiry Date    : " << res->getString("ExpiryDate") << '\n';
        } else {
            cout << "\nNo product found for this selection.\n";
        }

        delete res;
        delete pstmt;
        con->close();
        delete con;
    }
    catch (sql::SQLException& e) {
        cerr << "\n[ERROR] Failed to fetch product details: " << e.what() << endl;
        if (res) delete res;
        if (pstmt) delete pstmt;
        if (con) { con->close(); delete con; }
    }
}

// Show supplier details for a given product
// (we join PRODUCT -> Supplier using SID, but we only display Sname, Contact_Num, Email, Address)
void displaySupplierForProduct(int productId) {
    sql::Connection* con = nullptr;
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
        con = createConnection();

        pstmt = con->prepareStatement(
            "SELECT s.Sname, s.Contact_Num, s.Email, s.Address "
            "FROM PRODUCT p "
            "JOIN Supplier s ON p.SID = s.SID "
            "WHERE p.Product_ID = ?"
        );
        pstmt->setInt(1, productId);

        res = pstmt->executeQuery();

        if (res->next()) {
            cout << "\n====== SUPPLIER DETAILS ======\n";
            cout << "Supplier Name   : " << res->getString("Sname") << '\n';
            cout << "Contact Number  : " << res->getString("Contact_Num") << '\n';
            cout << "Email           : " << res->getString("Email") << '\n';
            cout << "Address         : " << res->getString("Address") << '\n';
        } else {
            cout << "\nNo supplier found for this product.\n";
        }

        delete res;
        delete pstmt;
        con->close();
        delete con;
    }
    catch (sql::SQLException& e) {
        cerr << "\n[ERROR] Failed to fetch supplier details: " << e.what() << endl;
        if (res) delete res;
        if (pstmt) delete pstmt;
        if (con) { con->close(); delete con; }
    }
}