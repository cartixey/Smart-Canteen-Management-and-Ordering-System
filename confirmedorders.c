#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "confirmedorders.h"
#include "kitchen_queue.h"
#include "logs.h"
#include "seating_api.h"

#define ACTIVE_ORDERS_FILE "active_orders.txt"
#define ACTIVE_ORDERS_TEMP_FILE "active_orders.tmp"
#define MAX_ACTIVE_ORDERS 50
#define ORDER_LIFETIME_SECONDS (2 * 60 * 60)

ConfirmedOrder *top = NULL;

static unsigned long long nextOrderNumber = 1;

static int findMenuItemIndex(const MenuItem *item)
{
    for (int i = 0; i < kitchenCount; ++i)
    {
        if (item->category == kitchenMenu[i].category &&
            item->name == kitchenMenu[i].name &&
            item->price == kitchenMenu[i].price)
        {
            return i;
        }
    }

    for (int i = 0; i < confectioneryCount; ++i)
    {
        if (item->category == confectioneryMenu[i].category &&
            item->name == confectioneryMenu[i].name &&
            item->price == confectioneryMenu[i].price)
        {
            return kitchenCount + i;
        }
    }

    return -1;
}

static int getMenuItem(int index, MenuItem *item)
{
    if (item == NULL || index < 0 ||
        index >= kitchenCount + confectioneryCount)
    {
        return 0;
    }

    if (index < kitchenCount)
    {
        *item = kitchenMenu[index];
    }
    else
    {
        *item = confectioneryMenu[index - kitchenCount];
    }

    return 1;
}

static int writeActiveOrders(const ConfirmedOrder *omit)
{
    const ConfirmedOrder *orders[MAX_ACTIVE_ORDERS];
    int count = 0;

    for (const ConfirmedOrder *order = top;
         order != NULL;
         order = order->next)
    {
        if (order == omit)
        {
            continue;
        }
        if (count == MAX_ACTIVE_ORDERS)
        {
            fprintf(stderr, "Cannot save more than %d active orders.\n",
                    MAX_ACTIVE_ORDERS);
            return 0;
        }
        orders[count++] = order;
    }

    FILE *fp = fopen(ACTIVE_ORDERS_TEMP_FILE, "w");
    if (fp == NULL)
    {
        perror(ACTIVE_ORDERS_TEMP_FILE);
        return 0;
    }

    int ok = fprintf(fp, "CANTEEN_ORDERS_V2 %llu\n", nextOrderNumber) >= 0;

    /* Save oldest first so the kitchen queue is restored in FIFO order. */
    for (int i = count - 1; ok && i >= 0; --i)
    {
        const ConfirmedOrder *order = orders[i];
        if (fprintf(fp, "%s %s %lld %d %d %.9g",
                    order->orderID,
                    order->studentID,
                    (long long)order->placedAt,
                    order->seatNumber,
                    order->count,
                    (double)order->total) < 0)
        {
            ok = 0;
            break;
        }

        for (int item = 0; item < order->count; ++item)
        {
            int itemIndex = findMenuItemIndex(&order->items[item]);
            if (itemIndex < 0 || fprintf(fp, " %d", itemIndex) < 0)
            {
                ok = 0;
                break;
            }
        }

        if (ok && fputc('\n', fp) == EOF)
        {
            ok = 0;
        }
    }

    if (fclose(fp) != 0)
    {
        perror(ACTIVE_ORDERS_TEMP_FILE);
        ok = 0;
    }

    if (!ok)
    {
        fprintf(stderr, "Could not write active order data.\n");
        remove(ACTIVE_ORDERS_TEMP_FILE);
        return 0;
    }

    if (rename(ACTIVE_ORDERS_TEMP_FILE, ACTIVE_ORDERS_FILE) != 0)
    {
        perror(ACTIVE_ORDERS_FILE);
        remove(ACTIVE_ORDERS_TEMP_FILE);
        return 0;
    }

    return 1;
}

static ConfirmedOrder **findOrderLink(const char *orderID)
{
    ConfirmedOrder **link = &top;

    while (*link != NULL)
    {
        if (strcmp((*link)->orderID, orderID) == 0)
        {
            return link;
        }
        link = &(*link)->next;
    }

    return NULL;
}

static void removeOrderNode(ConfirmedOrder **link)
{
    ConfirmedOrder *order = *link;
    *link = order->next;
    free(order);
}

