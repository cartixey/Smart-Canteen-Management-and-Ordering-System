#include <cstdlib>
#include <iostream>
#include <string>

#include "menuorder.h"
#include "ordering.h"

int main() {
    std::string studentID;
    std::cout << "Enter your Student ID: ";
    if (!(std::cin >> studentID)) {
        std::cerr << "Invalid student ID.\n";
        return EXIT_FAILURE;
    }

    std::cout << "Welcome to the Canteen, " << "!\n"
              << "1. View Kitchen Menu\n"
              << "2. View Confectionery Menu\n"
              << "3. Exit\n"
              << "Choose an option: ";

    int menuChoice;
    if (!(std::cin >> menuChoice)) {
        std::cerr << "Invalid menu selection.\n";
        return EXIT_FAILURE;
    }
    if (menuChoice == 3) {
        return EXIT_SUCCESS;
    }
    if (menuChoice != 1 && menuChoice != 2) {
        std::cerr << "Invalid menu selection.\n";
        return EXIT_FAILURE;
    }

    MenuItem *selectedMenu;
    int selectedCount;
    if (menuChoice == 1) {
        showKitchenMenu();
        selectedMenu = kitchenMenu;
        selectedCount = kitchenCount;
    } else {
        showConfectioneryMenu();
        selectedMenu = confectioneryMenu;
        selectedCount = confectioneryCount;
    }

    Order order;
    bool hasItems = false;
    while (true) {
        std::cout << "Enter an item number (0 to finish): ";
        int itemChoice;
        if (!(std::cin >> itemChoice)) {
            std::cerr << "Invalid item number.\n";
            return EXIT_FAILURE;
        }
        if (itemChoice == 0) {
            break;
        }
        if (itemChoice < 1 || itemChoice > selectedCount) {
            std::cerr << "Item number is outside the selected menu.\n";
            return EXIT_FAILURE;
        }

        order.addItem(selectedMenu[itemChoice - 1]);
        hasItems = true;

        std::cout << "View your cart? (1 = Yes, 0 = No): ";
        int viewCartChoice;
        if (!(std::cin >> viewCartChoice) ||
            (viewCartChoice != 0 && viewCartChoice != 1)) {
            std::cerr << "Enter 1 for Yes or 0 for No.\n";
            return EXIT_FAILURE;
        }
        if (viewCartChoice == 1) {
            order.showCart();
        }
    }

    if (!hasItems) {
        std::cout << "No items were added; no order was placed.\n";
        return EXIT_SUCCESS;
    }

    order.showCart();
    std::cout << "Confirm order placement? (1 = Yes, 0 = No): ";
    int confirmChoice;
    if (!(std::cin >> confirmChoice) ||
        (confirmChoice != 0 && confirmChoice != 1)) {
        std::cerr << "Enter 1 for Yes or 0 for No.\n";
        return EXIT_FAILURE;
    }
    if (confirmChoice == 1) {
        std::cout << "Order placed successfully!\n";
    } else {
        std::cout << "Order was not placed.\n";
    }

    return EXIT_SUCCESS;
}
