#ifndef CONFIRMEDORDERS_H
#define CONFIRMEDORDERS_H

#include <stddef.h>
#include <time.h>

#include "menuorder.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ConfirmedOrder
{
    char orderID[20];
    char studentID[30];

    MenuItem items[50];
    int count;
    float total;
    int seatNumber;
    time_t placedAt;

    struct ConfirmedOrder *next;

} ConfirmedOrder;

extern ConfirmedOrder *top;

int loadActiveOrders(void);
int expireUnpaidOrders(void);
int findActiveOrder(const char *orderID);
int payOrder(const char *orderID);
int completePaidOrder(const char *studentID,
                      MenuItem items[],
                      int count,
                      float total,
                      int seatNumber,
                      char *orderID,
                      size_t orderIDSize);

int pushOrder(const char *studentID,
              MenuItem cart[],
              int count,
              float total,
              int seatNumber,
              char *orderID,
              size_t orderIDSize);

void displayConfirmedOrders(void);

#ifdef __cplusplus
}
#endif

#endif