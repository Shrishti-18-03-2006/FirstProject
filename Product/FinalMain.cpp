#include <iostream>
using namespace std;

// Include headers
#include "Date.h"
#include "Product.h"
#include "DBFunctions.h"

// Include cpp implementations
#include "Date.cpp"
#include "Product.cpp"
#include "DBFunctions.cpp"

// This menu will just display all the Products list available on the store 

int main() {

    try {
        sql::Connection *con = createConnection();

        displayAllProducts(con);
    
        delete con;
    }
    catch (sql::SQLException &e) {
        cout << "Error : " << e.what() << endl;
    }

    return 0;
}