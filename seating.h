#ifndef SEATING_H
#define SEATING_H

class Table {

    int tableNumber;
    int capacity;
    bool occupied;

public:

    Table();
    Table(int number, int seats);

    void displayInfo() const;
    bool reserveTable();
    void releaseTable();

    int getTableNumber() const;
    bool isOccupied() const;
};

#ifdef __cplusplus
extern "C" {
#endif

void showSeatingArrangement(void);
int reserveTable(int tableNumber);
void releaseTable(int tableNumber);

#ifdef __cplusplus
}
#endif

#endif