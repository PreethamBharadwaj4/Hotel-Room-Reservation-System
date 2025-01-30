#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME "users.txt"

typedef struct {
    char username[50];
    char password[50];
} User;

typedef struct {
    char itemName[50];
    int price;
    int quantity;
} CartItem;

User *users = NULL;
int userCount = 0;
int userCapacity = 0;
CartItem cart[100];  
int cartItemCount = 0;
char loggedInUser[50];

void loadUsersFromFile() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        return;
    }
    userCount = 0;
    userCapacity = 10;
    users = (User *)malloc(userCapacity * sizeof(User));
    while (fscanf(file, "%s %s", users[userCount].username, users[userCount].password) != EOF) {
        userCount++;
        if (userCount >= userCapacity) {
            userCapacity *= 2;
            users = (User *)realloc(users, userCapacity * sizeof(User));
        }
    }
    fclose(file);
}

void saveUsersToFile() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Error: Could not open file to save users.\n");
        return;
    }
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }
    fclose(file);
}

void saveOrderToFile(float roomTotal, float foodTotal, float totalBill) {
    char filename[60];
    sprintf(filename, "%s_bill.txt", loggedInUser);
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not save order details.\n");
        return;
    }

    fprintf(file, "--- Final Bill ---\n");
    fprintf(file, "User: %s\n\n", loggedInUser);

    fprintf(file, "--- Food Order ---\n");
    if (cartItemCount == 0) {
        fprintf(file, "No food items ordered.\n");
    } else {
        fprintf(file, "Item Name\tPrice\tQuantity\tTotal\n");
        for (int i = 0; i < cartItemCount; i++) {
            fprintf(file, "%s\t\t%d\t%d\t\t%d\n", cart[i].itemName, cart[i].price, cart[i].quantity, cart[i].price * cart[i].quantity);
        }
    }
    fprintf(file, "\nRoom charges (excluding GST): %.2f rs\n", roomTotal);
    fprintf(file, "Food charges (excluding GST): %.2f rs\n", foodTotal);
    fprintf(file, "GST on Room charges: %.2f rs\n", roomTotal * 0.12);
    fprintf(file, "GST on Food charges: %.2f rs\n", foodTotal * 0.12);
    fprintf(file, "\nTotal Bill: %.2f rs\n", totalBill);

    fclose(file);
}

void registerUser() {
    printf("\n--- Register ---\n");
    if (userCount >= userCapacity) {
        userCapacity = (userCapacity == 0) ? 10 : userCapacity * 2;
        users = (User *)realloc(users, userCapacity * sizeof(User));
        if (!users) {
            printf("Error: Memory allocation failed.\n");
            return;
        }
    }
    printf("Enter username: ");
    scanf("%s", users[userCount].username);
    printf("Enter password: ");
    scanf("%s", users[userCount].password);
    userCount++;
    printf("User registered successfully!\n");
    saveUsersToFile();
}

int loginUser() {
    char username[50], password[50];
    printf("\n------Login--------\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            printf("Login successful! Welcome to PPR Hotel %s.\n", username);
            strcpy(loggedInUser, username);
            return 1;
        }
    }

    printf("Invalid username or password. Would you like to register? (1 for Yes, 0 for No): ");
    int choice;
    scanf("%d", &choice);
    if (choice == 1) {
        registerUser();
        return 1;
    }

    return 0;
}

void addToCart(char *itemName, int price, int quantity) {
    int found = 0;
    for (int i = 0; i < cartItemCount; i++) {
        if (strcmp(cart[i].itemName, itemName) == 0) {
            cart[i].quantity += quantity;
            found = 1;
            break;
        }
    }
    if (!found) {
        strcpy(cart[cartItemCount].itemName, itemName);
        cart[cartItemCount].price = price;
        cart[cartItemCount].quantity = quantity;
        cartItemCount++;
    }
}

void viewCart(float *foodTotal) {
    *foodTotal = 0;
    printf("\n--- Your Cart ---\n");
    if (cartItemCount == 0) {
        printf("Your cart is empty.\n");
        return;
    }
    printf("Item Name\tPrice\tQuantity\tTotal\n");
    for (int i = 0; i < cartItemCount; i++) {
        int itemTotal = cart[i].price * cart[i].quantity;
        printf("%s\t\t%d\t%d\t\t%d\n", cart[i].itemName, cart[i].price, cart[i].quantity, itemTotal);
        *foodTotal += itemTotal;
    }
    printf("Cart Total: %.2f rs\n", *foodTotal);
}

