#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <string>
using namespace std;

class Supplier {

public:
        int id;
        string name;
        long int cont_num;
        string email;
        string address;

        Supplier();
        Supplier(int i, string n, long int cnum, string e, string add);

};

#endif
