#include <iostream>
#include <vector>
#include "menuorder.h"
using namespace std;

class Order {
    vector<MenuItem> cart;
    float total = 0.0;

public:
    void addItem(const MenuItem& item) {
        cart.push_back(item);
        total += item.price;
        cout << item.name << " added. Price: Rs " << item.price << endl;
    }

    void showCart() {
        cout << "\n--- Your Order ---\n";
        for(auto &item : cart) {
            cout << item.category << " - " << item.name << " : Rs " << item.price << endl;
        }
        cout << "Total: Rs " << total << endl;
    }
};

int main() {
    Order myOrder;
    int choice, itemNo;

    cout << "Select 1 for Kitchen Menu, 2 for Confectionery Menu: ";
    cin >> choice;

    if(choice == 1) {
        showKitchenMenu();
        cout << "Enter item number to order: ";
        cin >> itemNo;
        if(itemNo >= 1 && itemNo <= kitchenCount)
            myOrder.addItem(kitchenMenu[itemNo-1]);
    } else if(choice == 2) {
        showConfectioneryMenu();
        cout << "Enter item number to order: ";
        cin >> itemNo;
        if(itemNo >= 1 && itemNo <= confectioneryCount)
            myOrder.addItem(confectioneryMenu[itemNo-1]);
    }

    myOrder.showCart();
    return 0;
}