void foodMenu(float *foodTotal) {
    printf("\n--- Welcome to PPR Veg Food Menu ---\n");
    int response, category, choice, quantity, item_cost;
    char itemName[50];

    do {
        printf("\nSelect a category:\n");
        printf("1. Snacks\n2. South Indian Breakfast\n3. South Indian Meals\n");
        printf("4. Beverages\n5. Desserts\n6. Side Dishes\n");
        printf("Enter your choice: ");
        scanf("%d", &category);
        item_cost = 0;
        switch (category) {
            case 1:
                printf("1. Samosa - 20rs\n2. Pakora - 30rs\n3. Bhel Puri - 40rs\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1) { strcpy(itemName, "Samosa"); item_cost = 20; }
                else if (choice == 2) { strcpy(itemName, "Pakoda"); item_cost = 30; }
                else if (choice == 3) { strcpy(itemName, "Bhel Puri"); item_cost = 40; }
                else { printf("Invalid choice.\n"); continue; }
                break;
            case 2:
                printf("1. Idli - 40rs\n2. Dosa - 50rs\n3. Vada - 30rs\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1) { strcpy(itemName, "Idli"); item_cost = 40; }
                else if (choice == 2) { strcpy(itemName, "Dosa"); item_cost = 50; }
                else if (choice == 3) { strcpy(itemName, "Vada"); item_cost = 30; }
                else { printf("Invalid choice.\n"); continue; }
                break;
            case 3:
                printf("1. Full Meals - 80rs\n2. Half Meals - 60rs\n3. Biryani - 75rs\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1){strcpy(itemName, "Full meals"); item_cost = 80;}
                else if (choice == 2){strcpy(itemName, "Half meals"); item_cost = 60;}
                else if (choice == 3){strcpy(itemName, "Biryani"); item_cost = 75;}
                else { printf("Invalid choice.\n"); continue; }
                break;
            case 4:
                printf("1. Filter Coffee - 20rs\n2. Masala Tea - 15rs\n3. Lemon tea - 20rs\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1){strcpy(itemName, "Filter coffee"); item_cost = 20;}
                else if (choice == 2){strcpy(itemName, "Masala tea"); item_cost = 15;}
                else if (choice == 3){strcpy(itemName, "Lemaon tea"); item_cost = 20;}
                else { printf("Invalid choice.\n"); continue; }     
                break;
             case 5:
                printf("1. Gulab Jamun - 20rs\n2. Rasgulla - 25rs\n3. Payasam - 30rs\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1){strcpy(itemName, "Gulab jamun"); item_cost = 20;}
                else if (choice == 2){strcpy(itemName, "Rasgulla"); item_cost = 25;}
                else if (choice == 3){strcpy(itemName, "Payasam"); item_cost = 30;}
                else { printf("Invalid choice.\n"); continue; }
                break;
            case 6:
                printf("1. Papad - 10rs\n2. Pickle - 5rs\n3. Chutney - 15rs\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if (choice == 1){strcpy(itemName, "Papad"); item_cost = 10;}
                else if (choice == 2){strcpy(itemName, "Pickle"); item_cost = 5;}
                else if (choice == 3){strcpy(itemName, "Chutney"); item_cost = 15;}
                else { printf("Invalid choice.\n"); continue; }        
                break;
            default:
                printf("Invalid category.\n");
                continue;
        }

        printf("Enter quantity: ");
        scanf("%d", &quantity);

        addToCart(itemName, item_cost, quantity);

        printf("Do you want to add more items? (1 for Yes, 0 for No): ");
        scanf("%d", &response);
    } while (response == 1);

    viewCart(foodTotal);
}

void roomBooking(float *roomTotal) {
    printf("\n--- PPR Room Booking ---\n");
    char name[20];
    int room_type, bedno, days, bill;
    printf("Enter the name of customer.\n");
    scanf("%s", name);
    printf("1. NON-AC rooms.\n");
    printf("2. AC rooms.\n");
    printf("3. SUITE rooms.\n");
    printf("4. DELUXE rooms.\n");
    printf("Enter your choice: ");
    scanf("%d", &room_type);
    switch (room_type) {
        case 1:
            printf("Enter number of persons (max 3 beds): ");
            scanf("%d", &bedno);
            bill = 350 + (bedno - 1) * 200;
            break;
        case 2:
            printf("Enter number of persons (max 3 beds): ");
            scanf("%d", &bedno);
            bill = 500 + (bedno - 1) * 250;
            break;
        case 3:
            printf("Enter number of persons (max 3 beds): ");
            scanf("%d", &bedno);
            bill = 1500 + (bedno - 1) * 750;
            break;
        case 4:
            printf("Enter number of rooms (max 3 rooms): ");
            scanf("%d", &bedno);
            bill = 2500 + (bedno - 1) * 1250;
            break;
        default:
            printf("Invalid input.\n");
            return;
    }
    printf("Enter the number of days: ");
    scanf("%d", &days);
    bill *= days;
    *roomTotal = bill;
}

void mainMenu() {
    int choice, isLoggedIn = 0;
    float roomTotal = 0, foodTotal = 0, totalBill;

    isLoggedIn = loginUser();
    if (!isLoggedIn) return;

    roomBooking(&roomTotal);
    foodMenu(&foodTotal);

    totalBill = roomTotal + foodTotal + (roomTotal * 0.12) + (foodTotal * 0.12);
    printf("\n--- Final Bill ---\n");
    printf("Room charges (excluding GST): %.2f rs\n", roomTotal);
    printf("Food charges (excluding GST): %.2f rs\n", foodTotal);
    printf("GST on Room charges: %.2f rs\n", roomTotal * 0.12);
    printf("GST on Food charges: %.2f rs\n", foodTotal * 0.12);
    printf("\nTotal Bill: %.2f rs\n", totalBill);

    saveOrderToFile(roomTotal, foodTotal, totalBill);
}

int main() {
    loadUsersFromFile();
    mainMenu();
    free(users);
    return 0;
}
