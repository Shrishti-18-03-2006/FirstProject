#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>
class Customer{
    private:
        int id;
        std::string name;
        std::string cont_num;
        std::string email;
        std::string address;

    public:
        Customer();

        Customer(int i,
             const std::string& n,
             const std::string& num,
             const std::string& e,
             const std::string& add);

        // Getters
        int getId() const { return id; }
        std::string getName() const { return name; }
        std::string getContactnumber() const { return cont_num; }
        std::string getEmail() const { return email; }
        std::string getAddress() const { return address; }

        // Setters
        void setId(int id) { this->id = id; }
        void setName(const std::string& name) { this->name = name; }
        void setContactnumber(const std::string& cnumber) { this->cont_num = cnumber; }
        void setEmail(const std::string& eml) { this-> email = eml; }
        void setAddress(const std::string& ad) {this-> address = ad; }

};

#endif