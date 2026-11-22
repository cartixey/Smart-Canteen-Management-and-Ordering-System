#include <string.h>

#include "kitchen_queue.h"

#define KITCHEN_QUEUE_CAPACITY 50

static KitchenQueue queue[KITCHEN_QUEUE_CAPACITY];
static int front = 0;
static int rear = 0;
static int size = 0;

int enqueueKitchenOrder(const char *orderID,
                        const char *studentID,
                        MenuItem items[],
                        int count,
                        float total)
{
    if (orderID == NULL || studentID == NULL || items == NULL ||
        strlen(orderID) >= sizeof(queue[0].orderID) ||
        strlen(studentID) >= sizeof(queue[0].studentID) ||
        count < 1 || count > 50 || total < 0.0f ||
        size == KITCHEN_QUEUE_CAPACITY)
    {
        return 0;
    }

    KitchenQueue *order = &queue[rear];
    strcpy(order->orderID, orderID);
    strcpy(order->studentID, studentID);
    order->count = count;
    order->total = total;

    for (int i = 0; i < count; ++i)
    {
        order->items[i] = items[i];
    }

    rear = (rear + 1) % KITCHEN_QUEUE_CAPACITY;
    ++size;
    return 1;
}

int dequeueKitchenOrder(KitchenQueue *order)
{
    if (order == NULL || size == 0)
    {
        return 0;
    }

    *order = queue[front];
    front = (front + 1) % KITCHEN_QUEUE_CAPACITY;
    --size;
    return 1;
}

int removeKitchenOrder(const char *orderID)
{
    if (orderID == NULL)
    {
        return 0;
    }

    int offset = 0;
    while (offset < size)
    {
        int index = (front + offset) % KITCHEN_QUEUE_CAPACITY;
        if (strcmp(queue[index].orderID, orderID) == 0)
        {
            for (int move = offset; move < size - 1; ++move)
            {
                int current = (front + move) % KITCHEN_QUEUE_CAPACITY;
                int next = (front + move + 1) % KITCHEN_QUEUE_CAPACITY;
                queue[current] = queue[next];
            }

            rear = (rear - 1 + KITCHEN_QUEUE_CAPACITY) %
                   KITCHEN_QUEUE_CAPACITY;
            --size;
            return 1;
        }
        ++offset;
    }

    return 0;
}

int getKitchenQueueSize(void)
{
    return size;
}
