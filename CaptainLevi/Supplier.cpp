#include "Supplier.h"

Supplier::Supplier() {
    // default constructor (empty or give default values)
}

Supplier::Supplier(int i, string n, long int cnum, string e, string add) {
    this->id = i;
    this->name = n;
    this->cont_num = cnum;
    this->email = e;
    this->address = add;
}
