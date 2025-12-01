#include "CartFunctions.h"
#include <iostream>
#include <iomanip>
#include <limits>

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>

using namespace std;

// ------------------------------------------------------
// ADD TO CART
// ------------------------------------------------------
bool addToCart(sql::Connection* con, int customer_id, int product_id, int qty) {
    if (!con) {
        cout << "❌ No active DB connection.\n";
        return false;
    }
    if (qty <= 0) {
        cout << "❌ Quantity must be positive.\n";
        return false;
    }

    cout << "[DEBUG] addToCart received customer_id = " << customer_id << endl;

    try {
        sql::PreparedStatement* pstmt = nullptr;
        sql::ResultSet* res = nullptr;

        // 1) Check stock
        pstmt = con->prepareStatement("SELECT Stock_Qtn FROM PRODUCT WHERE Product_ID = ?");
        pstmt->setInt(1, product_id);
        res = pstmt->executeQuery();

        if (!res->next()) {
            cout << "❌ Product not found.\n";
            delete res; delete pstmt;
            return false;
        }

        int stock = res->getInt("Stock_Qtn");
        delete res; delete pstmt;

        if (stock < qty) {
            cout << "❌ Not enough stock. Available: " << stock << "\n";
            return false;
        }

        // 2) Check if already in cart
        pstmt = con->prepareStatement(
            "SELECT Quantity FROM Cart WHERE Customer_ID = ? AND Product_ID = ?"
        );
        pstmt->setInt(1, customer_id);
        pstmt->setInt(2, product_id);
        res = pstmt->executeQuery();

        if (res->next()) {
            int existing = res->getInt("Quantity");
            delete res; delete pstmt;

            int newQty = existing + qty;

            // Check stock for combined qty
            pstmt = con->prepareStatement("SELECT Stock_Qtn FROM PRODUCT WHERE Product_ID = ?");
            pstmt->setInt(1, product_id);
            res = pstmt->executeQuery();

            if (res->next() && res->getInt("Stock_Qtn") < newQty) {
                cout << "❌ Combined quantity exceeds stock.\n";
                delete res; delete pstmt;
                return false;
            }

            delete res; delete pstmt;

            // Update cart
            pstmt = con->prepareStatement(
                "UPDATE Cart SET Quantity = ? WHERE Customer_ID = ? AND Product_ID = ?"
            );
            pstmt->setInt(1, newQty);
            pstmt->setInt(2, customer_id);
            pstmt->setInt(3, product_id);

            int rows = pstmt->executeUpdate();
            delete pstmt;

            if (rows > 0) {
                cout << "✅ Quantity updated in cart.\n";
                return true;
            }
            return false;

        } else {
            // Insert new item into cart
            delete res; delete pstmt;

            pstmt = con->prepareStatement(
                "INSERT INTO Cart (Customer_ID, Product_ID, Quantity) "
                "VALUES (?, ?, ?)"
            );
            pstmt->setInt(1, customer_id);
            pstmt->setInt(2, product_id);
            pstmt->setInt(3, qty);

            int rows = pstmt->executeUpdate();
            delete pstmt;

            if (rows > 0) {
                cout << "✅ Added to cart.\n";
                return true;
            }
            return false;
        }
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in addToCart: " << e.what() << endl;
        return false;
    }
}

// ------------------------------------------------------
// VIEW CART
// ------------------------------------------------------
std::vector<Cart> viewCart(sql::Connection* con, int customer_id) {
    vector<Cart> cartItems;

    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement(
                "SELECT c.Product_ID, c.Quantity, p.Product_Name, p.Price "
                "FROM Cart c JOIN PRODUCT p ON c.Product_ID = p.Product_ID "
                "WHERE c.Customer_ID = ?"
            );
        pstmt->setInt(1, customer_id);
        sql::ResultSet* res = pstmt->executeQuery();

        cout << "\n========== YOUR CART ==========\n";
        cout << left << setw(10) << "PID"
             << setw(35) << "Product Name"
             << setw(10) << "Qty"
             << setw(10) << "Price"
             << setw(10) << "Subtotal"
             << endl;
        cout << string(75, '-') << endl;

        double total = 0.0;

        while (res->next()) {
            int pid = res->getInt("Product_ID");
            int qty = res->getInt("Quantity");
            string pname = res->getString("Product_Name");
            double price = res->getDouble("Price");
            double subtotal = price * qty;

            cout << left << setw(10) << pid
                 << setw(35) << pname
                 << setw(10) << qty
                 << setw(10) << price
                 << setw(10) << subtotal
                 << endl;

            total += subtotal;

            Cart c(customer_id, pid, qty);
            cartItems.push_back(c);
        }

        cout << string(75, '-') << endl;
        cout << "TOTAL: " << total << endl << endl;

        delete res;
        delete pstmt;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in viewCart: " << e.what() << endl;
    }

    return cartItems;
}

