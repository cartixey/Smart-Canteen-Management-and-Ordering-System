#ifndef MENU_H
#define MENU_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MenuItem {
    const char *category;
    const char *name;
    float price;
} MenuItem;

extern MenuItem kitchenMenu[];
extern int kitchenCount;

extern MenuItem confectioneryMenu[];
extern int confectioneryCount;

void showKitchenMenu(void);
void showConfectioneryMenu(void);

#ifdef __cplusplus
}
#endif

#endif
