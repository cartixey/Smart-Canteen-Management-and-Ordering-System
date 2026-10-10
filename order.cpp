#include <iostream>

#include "ordering.h"

void Order::addItem(const MenuItem &item) {
    if (count == 50) {
        std::cout << "Cart is full!\n";
        return;
    }

    cart[count++] = item;
    total += item.price;
    std::cout << item.name << " added. Price: Rs " << item.price << '\n';
}

void Order::showCart() const {
    std::cout << "\n--- Your Order ---\n";
    for (int i = 0; i < count; ++i) {
        std::cout << cart[i].category << " - " << cart[i].name
                  << " : Rs " << cart[i].price << '\n';
    }
    std::cout << "Total: Rs " << total << '\n';
}
