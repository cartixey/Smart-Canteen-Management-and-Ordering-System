# Smart Canteen Management and Ordering System

A console-based canteen management and food ordering system built using C and C++. The application allows students to browse menus, place orders, reserve seats, make payments, and track active orders.

## Features

- Kitchen and confectionery menus
- Shopping cart functionality
- Student ID-based ordering
- Kitchen order queue management
- Seat allocation for kitchen orders
- Active unpaid order tracking
- Order payment using an order ID
- Automatic expiration of unpaid orders after two hours
- Completed order logging
- Expired order tracking
- Admin access to view order logs
- Persistent order storage using text files
- C and C++ module integration

## Menu Types

### Kitchen Menu

Kitchen orders are placed as unpaid active orders.

When placing a kitchen order:

1. Select items from the kitchen menu.
2. Confirm the order.
3. Choose an available seat.
4. Receive an order ID.
5. Pay later using the order ID.

The seat is released after payment or when the order expires.

### Confectionery Menu

Confectionery orders require immediate payment. They do not require seat allocation.

## Admin Access

To access the administrator menu, enter:

The admin menu currently allows viewing the completed order logs.


**Project Structure**

**File	Description**
main.cpp	Main application workflow
ordering.cpp / ordering.h	Shopping cart and order operations
menu.c / menuorder.h	Kitchen and confectionery menu data
confirmedorders.c / confirmedorders.h	Active order storage, payment, and expiration
kitchen_queue.c / kitchen_queue.h	Kitchen order queue management
seating.cpp / seating.h	Table and seat management
seating_api.h	C-compatible interface for seating functions
logs.c / logs.h	Completed and expired order logging
active_orders.txt	Persistent active order data
logs.txt	Paid order records
defaulters.txt	Expired unpaid order records

**Requirements**
GCC
G++
C11-compatible compiler
C++17-compatible compiler
Linux, macOS, or Windows with a suitable terminal environment


**Order Lifecycle**

Create Order
     |
     v
Kitchen Order + Seat Assigned
     |
     v
Active Unpaid Order
     |
     +--> Payment Successful --> Order Added to Logs + Seat Released
     |
     +--> Two Hours Elapse ----> Order Added to Defaulters + Seat Released


**Notes**
Kitchen orders require a valid available seat.
Confectionery orders are paid immediately.
Active orders are saved between program runs.
Order IDs are automatically generated.
Existing text files are used for persistence and logging.

**Future Improvements**
Graphical user interface
Database-based storage
Online payment integration
User authentication
Kitchen staff dashboard
Order status tracking
Receipt generation
More advanced table and capacity management

**License**
This project is available for educational and personal use.
