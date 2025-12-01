// Functions.cpp
#include "Functions.h"
#include "DBFunctions.h"
#include "CartFunctions.h"

#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <iomanip>
#include <sstream>

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

// -------------------- ANSI COLOR HELPERS --------------------
static const string ANSI_RESET = "\033[0m";
static const string ANSI_BOLD = "\033[1m";
static const string ANSI_RED = "\033[31m";
static const string ANSI_GREEN = "\033[32m";
static const string ANSI_YELLOW = "\033[33m";
static const string ANSI_BLUE = "\033[34m";
static const string ANSI_MAGENTA = "\033[35m";
static const string ANSI_CYAN = "\033[36m";

std::string color(const std::string &code, const std::string &text) {
    return code + text + ANSI_RESET;
}

// -------------------- SCREEN HELPERS --------------------
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // use ANSI clear
    std::cout << "\033[2J\033[1;1H";
#endif
}

void pressEnterToContinue() {
    cout << color(ANSI_CYAN, "\nPress ENTER to continue...");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getIntInput(const string &prompt) {
    int value;
    while (true) {
        cout << color(ANSI_YELLOW, prompt);
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << color(ANSI_RED, "Invalid input. Please enter a number.\n");
    }
}

void printFancyHeader(const string &title) {
    cout << color(ANSI_MAGENTA, "═══════════════════════════════════════════════════════════════════════════════\n");
    int pad = (79 - (int)title.size()) / 2;
    if (pad < 0) pad = 0;
    cout << string(pad, ' ') << color(ANSI_BLUE, "🛒  " + title) << "\n";
    cout << color(ANSI_MAGENTA, "═══════════════════════════════════════════════════════════════════════════════\n\n");
}

// -------------------- PRODUCT LIST UTILS --------------------
// Query products for a given category + subcategory and present a numbered list.
// Returns a vector of product IDs in the order shown.
vector<int> showNumberedProducts(sql::Connection* con, const string &category, const string &subcategory) {
    vector<int> productIds;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID, Product_Name, Price, Stock_Qtn "
            "FROM PRODUCT WHERE Category = ? AND Subcategory = ? ORDER BY Product_ID"
        );
        pstmt->setString(1, category);
        pstmt->setString(2, subcategory);
        sql::ResultSet* res = pstmt->executeQuery();

        cout << left << setw(6) << "No."
             << setw(8) << "PID"
             << setw(40) << "Product Name"
             << setw(10) << "Price"
             << setw(8) << "Stock" << "\n";
        cout << string(70, '-') << "\n";

        int idx = 1;
        while (res->next()) {
            int pid = res->getInt("Product_ID");
            string name = res->getString("Product_Name");
            double price = res->getDouble("Price");
            int stock = res->getInt("Stock_Qtn");

            cout << left << setw(6) << idx
                 << setw(8) << pid
                 << setw(40) << name
                 << setw(10) << price
                 << setw(8) << stock << "\n";

            productIds.push_back(pid);
            idx++;
        }

        delete res;
        delete pstmt;
    } catch (sql::SQLException &e) {
        cerr << color(ANSI_RED, string("SQL Error in showNumberedProducts: ") + e.what()) << endl;
    }

    return productIds;
}

// -------------------- PRODUCT DETAILS SCREEN --------------------
void productDetailsScreen(int productId, int customerId, sql::Connection* con) {
    clearScreen();
    printFancyHeader("PRODUCT DETAILS");

    // Display product details using the existing function (it opens its own connection and prints)
    displayProductDetailsById(productId);

    cout << "\n" << color(ANSI_CYAN, "Actions:") << "\n";
    cout << color(ANSI_GREEN, "1. View Supplier Details") << "\n";
    cout << color(ANSI_GREEN, "2. Add this product to Cart") << "\n";
    cout << color(ANSI_GREEN, "0. Back") << "\n";

    int choice = getIntInput("Enter choice: ");

    if (choice == 1) {
        clearScreen();
        printFancyHeader("SUPPLIER DETAILS");
        displaySupplierForProduct(productId);
        pressEnterToContinue();
    }
    else if (choice == 2) {
        int qty = getIntInput("\nEnter quantity: ");
        bool ok = addToCart(con, customerId, productId, qty);
        if (ok) cout << color(ANSI_GREEN, "\n✅ Product added to cart.\n");
        else cout << color(ANSI_RED, "\n❌ Failed to add to cart.\n");
        pressEnterToContinue();
    }
    else {
        // back
    }
}

