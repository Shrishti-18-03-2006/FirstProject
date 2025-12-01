#ifndef CART_H
#define CART_H

class Cart {
private:
    int cart_id;
    int customer_id;
    int product_id;
    int quantity;

public:
    Cart();
    Cart(int cid, int pid, int qty);

    int getCartID() const { return cart_id; }
    int getCustomerID() const { return customer_id; }
    int getProductID() const { return product_id; }
    int getQuantity() const { return quantity; }

    void setCartID(int id) { cart_id = id; }
    void setQuantity(int q) { quantity = q; }
};

#endif
