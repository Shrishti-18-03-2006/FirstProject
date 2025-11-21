#ifndef DATE_H
#define DATE_H

#include <string>
using namespace std;

class Date {
public:
    int day;
    int month;
    int year;

    Date();
    Date(int d, int m, int y);
    void setDate(int d, int m, int y);
    string toSQL();
};

#endif
