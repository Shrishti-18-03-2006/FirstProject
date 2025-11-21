#include "Date.h"

Date::Date() {}

Date::Date(int d, int m, int y) {
    day = d;
    month = m;
    year = y;
}

void Date::setDate(int d, int m, int y) {
    day = d;
    month = m;
    year = y;
}

string Date::toSQL() {
    return to_string(year) + "-" + to_string(month) + "-" + to_string(day);
}