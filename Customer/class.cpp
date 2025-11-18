#include<bits/stdc++.h>
using namespace std;

class Customer{

    public:
        int id;
        string name;
        long int cont_num;
        string email;
        string address;

        Customer();
        Customer(int i, string n, long int cnum, string e, string add){
            this-> id = i;
            this-> name = n;
            this-> cont_num = cnum;
            this-> email = e;
            this-> address = add;
        }

        void display(){
            cout << "------------------------------------"<< endl;
            cout << "PRODUCT" << "\t\t" << "DETAILS"<< endl;
            cout << "------------------------------------"<< endl;
            cout << "id" << "\t\t" << this->id << endl;
            cout << "name"<< "\t\t" << this-> name<< endl;
            cout << "Phone number" << "\t" << this->cont_num << endl;
            cout << "email"<< "\t\t" << this->email<< endl;
            cout << "address"<< "\t\t" << this->address << endl;
            cout << "------------------------------------" << endl;
        }

};

int main(){
    Customer Cu(101,"Diva",88264733315, "diva@gmail.com","Bandra");
    Cu.display();

    return 0;
}

