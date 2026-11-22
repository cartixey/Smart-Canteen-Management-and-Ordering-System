#ifndef KITCHEN_QUEUE_H
#define KITCHEN_QUEUE_H

#include "menuorder.h"

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct KitchenQueue
{
    char orderID[20];
    char studentID[30];
    MenuItem items[50];
    int count;
    float total;
} KitchenQueue;

int enqueueKitchenOrder(const char *orderID,
                        const char *studentID,
                        MenuItem items[],
                        int count,
                        float total);
int dequeueKitchenOrder(KitchenQueue *order);
int removeKitchenOrder(const char *orderID);
int getKitchenQueueSize(void);

#ifdef __cplusplus
}   
#endif
#endif // KITCHEN_QUEUE_H