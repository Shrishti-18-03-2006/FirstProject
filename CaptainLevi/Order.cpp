#include<bits/stdc++.h>
using namespace std;

class Date{
    public:
    int day;
    int month;
    int year;
    Date(){}
    Date(int d, int m, int y) {
            day = d;
            month = m;
            year = y;
    }
    void setdate(int d, int m, int y){
            this-> day = d;
            this-> month = m;
            this-> year = y;
    }

    void getdate(){
            cout << day << "-" << month << "-" << year << endl;
    }

};

class Order{

    public:
        int id;
        int cus_id;
        float TotalAmt;
        Date OrdDate;

        Order();
        Order(int i,int cid, float amt){
            this-> id = i;
            this-> cus_id = cid;
            this-> TotalAmt = amt;
            this-> OrdDate.setdate(18,03,2006);
        }

        void display(){
            cout << "------------------------------------"<< endl;
            cout << "PRODUCT" << "\t\t" << "DETAILS"<< endl;
            cout << "------------------------------------"<< endl;
            cout << "id" << "\t\t" << this->id << endl;
            cout << "Customer id"<< "\t" << this-> cus_id<< endl;
            cout << "Total Amt" << "\t" << this->TotalAmt << endl;
            cout << "Odre Date"<< "\t";
            this->OrdDate.getdate();
            cout << "------------------------------------" << endl;
        }

};

int main(){
    Order O(101,10001,900.99);
    O.display();

    return 0;
}

