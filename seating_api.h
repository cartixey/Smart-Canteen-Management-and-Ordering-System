#ifndef SEATING_API_H
#define SEATING_API_H

#ifdef __cplusplus
extern "C" {
#endif

void releaseTable(int tableNumber);
int reserveTable(int tableNumber);
void showSeatingArrangement(void);

#ifdef __cplusplus
}
#endif

#endif