int loadActiveOrders(void)
{
    FILE *fp = fopen(ACTIVE_ORDERS_FILE, "r");
    if (fp == NULL)
    {
        if (errno != ENOENT)
        {
            perror(ACTIVE_ORDERS_FILE);
            return 0;
        }
        if (writeActiveOrders(NULL))
        {
            return 1;
        }
        return 0;
    }

    char magic[32];
    unsigned long long savedSequence;
    if (fscanf(fp, "%31s %llu", magic, &savedSequence) != 2 ||
        (strcmp(magic, "CANTEEN_ORDERS_V1") != 0 &&
         strcmp(magic, "CANTEEN_ORDERS_V2") != 0) ||
        savedSequence == 0)
    {
        fprintf(stderr, "Active order file has an invalid header.\n");
        fclose(fp);
        return 0;
    }

    nextOrderNumber = savedSequence;
    char orderID[20];
    char studentID[30];
    long long placedAt;
    int count;
    float total;
    int seatNumber;
    int hasSeat = strcmp(magic, "CANTEEN_ORDERS_V2") == 0;
    int parsed;
    int loaded = 0;

    while ((parsed = hasSeat
                ? fscanf(fp, "%19s %29s %lld %d %d %f",
                         orderID, studentID, &placedAt, &seatNumber,
                         &count, &total)
                : fscanf(fp, "%19s %29s %lld %d %f",
                         orderID, studentID, &placedAt, &count, &total)) ==
           (hasSeat ? 6 : 5))
    {
        if (loaded == MAX_ACTIVE_ORDERS ||
            strncmp(orderID, "ORD", 3) != 0 ||
            (hasSeat && (seatNumber < 0 || seatNumber > 6)) ||
            count < 1 || count > 50 || !isfinite(total) || total < 0.0f)
        {
            fprintf(stderr, "Active order file contains invalid order data.\n");
            fclose(fp);
            return 0;
        }

        ConfirmedOrder *order = malloc(sizeof(*order));
        if (order == NULL)
        {
            perror("malloc");
            fclose(fp);
            return 0;
        }

        strcpy(order->orderID, orderID);
        strcpy(order->studentID, studentID);
        order->placedAt = (time_t)placedAt;
        order->seatNumber = hasSeat ? seatNumber : 0;
        order->count = count;
        order->total = total;

        int valid = 1;
        for (int i = 0; i < count; ++i)
        {
            int itemIndex;
            if (fscanf(fp, "%d", &itemIndex) != 1 ||
                !getMenuItem(itemIndex, &order->items[i]))
            {
                valid = 0;
                break;
            }
        }

        if (!valid)
        {
            free(order);
            fprintf(stderr, "Active order file contains invalid menu data.\n");
            fclose(fp);
            return 0;
        }

        if (!enqueueKitchenOrder(order->orderID, order->studentID,
                                 order->items, order->count, order->total))
        {
            free(order);
            fprintf(stderr, "Could not restore the kitchen queue.\n");
            fclose(fp);
            return 0;
        }

        if (order->seatNumber > 0 && !reserveTable(order->seatNumber))
        {
            free(order);
            fprintf(stderr, "Could not restore the reserved table.\n");
            fclose(fp);
            return 0;
        }

        order->next = top;
        top = order;
        ++loaded;
    }

    if (parsed != EOF || ferror(fp))
    {
        fprintf(stderr, "Active order file could not be read completely.\n");
        fclose(fp);
        return 0;
    }

    if (fclose(fp) != 0)
    {
        perror(ACTIVE_ORDERS_FILE);
        return 0;
    }

    return 1;
}

int expireUnpaidOrders(void)
{
    time_t now = time(NULL);
    if (now == (time_t)-1)
    {
        fprintf(stderr, "Could not read the current time.\n");
        return -1;
    }

    int expired = 0;
    ConfirmedOrder **link = &top;
    while (*link != NULL)
    {
        ConfirmedOrder *order = *link;
        if (now < order->placedAt ||
            difftime(now, order->placedAt) < ORDER_LIFETIME_SECONDS)
        {
            link = &order->next;
            continue;
        }

        if (!writeActiveOrders(order))
        {
            return -1;
        }

        if (!removeKitchenOrder(order->orderID))
        {
            fprintf(stderr, "Kitchen queue is missing order %s.\n",
                    order->orderID);
        }
        releaseTable(order->seatNumber);
        if (!saveDefaulterOrderLog(order))
        {
            fprintf(stderr, "Expired order was removed, but its defaulter log failed.\n");
        }
        removeOrderNode(link);
        ++expired;
    }

    return expired;
}

int findActiveOrder(const char *orderID)
{
    return orderID != NULL && findOrderLink(orderID) != NULL;
}

int payOrder(const char *orderID)
{
    if (orderID == NULL)
    {
        return 0;
    }

    ConfirmedOrder **link = findOrderLink(orderID);
    if (link == NULL)
    {
        return 0;
    }

    ConfirmedOrder *order = *link;
    if (!writeActiveOrders(order))
    {
        return 0;
    }

    if (!saveOrderLog(order))
    {
        fprintf(stderr, "Order was paid, but its finished-order log failed.\n");
    }

    if (!removeKitchenOrder(order->orderID))
    {
        fprintf(stderr, "Kitchen queue is missing order %s.\n", order->orderID);
    }
    releaseTable(order->seatNumber);

    removeOrderNode(link);
    return 1;
}

