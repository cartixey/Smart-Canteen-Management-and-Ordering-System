#include <iostream>

#include "seating.h"

Table::Table() {
    tableNumber = 0;
    capacity = 0;
    occupied = false;
}

Table::Table(int number, int seats) {
    tableNumber = number;
    capacity = seats;
    occupied = false;
}

void Table::displayInfo() const {

    std::cout << "Table " << tableNumber
              << " | " << capacity << " seats | ";

    if (occupied) {
        std::cout << "Occupied\n";
    } else {
        std::cout << "Available\n";
    }
}

bool Table::reserveTable() {

    if (occupied) {
        return false;
    }

    occupied = true;
    return true;
}

void Table::releaseTable() {
    occupied = false;
}

int Table::getTableNumber() const {
    return tableNumber;
}

bool Table::isOccupied() const {
    return occupied;
}




Table tables[] = {

    Table(1, 2),
    Table(2, 2),
    Table(3, 4),
    Table(4, 4),
    Table(5, 6),
    Table(6, 6)

};

int tableCount = sizeof(tables) / sizeof(tables[0]);


void showSeatingArrangement() {

    std::cout << "\n----- SEATING ARRANGEMENT -----\n";

    for (int i = 0; i < tableCount; i++) {
        tables[i].displayInfo();
    }
}


bool reserveTable(int tableNumber) {

    if (tableNumber < 1 || tableNumber > tableCount) {
        return false;
    }

    return tables[tableNumber - 1].reserveTable();
}