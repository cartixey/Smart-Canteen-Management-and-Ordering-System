// menu.h
#ifndef MENU_H
#define MENU_H

#ifdef __cplusplus
extern "C" {
#endif

struct MenuItem {
    const char* category;
    const char* name;
    float price;
};

extern struct MenuItem kitchenMenu[];
extern int kitchenCount;

extern struct MenuItem confectioneryMenu[];
extern int confectioneryCount;

extern void showKitchenMenu();
extern void showConfectioneryMenu();

#ifdef __cplusplus
}
#endif

#endif