int completePaidOrder(const char *studentID,
                      MenuItem items[],
                      int count,
                      float total,
                      int seatNumber,
                      char *orderID,
                      size_t orderIDSize)
{
    if (studentID == NULL || items == NULL || orderID == NULL ||
        seatNumber != 0 ||
        orderIDSize < sizeof(((ConfirmedOrder *)0)->orderID) ||
        strlen(studentID) >= sizeof(((ConfirmedOrder *)0)->studentID) ||
        count < 1 || count > 50 || !isfinite(total) || total < 0.0f ||
        nextOrderNumber == 0 || nextOrderNumber == ULLONG_MAX)
    {
        fprintf(stderr, "Invalid paid order details.\n");
        return 0;
    }

    ConfirmedOrder order = {0};
    int idLength = snprintf(order.orderID, sizeof(order.orderID),
                            "ORD%08llu", nextOrderNumber);
    if (idLength < 0 || (size_t)idLength >= sizeof(order.orderID))
    {
        fprintf(stderr, "Could not generate an order ID.\n");
        return 0;
    }

    strcpy(order.studentID, studentID);
    order.count = count;
    order.total = total;
    order.seatNumber = 0;
    order.placedAt = time(NULL);
    if (order.placedAt == (time_t)-1)
    {
        fprintf(stderr, "Could not record the order time.\n");
        return 0;
    }

    for (int i = 0; i < count; ++i)
    {
        int menuIndex = findMenuItemIndex(&items[i]);
        if (menuIndex < kitchenCount ||
            menuIndex >= kitchenCount + confectioneryCount)
        {
            fprintf(stderr, "Immediate-payment orders must contain confectionery items.\n");
            return 0;
        }
        order.items[i] = items[i];
    }

    ++nextOrderNumber;
    if (!writeActiveOrders(NULL))
    {
        --nextOrderNumber;
        return 0;
    }

    if (!saveOrderLog(&order))
    {
        fprintf(stderr, "Payment was accepted, but the completed order could not be logged.\n");
        return 0;
    }

    strcpy(orderID, order.orderID);
    return 1;
}

int pushOrder(const char *studentID,
              MenuItem cart[],
              int count,
              float total,
              int seatNumber,
              char *orderID,
              size_t orderIDSize)
{
    if (studentID == NULL || cart == NULL || orderID == NULL ||
        seatNumber < 1 || seatNumber > 6 ||
        orderIDSize < sizeof(((ConfirmedOrder *)0)->orderID) ||
        strlen(studentID) >= sizeof(((ConfirmedOrder *)0)->studentID) ||
        count < 1 || count > 50 || !isfinite(total) || total < 0.0f ||
        nextOrderNumber == 0 || nextOrderNumber == ULLONG_MAX ||
        getKitchenQueueSize() >= MAX_ACTIVE_ORDERS)
    {
        fprintf(stderr, "Invalid order details or the active order limit was reached.\n");
        return 0;
    }

    ConfirmedOrder *newOrder = malloc(sizeof(*newOrder));
    if (newOrder == NULL)
    {
        perror("malloc");
        return 0;
    }

    int idLength = snprintf(newOrder->orderID, sizeof(newOrder->orderID),
                            "ORD%08llu", nextOrderNumber);
    if (idLength < 0 || (size_t)idLength >= sizeof(newOrder->orderID))
    {
        free(newOrder);
        fprintf(stderr, "Could not generate an order ID.\n");
        return 0;
    }

    strcpy(newOrder->studentID, studentID);
    newOrder->count = count;
    newOrder->total = total;
    newOrder->seatNumber = seatNumber;
    newOrder->placedAt = time(NULL);
    if (newOrder->placedAt == (time_t)-1)
    {
        free(newOrder);
        fprintf(stderr, "Could not record the order time.\n");
        return 0;
    }

    for (int i = 0; i < count; ++i)
    {
        if (findMenuItemIndex(&cart[i]) < 0)
        {
            free(newOrder);
            fprintf(stderr, "Order contains an item that is not on the menu.\n");
            return 0;
        }
        newOrder->items[i] = cart[i];
    }

    if (!enqueueKitchenOrder(newOrder->orderID, newOrder->studentID,
                             newOrder->items, newOrder->count, newOrder->total))
    {
        free(newOrder);
        fprintf(stderr, "Could not add the order to the kitchen queue.\n");
        return 0;
    }

    newOrder->next = top;
    top = newOrder;
    ++nextOrderNumber;

    if (!writeActiveOrders(NULL))
    {
        top = newOrder->next;
        removeKitchenOrder(newOrder->orderID);
        free(newOrder);
        --nextOrderNumber;
        return 0;
    }

    strcpy(orderID, newOrder->orderID);
    return 1;
}

void displayConfirmedOrders(void)
{
    if (top == NULL)
    {
        printf("\nNo active unpaid orders.\n");
        return;
    }

    printf("\n===== ACTIVE UNPAID KITCHEN ORDERS =====\n");
    for (ConfirmedOrder *order = top; order != NULL; order = order->next)
    {
        printf("\nOrder ID   : %s\n", order->orderID);
        printf("Student ID : %s\n", order->studentID);
        if (order->seatNumber > 0)
        {
            printf("Seat       : %d\n", order->seatNumber);
        }
        for (int i = 0; i < order->count; ++i)
        {
            printf("%s : Rs %.2f\n",
                   order->items[i].name,
                   order->items[i].price);
        }
        printf("Total Bill : Rs %.2f\n", order->total);
    }
}
