#include <iostream>
#include "menuorder.h"
using namespace std;

struct CartItem {
    MenuItem item;
};

class Order {
    CartItem cart[50];   // fixed-size array for cart items
    int count = 0;       // number of items added
    float total = 0.0;

public:
    void addItem(const MenuItem& item) {
        if(count < 50) {
            cart[count].item = item;
            count++;
            total += item.price;
            cout << item.name << " added. Price: Rs " << item.price << endl;
        } else {
            cout << "Cart is full!" << endl;
        }
    }

    void showCart() {
        cout << "\n--- Your Order ---\n";
        for(int i = 0; i < count; i++) {
            cout << cart[i].item.category << " - " 
                 << cart[i].item.name << " : Rs " 
                 << cart[i].item.price << endl;
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
