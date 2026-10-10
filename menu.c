#include <stdio.h>

#include "menuorder.h"

MenuItem kitchenMenu[] = {
    {"Breakfast (South Indian)", "Idli", 30.0f},
    {"Breakfast (South Indian)", "Vada", 25.0f},
    {"Breakfast (South Indian)", "Masala Dosa", 60.0f},
    {"Breakfast (North Indian)", "Aloo Paratha", 40.0f},
    {"Breakfast (North Indian)", "Poori Bhaji", 50.0f},
    {"Lunch", "Thali (Rice, Dal, Curry, Chapatis)", 120.0f},
    {"Snacks", "Samosa", 20.0f},
    {"Snacks", "Maggi Noodles", 40.0f},
    {"Beverages", "Filter Coffee", 25.0f},
    {"Beverages", "Hot Tea", 20.0f}
};

int kitchenCount = sizeof kitchenMenu / sizeof kitchenMenu[0];

MenuItem confectioneryMenu[] = {
    {"Cakes & Pastries", "Chocolate Truffle Cake", 250.0f},
    {"Cakes & Pastries", "Red Velvet Cake", 280.0f},
    {"Cookies & Biscuits", "Chocolate Chip Cookies", 150.0f},
    {"Cookies & Biscuits", "Butter Cookies", 120.0f},
    {"Chocolates & Sweets", "Brownies", 100.0f},
    {"Chocolates & Sweets", "Motichoor Laddoo", 80.0f},
    {"Ice Creams", "Vanilla Scoop", 60.0f},
    {"Ice Creams", "Sundae", 120.0f},
    {"Specials", "Tiramisu", 300.0f},
    {"Beverages", "Cold Coffee", 90.0f},
    {"Beverages", "Milkshake", 120.0f}
};

int confectioneryCount = sizeof confectioneryMenu / sizeof confectioneryMenu[0];

void showKitchenMenu()
{
    printf("\n\t\tKitchen Menu\n");
    for (int i = 0; i < kitchenCount; i++) {
        printf("%d. %s - %s : Rs %.2f\n", i + 1,
               kitchenMenu[i].category, kitchenMenu[i].name,
               kitchenMenu[i].price);
    }
}

void showConfectioneryMenu() {
    printf("\n\t\tConfectionery Menu\n");
    for (int i = 0; i < confectioneryCount; i++) {
        printf("%d. %s - %s : Rs %.2f\n", i + 1,
               confectioneryMenu[i].category, confectioneryMenu[i].name,
               confectioneryMenu[i].price);
    }
}
