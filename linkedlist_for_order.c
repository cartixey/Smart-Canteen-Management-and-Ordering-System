struct Item {
    char name[100];
    float price;
    struct Item* next;
};

struct Order {
    int orderId;
    int itemCount;
    float total;
    struct Item* items;   // head of linked list
    struct Order* next;   // for queue of orders (kitchen)
};
