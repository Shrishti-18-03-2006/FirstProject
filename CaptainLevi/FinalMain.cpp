// Menu/FinalMain.cpp
// Dynamic, vector-based Category -> Subcategory -> Product navigation
// CaptainLevi/FinalMain.cpp

#include "Customer.h"
#include "login.h"
#include "DBFunctions.h"

#include <vector>
#include <limits>   // numeric_limits
#include <cstdlib>  // system("clear") or system("cls")
#include <sstream>

using namespace std;

// Helper: clear screen (works on Windows or Unix)
static void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Helper: pause until user presses Enter
static void pressEnterToContinue() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Print the header box
void printFancyHeader(const string &title) {
    cout << "═══════════════════════════════════════════════════════════════════════════════\n";
    int pad = (79 - (int)title.size()) / 2;
    if (pad < 0) pad = 0;
    cout << string(pad, ' ') << "🛒  " << title << "\n";
    cout << "═══════════════════════════════════════════════════════════════════════════════\n\n";
}



void productDetailsScreen(int productId) {
    // ✅ Clear before showing product details
    clearScreen();
    printFancyHeader("PRODUCT DETAILS");

    // 1) Show only product details (no Product_ID, no SID)
    displayProductDetailsById(productId);

    // 2) Ask if user wants supplier details
    int choice;
    cout << "\n----------------------------------\n";
    cout << "1. View Supplier Details\n";
    cout << "0. Back\n";
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear buffer

    if (choice == 1) {
        // ✅ Clear before showing supplier details
        clearScreen();
        printFancyHeader("SUPPLIER DETAILS");

        displaySupplierForProduct(productId);
    } else {
        cout << "\nReturning to previous menu...\n";
    }
}



// Main interactive loop
int main() {
    // Create DB connection
    sql::Connection* con = nullptr;
    try {
        con = createConnection();
    } 
    catch (sql::SQLException &e) {
        cout << "Failed to connect to DB: " << e.what() << endl;
        return 1;
    }

    //Taking login details

    std::string loggedInEmail;
    if (!loginWindow(con, loggedInEmail)) {
        cout << "\nExiting due to login failure.\n";
        if (con) {
            try {
                con->close();
                delete con;
            } catch (...) {}
        }
        return 1;
    }
    cout << "\nPress ENTER to continue...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clearScreen();

    // Taking customer details — using same email as login
    inputCustomerDetails(con, loggedInEmail);
    bool exitProgram = false;

    //-------------------------------
    //Product details to be desplayed:-
    //-------------------------------

    while (!exitProgram) {
        clearScreen();
        printFancyHeader("CATEGORY MENU");

        // Load categories dynamically
        vector<string> categories = loadCategories(con);

        // Show menu
        cout << "Available Categories:\n\n";
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << "  " << (i + 1) << ") " << categories[i] << "\n";
        }
        cout << "\n  " << (categories.size() + 1) << ") View ALL Products\n";
        cout << "  0) Exit\n\n";

        cout << "---------------------------------------\n";
        cout << "👉 Enter your choice: ";

        int choice;
        if (!(cin >> choice)) {
            // invalid input: clear and loop
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush newline

        if (choice == 0) {
            exitProgram = true;
            break;
        } 
        else if (choice == static_cast<int>(categories.size() + 1)) {
            // View all products
            clearScreen();
            printFancyHeader("ALL PRODUCTS");
            displayAllProducts(con);
            cout << "\n---------------------------------------\n";
            pressEnterToContinue();
            continue;
        } 
        else if (choice >= 1 && choice <= static_cast<int>(categories.size())) {
            // User selected a category
            string selectedCategory = categories[choice - 1];

            bool backToCategories = false;
            while (!backToCategories) {
                clearScreen();
                string header = "SUBCATEGORY MENU — " + selectedCategory;
                printFancyHeader(header);

                // Load subcategories dynamically
                vector<string> subcats = loadSubcategories(con, selectedCategory);

                if (subcats.empty()) {
                    cout << "No subcategories found for \"" << selectedCategory << "\".\n\n";
                    cout << "1) View all products in this category\n";
                    cout << "0) Back\n\n";
                    cout << "---------------------------------------\n";
                    cout << "👉 Enter your choice: ";

                    int c2;
                    if (!(cin >> c2)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (c2 == 0) {
                        backToCategories = true;
                        break;
                    } else if (c2 == 1) {
                        clearScreen();
                        printFancyHeader("PRODUCTS — " + selectedCategory);
                        displayByCategory(con, selectedCategory);
                        cout << "\n---------------------------------------\n";
                        pressEnterToContinue();
                        continue;
                    } else {
                        continue;
                    }
                }

                // show subcategories
                cout << "Subcategories in \"" << selectedCategory << "\":\n\n";
                for (size_t j = 0; j < subcats.size(); ++j) {
                    cout << "  " << (j + 1) << ") " << subcats[j] << "\n";
                }
                cout << "\n  0) Back\n\n";
                cout << "---------------------------------------\n";
                cout << "👉 Enter your choice: ";

                int subChoice;
                if (!(cin >> subChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (subChoice == 0) {
                    backToCategories = true;
                    break;
                }
else if (subChoice >= 1 && subChoice <= static_cast<int>(subcats.size())) {
    string selectedSub = subcats[subChoice - 1];

    bool backFromProducts = false;
    while (!backFromProducts) {
        clearScreen();
        string productsHeader = selectedCategory + " → " + selectedSub;
        printFancyHeader("PRODUCTS — " + productsHeader);

        // Display products in chosen category+subcategory
        displayBySubcategory(con, selectedCategory, selectedSub);

        cout << "\n---------------------------------------\n";
        cout << "1) View product details\n";
        cout << "0) Back to subcategory menu\n";
        cout << "👉 Enter your choice: ";

        int innerChoice;
        if (!(cin >> innerChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (innerChoice == 0) {
            backFromProducts = true;
            break;
                } else if (innerChoice == 1) {
            int prodId;
            cout << "\nEnter Product_ID of the product you want to view: ";
            if (!(cin >> prodId)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // productDetailsScreen will handle clearScreen + headers now
            productDetailsScreen(prodId);

            cout << "\n---------------------------------------\n";
            pressEnterToContinue();
        } else {
 
            continue;
        }
    }
}

 else {
                    // invalid choice, retry
                    continue;
                }
            } // end subcategory loop

        } 
        else {
            // invalid category choice
            continue;
        }
    } // end main loop

    // Close DB connection
    if (con) {
        try {
            con->close();
            delete con;
        } catch (...) {
            // ignore
        }
    }

    clearScreen();
    cout << "Goodbye! 👋\n";
    return 0;
}