// -------------------- HANDLER: PRODUCTS IN SUBCATEGORY --------------------
void handleProductsFlow(sql::Connection *con,
                        const string &category,
                        const string &subcategory,
                        int customerId)
{
    bool backFromProducts = false;

    while (!backFromProducts) {
        clearScreen();
        string productsHeader = category + " → " + subcategory;
        printFancyHeader("PRODUCTS — " + productsHeader);

        // Show a numbered view (No need to type Product_ID)
        vector<int> productIds = showNumberedProducts(con, category, subcategory);

        if (productIds.empty()) {
            cout << color(ANSI_YELLOW, "\nNo products found in this subcategory.\n");
            pressEnterToContinue();
            return;
        }

        cout << "\n" << color(ANSI_CYAN, "Options:") << "\n";
        cout << color(ANSI_GREEN, "1) View product details (choose by number)") << "\n";
        cout << color(ANSI_GREEN, "0) Back to subcategory menu") << "\n";

        int innerChoice = getIntInput("👉 Enter your choice: ");

        if (innerChoice == 0) {
            backFromProducts = true;
        }
        else if (innerChoice == 1) {
            int choiceNo = getIntInput("\nEnter item number shown in the list: ");
            if (choiceNo >= 1 && choiceNo <= (int)productIds.size()) {
                int prodId = productIds[choiceNo - 1];
                productDetailsScreen(prodId, customerId, con);
            } else {
                cout << color(ANSI_RED, "Invalid item number.\n");
                pressEnterToContinue();
            }
        }
        else {
            cout << color(ANSI_RED, "Invalid choice. Try again.\n");
            pressEnterToContinue();
        }
    }
}

// -------------------- HANDLER: SUBCATEGORY MENU --------------------
void handleSubcategoryFlow(sql::Connection *con, const string &category, int customerId) {
    bool backToCategories = false;

    while (!backToCategories) {
        clearScreen();
        string header = "SUBCATEGORY MENU — " + category;
        printFancyHeader(header);

        vector<string> subcats = loadSubcategories(con, category);

        if (subcats.empty()) {
            cout << color(ANSI_YELLOW, "No subcategories found for \"" + category + "\".\n\n");
            cout << color(ANSI_CYAN, "1) View all products in this category\n");
            cout << color(ANSI_CYAN, "0) Back\n\n");
            cout << "---------------------------------------\n";

            int c2 = getIntInput("👉 Enter your choice: ");
            if (c2 == 0) {
                backToCategories = true;
            }
            else if (c2 == 1) {
                clearScreen();
                printFancyHeader("PRODUCTS — " + category);
                displayByCategory(con, category);
                pressEnterToContinue();
            }
            else {
                cout << color(ANSI_RED, "Invalid choice. Try again.\n");
            }
            continue;
        }

        cout << color(ANSI_CYAN, "Subcategories in \"" + category + "\":\n\n");
        for (size_t j = 0; j < subcats.size(); ++j) {
            cout << "  " << color(ANSI_YELLOW, to_string(j + 1)) << ") " << subcats[j] << "\n";
        }
        cout << "\n  " << color(ANSI_YELLOW, "0") << ") Back\n\n";
        cout << "---------------------------------------\n";

        int subChoice = getIntInput("👉 Enter your choice: ");

        if (subChoice == 0) {
            backToCategories = true;
        }
        else if (subChoice >= 1 && subChoice <= (int)subcats.size()) {
            string selectedSub = subcats[subChoice - 1];
            handleProductsFlow(con, category, selectedSub, customerId);
        }
        else {
            cout << color(ANSI_RED, "Invalid choice. Try again.\n");
            pressEnterToContinue();
        }
    }
}

// -------------------- RUN CATEGORY MENU --------------------
void runCategoryMenu(sql::Connection *con, int customerId) {
    bool exitProgram = false;

    while (!exitProgram) {
        clearScreen();
        printFancyHeader("CATEGORY MENU");

        vector<string> categories = loadCategories(con);

        cout << color(ANSI_CYAN, "Available Categories:\n\n");
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << "  " << color(ANSI_YELLOW, to_string(i + 1)) << ") " << categories[i] << "\n";
        }

        cout << "\n  " << color(ANSI_YELLOW, to_string(categories.size() + 1)) << ") View ALL Products\n";
        cout << "  " << color(ANSI_YELLOW, to_string(categories.size() + 2)) << ") View Cart\n";
        cout << "  0) Exit\n\n";

        cout << "---------------------------------------\n";
        int choice = getIntInput("👉 Enter your choice: ");

        if (choice == 0) {
            exitProgram = true;
        }
        else if (choice == (int)categories.size() + 1) {
            clearScreen();
            printFancyHeader("ALL PRODUCTS");
            displayAllProducts(con);
            pressEnterToContinue();
        }
        else if (choice == (int)categories.size() + 2) {
            // View Cart - uses the cart menu with the known customerId
            runCartMenu(con, customerId);
        }
        else if (choice >= 1 && choice <= (int)categories.size()) {
            string selectedCategory = categories[choice - 1];
            handleSubcategoryFlow(con, selectedCategory, customerId);
        }
        else {
            cout << color(ANSI_RED, "Invalid choice. Try again.\n");
            pressEnterToContinue();
        }
    }
}
