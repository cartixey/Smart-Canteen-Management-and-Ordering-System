#ifndef LOGS_H
#define LOGS_H

#include "confirmedorders.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Save an order to log file */
int saveOrderLog(const ConfirmedOrder *order);

/* Save an expired unpaid order to the defaulter log */
int saveDefaulterOrderLog(const ConfirmedOrder *order);

/* Display all logged orders */
void displayOrderLogs(void);

/* Clear log file */
int clearOrderLogs(void);

#ifdef __cplusplus
}
#endif

#endif