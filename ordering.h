#ifndef ORDERING_H
#define ORDERING_H

#include "menuorder.h"

class Order {
    MenuItem cart[50];
    int count = 0;
    float total = 0.0f;

public:
    void addItem(const MenuItem &item);
    void showCart() const;

    MenuItem* getCart();
    int getCount() const;
    float getTotal() const;
};

 #endif