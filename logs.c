#include <stdio.h>
#include <time.h>

#include "logs.h"

static int appendOrderLog(const char *fileName,
                          const char *status,
                          const ConfirmedOrder *order)
{
    if (order == NULL)
    {
        return 0;
    }

    FILE *fp = fopen(fileName, "a");
    if (fp == NULL)
    {
        perror(fileName);
        return 0;
    }

    time_t now = time(NULL);
    struct tm *currentTime = localtime(&now);
    char timestamp[30];

    if (currentTime == NULL ||
        strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S",
                 currentTime) == 0)
    {
        fclose(fp);
        fprintf(stderr, "Could not format the order log timestamp.\n");
        return 0;
    }

    fprintf(fp, "\n================================");
    fprintf(fp, "\nStatus      : %s", status);
    fprintf(fp, "\nDate & Time : %s", timestamp);
    fprintf(fp, "\nOrder ID    : %s", order->orderID);
    fprintf(fp, "\nStudent ID  : %s", order->studentID);
    if (order->seatNumber > 0)
    {
        fprintf(fp, "\nSeat        : %d", order->seatNumber);
    }
    fprintf(fp, "\nTotal Bill  : %.2f", order->total);
    fprintf(fp, "\nItems:");

    for (int i = 0; i < order->count; ++i)
    {
        fprintf(fp, "\n%s : Rs %.2f",
                order->items[i].name,
                order->items[i].price);
    }

    fprintf(fp, "\n================================\n");

    if (fclose(fp) != 0)
    {
        perror(fileName);
        return 0;
    }

    return 1;
}

int saveOrderLog(const ConfirmedOrder *order)
{
    return appendOrderLog("logs.txt", "Paid", order);
}

int saveDefaulterOrderLog(const ConfirmedOrder *order)
{
    return appendOrderLog("defaulters.txt", "Expired unpaid", order);
}

void displayOrderLogs(void)
{
    FILE *fp = fopen("logs.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo logs available or error opening the log file.\n");
        return;
    }

    int ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        putchar(ch);
    }

    if (ferror(fp))
    {
        perror("logs.txt");
    }

    fclose(fp);
}

int clearOrderLogs(void)
{
    FILE *fp = fopen("logs.txt", "w");

    if (fp == NULL)
    {
        perror("logs.txt");
        return 0;
    }

    if (fclose(fp) != 0)
    {
        perror("logs.txt");
        return 0;
    }

    return 1;
}