// ------------------------------------------------------
// REMOVE FROM CART
// ------------------------------------------------------
bool removeFromCart(sql::Connection* con, int customer_id, int product_id) {
    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement("DELETE FROM Cart WHERE Customer_ID = ? AND Product_ID = ?");
        pstmt->setInt(1, customer_id);
        pstmt->setInt(2, product_id);

        int rows = pstmt->executeUpdate();
        delete pstmt;

        if (rows > 0) {
            cout << "✅ Removed from cart.\n";
            return true;
        }
        cout << "⚠️ Item not found in cart.\n";
        return false;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in removeFromCart: " << e.what() << endl;
        return false;
    }
}

// ------------------------------------------------------
// UPDATE CART QUANTITY
// ------------------------------------------------------
bool updateCartQuantity(sql::Connection* con, int customer_id, int product_id, int newQty) {
    if (newQty <= 0)
        return removeFromCart(con, customer_id, product_id);

    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement(
                "UPDATE Cart SET Quantity = ? WHERE Customer_ID = ? AND Product_ID = ?"
            );
        pstmt->setInt(1, newQty);
        pstmt->setInt(2, customer_id);
        pstmt->setInt(3, product_id);

        int rows = pstmt->executeUpdate();
        delete pstmt;

        if (rows > 0) {
            cout << "✅ Quantity updated.\n";
            return true;
        }

        cout << "⚠️ Item not found.\n";
        return false;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in updateCartQuantity: " << e.what() << endl;
        return false;
    }
}

// ------------------------------------------------------
// CHECKOUT (reduces stock + clears cart)
// ------------------------------------------------------
bool checkoutCart(sql::Connection* con, int customer_id) {
    try {
        // Step 1: Load all cart items
        sql::PreparedStatement* pstmt =
            con->prepareStatement(
                "SELECT c.Product_ID, c.Quantity, p.Product_Name, p.Price "
                "FROM Cart c JOIN PRODUCT p ON c.Product_ID = p.Product_ID "
                "WHERE c.Customer_ID = ?"
            );
        pstmt->setInt(1, customer_id);

        sql::ResultSet* res = pstmt->executeQuery();

        vector<int> productIds;
        vector<int> qtys;
        vector<double> prices;
        vector<string> names;

        double total = 0;

        cout << "\n================= RECEIPT =================\n";

        cout << left << setw(10) << "PID"
             << setw(30) << "Product Name"
             << setw(10) << "Qty"
             << setw(10) << "Price"
             << setw(12) << "Subtotal"
             << endl;

        cout << string(75, '-') << endl;

        while (res->next()) {
            int pid = res->getInt("Product_ID");
            int qty = res->getInt("Quantity");
            string pname = res->getString("Product_Name");
            double price = res->getDouble("Price");
            double subtotal = price * qty;

            cout << left << setw(10) << pid
                 << setw(30) << pname
                 << setw(10) << qty
                 << setw(10) << price
                 << setw(12) << subtotal
                 << endl;

            productIds.push_back(pid);
            qtys.push_back(qty);
            prices.push_back(price);
            names.push_back(pname);

            total += subtotal;
        }

        cout << string(75, '-') << endl;
        cout << "TOTAL AMOUNT: " << total << endl;
        cout << "=============================================\n\n";

        delete res;
        delete pstmt;

        // Step 2: Reduce stock
        sql::Statement* stmt = con->createStatement();
        stmt->execute(
            "UPDATE PRODUCT p "
            "JOIN Cart c ON p.Product_ID = c.Product_ID "
            "SET p.Stock_Qtn = p.Stock_Qtn - c.Quantity "
            "WHERE c.Customer_ID = " + to_string(customer_id)
        );

        // Step 3: Clear cart
        stmt->execute("DELETE FROM Cart WHERE Customer_ID = " + to_string(customer_id));

        delete stmt;

        cout << "✅ Checkout completed successfully!\n";
        cout << "🧾 Your receipt is displayed above.\n";
        cout << "🛒 Your cart is now empty.\n";

        return true;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in checkoutCart: " << e.what() << endl;
        return false;
    }
}

// ------------------------------------------------------
// CART MENU
// ------------------------------------------------------
void runCartMenu(sql::Connection* con, int customer_id) {
    while (true) {
        cout << "\n========== CART MENU ==========\n";
        cout << "1) View Cart\n";
        cout << "2) Update Quantity\n";
        cout << "3) Remove Item\n";
        cout << "4) Checkout\n";
        cout << "0) Back\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (choice == 0) return;

        if (choice == 1) {
            viewCart(con, customer_id);
        }
        else if (choice == 2) {
            int pid, qty;
            cout << "Product ID: "; cin >> pid;
            cout << "New Quantity: "; cin >> qty;
            updateCartQuantity(con, customer_id, pid, qty);
        }
        else if (choice == 3) {
            int pid;
            cout << "Product ID to remove: ";
            cin >> pid;
            removeFromCart(con, customer_id, pid);
        }
        else if (choice == 4) {
            checkoutCart(con, customer_id);
        }

        cout << "\nPress ENTER to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}
