#include <cstdlib>
#include <iostream>
#include <string>

#include "menuorder.h"
#include "ordering.h"
#include "confirmedorders.h"
#include "logs.h"
#include "seating_api.h"

int main()
{
    if (!loadActiveOrders())
    {
        std::cerr << "Could not load active orders.\n";
        return EXIT_FAILURE;
    }

    if (expireUnpaidOrders() < 0)
    {
        std::cerr << "Could not expire overdue orders.\n";
        return EXIT_FAILURE;
    }

    displayConfirmedOrders();

    std::string studentID;

    std::cout << "\nEnter your Student ID or an active Order ID to pay: ";

    if (!(std::cin >> studentID))
    {
        std::cerr << "Invalid student ID.\n";
        return EXIT_FAILURE;
    }

    if (studentID.rfind("ORD", 0) == 0)
    {
        if (!findActiveOrder(studentID.c_str()))
        {
            std::cerr << "That order ID is not active.\n";
            return EXIT_FAILURE;
        }

        int paymentChoice;
        while (true)
        {
            std::cout << "Enter 1 to pay order " << studentID << ": ";
            if (!(std::cin >> paymentChoice))
            {
                std::cerr << "Invalid payment input.\n";
                return EXIT_FAILURE;
            }

            if (paymentChoice == 1)
            {
                if (expireUnpaidOrders() < 0)
                {
                    std::cerr << "Could not check order expiration.\n";
                    return EXIT_FAILURE;
                }

                if (!findActiveOrder(studentID.c_str()))
                {
                    std::cerr << "This order has expired and is no longer payable.\n";
                    return EXIT_FAILURE;
                }

                if (!payOrder(studentID.c_str()))
                {
                    std::cerr << "Payment could not be completed.\n";
                    return EXIT_FAILURE;
                }

                std::cout << "Payment successful. Order " << studentID
                          << " is complete and has been removed from the active orders.\n";
                displayConfirmedOrders();
                return EXIT_SUCCESS;
            }

            std::cout << "Payment failed. Enter 1 to try payment again.\n";
        }
    }

    if (studentID == "777")
    {
        int adminChoice;

        std::cout << "\nWELCOME ADMIN\n"
                  << "1. View logs\n"
                  << "Choose an option: ";

        if (!(std::cin >> adminChoice))
        {
            std::cerr << "Invalid admin selection.\n";
            return EXIT_FAILURE;
        }

        if (adminChoice != 1)
        {
            std::cerr << "Invalid admin selection.\n";
            return EXIT_FAILURE;
        }

        displayOrderLogs();
        return EXIT_SUCCESS;
    }

    std::cout << "Welcome to the Canteen!\n"
              << "1. View Kitchen Menu\n"
              << "2. View Confectionery Menu\n"
              << "3. Exit\n"
              << "Choose an option: ";

    int menuChoice;

    if (!(std::cin >> menuChoice))
    {
        std::cerr << "Invalid menu selection.\n";
        return EXIT_FAILURE;
    }

    if (menuChoice == 3)
    {
        return EXIT_SUCCESS;
    }

    if (menuChoice != 1 && menuChoice != 2)
    {
        std::cerr << "Invalid menu selection.\n";
        return EXIT_FAILURE;
    }

    MenuItem *selectedMenu;
    int selectedCount;

    if (menuChoice == 1)
    {
        showKitchenMenu();
        selectedMenu = kitchenMenu;
        selectedCount = kitchenCount;
    }
    else
    {
        showConfectioneryMenu();
        selectedMenu = confectioneryMenu;
        selectedCount = confectioneryCount;
    }

    Order order;
    bool hasItems = false;

    while (true)
    {
        int itemChoice;

        std::cout << "Enter an item number (0 to finish): ";

        if (!(std::cin >> itemChoice))
        {
            std::cerr << "Invalid item number.\n";
            return EXIT_FAILURE;
        }

        if (itemChoice == 0)
        {
            break;
        }

        if (itemChoice < 1 || itemChoice > selectedCount)
        {
            std::cerr << "Item number is outside the selected menu.\n";
            return EXIT_FAILURE;
        }

        order.addItem(selectedMenu[itemChoice - 1]);
        hasItems = true;

        int viewCartChoice;

        std::cout << "View your cart? (1 = Yes, 0 = No): ";

        if (!(std::cin >> viewCartChoice) ||
            (viewCartChoice != 0 && viewCartChoice != 1))
        {
            std::cerr << "Enter 1 for Yes or 0 for No.\n";
            return EXIT_FAILURE;
        }

        if (viewCartChoice == 1)
        {
            order.showCart();
        }
    }

    if (!hasItems)
    {
        std::cout << "No items were added; no order was placed.\n";
        return EXIT_SUCCESS;
    }

    order.showCart();

    if (menuChoice == 2)
    {
        int paymentChoice;
        while (true)
        {
            std::cout << "\nPay now to place this confectionery order? (Enter 1 to pay): ";
            if (!(std::cin >> paymentChoice))
            {
                std::cerr << "Invalid payment input.\n";
                return EXIT_FAILURE;
            }

            if (paymentChoice == 1)
            {
                char orderID[20];
                if (!completePaidOrder(
                    studentID.c_str(),
                    order.getCart(),
                    order.getCount(),
                    order.getTotal(),
                    0,
                    orderID,
                    sizeof(orderID)
                ))
                {
                    std::cerr << "Confectionery order could not be completed.\n";
                    return EXIT_FAILURE;
                }

                std::cout << "Payment successful. Confectionery order "
                          << orderID << " is placed and complete.\n";
                return EXIT_SUCCESS;
            }

            std::cout << "Payment failed. Enter 1 to try payment again.\n";
        }
    }

    int confirmChoice;

    std::cout << "\nConfirm order placement? (1 = Yes, 0 = No): ";

    if (!(std::cin >> confirmChoice) ||
        (confirmChoice != 0 && confirmChoice != 1))
    {
        std::cerr << "Enter 1 for Yes or 0 for No.\n";
        return EXIT_FAILURE;
    }

    if (confirmChoice == 1)
    {
        if (expireUnpaidOrders() < 0)
        {
            std::cerr << "Could not check for expired orders.\n";
            return EXIT_FAILURE;
        }

        showSeatingArrangement();
        int seatNumber;
        std::cout << "\nChoose an available seat (1-6): ";
        if (!(std::cin >> seatNumber) || !reserveTable(seatNumber))
        {
            std::cerr << "Invalid or occupied seat. Order was not placed.\n";
            return EXIT_FAILURE;
        }

        char orderID[20];
        if (!pushOrder(
            studentID.c_str(),
            order.getCart(),
            order.getCount(),
            order.getTotal(),
            seatNumber,
            orderID,
            sizeof(orderID)
        ))
        {
            releaseTable(seatNumber);
            std::cerr << "Order could not be placed.\n";
            return EXIT_FAILURE;
        }
        std::cout << "\nOrder placed successfully!\n";
        std::cout << "Your Order ID is: " << orderID << '\n';

        displayConfirmedOrders();
    }
    else
    {
        std::cout << "\nOrder was not placed.\n";
    }

    return EXIT_SUCCESS;
}