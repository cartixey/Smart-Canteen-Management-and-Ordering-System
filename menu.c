#include <stdio.h>

// Define a structure for menu items with price
struct Menu {
    char category[50];
    char item[100];
    float price;
};

// Kitchen Menu
void kmenu() {
    struct Menu kitchen[] = {
        {"Breakfast (South Indian)", "Idli", 30.0},
        {"Breakfast (South Indian)", "Vada", 25.0},
        {"Breakfast (South Indian)", "Masala Dosa", 60.0},
        {"Breakfast (North Indian)", "Aloo Paratha", 40.0},
        {"Breakfast (North Indian)", "Poori Bhaji", 50.0},
        {"Lunch", "Thali (Rice, Dal, Curry, Chapatis)", 120.0},
        {"Snacks", "Samosa", 20.0},
        {"Snacks", "Maggi Noodles", 40.0},
        {"Beverages", "Filter Coffee", 25.0},
        {"Beverages", "Hot Tea", 20.0}
    };

    int n = sizeof(kitchen)/sizeof(kitchen[0]);
    printf("\n\t\tKitchen Menu\n");
    for(int i=0; i<n; i++) {
        printf("%s - %s : Rs %.2f\n", kitchen[i].category, kitchen[i].item, kitchen[i].price);
    }
}

// Confectionery Menu
void cmenu() {
    struct Menu confectionery[] = {
        {"Cakes & Pastries", "Chocolate Truffle Cake", 250.0},
        {"Cakes & Pastries", "Red Velvet Cake", 280.0},
        {"Cookies & Biscuits", "Chocolate Chip Cookies", 150.0},
        {"Cookies & Biscuits", "Butter Cookies", 120.0},
        {"Chocolates & Sweets", "Brownies", 100.0},
        {"Chocolates & Sweets", "Motichoor Laddoo", 80.0},
        {"Ice Creams", "Vanilla Scoop", 60.0},
        {"Ice Creams", "Sundae", 120.0},
        {"Specials", "Tiramisu", 300.0},
        {"Beverages", "Cold Coffee", 90.0},
        {"Beverages", "Milkshake", 120.0}
    };

    int n = sizeof(confectionery)/sizeof(confectionery[0]);
    printf("\n\t\tConfectionery Menu\n");
    for(int i=0; i<n; i++) {
        printf("%s - %s : Rs %.2f\n", confectionery[i].category, confectionery[i].item, confectionery[i].price);
    }
}

int menu() {
    int choice;
    printf("Select 1 for kitchen menu and 2 for confectionery menu\n");
    scanf("%d", &choice);

    switch(choice) {
        case 1: kmenu(); break;
        case 2: cmenu(); break;
        default: printf("ERROR\n");
    }
    return 0;
}

int main() {
    menu();
    return 0;
}
