/*

----------------------------------------COMPUTER STORE MANAGEMENT--------------------------------------------------
-------------------------------------------CODE BY ANISH JOSHI-----------------------------------------------------
------------------------------------------COPYRIGHT ANISH JOSHI----------------------------------------------------
------------------------------------------CREATED ON 18/03/2022----------------------------------------------------
-----------------------------------PASSWORD FOR ADMIN PANEL - root@1234--------------------------------------------

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PASSWORD "root@1234"
FILE *fptr;

typedef struct item {
    int item_num;
    char item_name[25];
    int item_price;
    int available_stock;
}item;

item inventory_store[200];

int customer_billing_amount = 0;
int customer_cart[200];
char customer_name[20];

void initialize_customer_cart(){
    for(int i=0; i<200; i++) {
        customer_cart[i] = 0;
    }
}

int get_total_items_in_inventory() {
    int count = 0;
    char c;
    fptr = fopen("inventory.txt", "r");
    for (c = getc(fptr); c != EOF; c = getc(fptr)) {
        if (c == '\n') {
            count = count + 1;
        }
    }
    fclose(fptr);
    return count;
}

int max_item_index = get_total_items_in_inventory();

void update_inventory_store() {
    fptr = fopen("inventory.txt","r");
    int i = 0;
    while (i < max_item_index) {
        fscanf(fptr,"%d,%d,%d,%s\n",&inventory_store[i].item_num, &inventory_store[i].item_price, &inventory_store[i].available_stock, inventory_store[i].item_name);
        i++;
    }
    fclose(fptr);
}

void update_inventory_stock() {
    fptr = fopen("inventory.txt","w");
    int i = 0;
    while (i < max_item_index) {
        fprintf(fptr, "%d,%d,%d,%s\n", inventory_store[i].item_num, inventory_store[i].item_price, inventory_store[i].available_stock, inventory_store[i].item_name);
        i++;
    }
    fclose(fptr);
}

bool check_cart_empty() {
    for(int i=0; i<max_item_index; i++) {
        if (customer_cart[i] != 0) {
            return false;
        }
    }
    return true;
}

void view_cart() {
    if (check_cart_empty()) {
        printf("\nCart is empty.\n");
    }
    else {
        printf("\nCustomer Name : %s\n\n",customer_name);
        printf("### Items in cart ###\n\n");
        int i;
        printf("ITEM_NO     ITEM_NAME         PRICE      QUANTITY      AMOUNT\n\n");
        for (i=0;i<200;i++) {
            if (customer_cart[i] > 0) {
               printf("%d%25s%10d%10d%10d\n",inventory_store[i].item_num, inventory_store[i].item_name, inventory_store[i].item_price, customer_cart[i], customer_cart[i]*inventory_store[i].item_price);
            }
        }
    }
    if (customer_billing_amount > 0) {
        printf("\n\nBilling Amount: %d\n", customer_billing_amount);
        printf("\nDiscounted Billing Amount: %d\n", (int)((customer_billing_amount*9)/10));
    }
    char op;
    printf("\nConfirm exit, press any key: ");
    scanf(" %c", &op);
    system("cls");
}

void view_all_products() {
    system("cls");
    printf("ITEM_NO     ITEM_NAME         PRICE\n\n");
    int i = 0;
    while (i < max_item_index) {
        printf("%d%25s%10d\n",inventory_store[i].item_num, inventory_store[i].item_name, inventory_store[i].item_price);
        i++;
    }
}

void add_to_cart(int quantity, int item_no) {
    customer_cart[item_no] += quantity;
    inventory_store[item_no].available_stock -= quantity;
    customer_billing_amount += inventory_store[item_no].item_price*quantity;
    printf("\nItem added to cart successfully.\n");
}

void delete_from_cart(int quantity, int item_no) {
    customer_cart[item_no] -= quantity;
    inventory_store[item_no].available_stock += quantity;
    customer_billing_amount -= inventory_store[item_no].item_price*quantity;
    printf("\nItem removed from cart successfully.\n");
}

bool check_availablity(int item_no, int quantity) {
    if (inventory_store[item_no].available_stock >= quantity) {
        return true;
    }
    else return false;
}

void buy() {
    char op;
    printf("\n\nEnter 'a' to add an item to a cart, 'd' to delete an item/item quantity or press any other key to go back to the welcome screen : ");
    scanf(" %c",&op);
    char op1='c';
    printf("\n");
    switch(op) {
        case 'a':
            do {
                printf("\nEnter 'c' to choose an item, press any other button to go back from where you left : ");
                scanf(" %c",&op1);
                if (op1 == 'c') {
                    printf("\nPlease enter the item number : ");
                    int item_no, quantity;
                    scanf("%d",&item_no);
                    if (item_no>=0 && item_no<=max_item_index) {
                        do {
                            printf("\nPlease enter quantity(1-5) : ");
                            scanf("%d",&quantity);
                            if (quantity>=1 && quantity<=5) {
                                if (!(check_availablity(item_no,quantity))) {
                                    printf("\nSorry this product is currently out of stock\n");
                                    break;
                                }
                                add_to_cart(quantity, item_no);
                                break;
                            }
                            else printf("\nOops try again, looks like you entered a bad quantity :(\n");
                        }
                        while (!(quantity>=1) || !(quantity<=5));
                    }
                    else printf("\nTry again, item number %d does not exist\n",item_no);
                }
                else buy();
            }
            while (op1 == 'c');
            break;
        case 'd':
            if (!check_cart_empty()) {
                do {
                    printf("\nEnter 'c' to choose an item, press any other button to go back from where you left : ");
                    scanf(" %c",&op1);
                    if (op1 == 'c') {
                        printf("\nPlease enter the item number : ");
                        int item_no, quantity;
                        scanf("%d",&item_no);
                        if (customer_cart[item_no] == 0) {
                            printf("\nItem not added.\n");
                        }
                        if (customer_cart[item_no] > 0) {
                            do {
                                printf("\nPlease enter quantity to be removed : ");
                                scanf("%d",&quantity);
                                if (quantity>=1 && quantity<=5 && quantity <= customer_cart[item_no]) {
                                    delete_from_cart(quantity, item_no);
                                    break;
                                }
                                else printf("\nOops try again, you haven't added that much quantity :(\n");
                            }
                            while (!(quantity>=1) || !(quantity<=5));
                        }
                        else printf("\nTry again, item number %d does not exist or is not added to cart\n",item_no);
                    }
                    else buy();
                }
                while (op1 == 'c');
            }
            else {
                printf("\nCart is Empty.\n");
                printf("\nConfirm exit, press any key: ");
                scanf(" %c", &op);
                system("cls");
            }
            break;
        default:
            system("cls");
    }
}

void welcome_screen() {
    int op = 0;
    while (op != 3) {
        printf("............................................................COMPUTER WORLD............................................................");
        printf("\n\n                                                         Welcome to the Store :)");
        printf("\n\n1)View All Products\n2)View Cart\n3)Exit Application");

        printf("\n\nWhat's on your mind today : ");
        scanf("%d",&op);
        switch(op) {
            case 1 :
                view_all_products();
                buy();
                break;
            case 2 :
                view_cart();
                break;
            default:
                if(op == 3) {
                    system("cls");
                    printf("\nPrinting final bill\n");
                    view_cart();
                    update_inventory_stock();
                    printf("\n\n---------Thank you come again!---------\n");
                    break;
                }
                welcome_screen();
        }
    }
}

void view_inventory() {
    system("cls");
    printf("ITEM_NO     ITEM_NAME         PRICE         STOCK\n\n");
    int i = 0;
    while (i < max_item_index) {
        printf("%d%25s%10d%10d\n",inventory_store[i].item_num, inventory_store[i].item_name, inventory_store[i].item_price, inventory_store[i].available_stock);
        i++;
    }
}

void add_new_product() {
    char item_name1[25];
    int item_price;
    int item_stock;
    printf("\nEnter new item name:");
    fflush(stdin);
    gets(item_name1);
    printf("\nEnter new item price:");
    scanf("%d", &item_price);
    printf("\nEnter new item stock:");
    scanf("%d", &item_stock);
    inventory_store[max_item_index].item_num = max_item_index;
    strcpy(inventory_store[max_item_index].item_name, item_name1);
    inventory_store[max_item_index].item_price = item_price;
    inventory_store[max_item_index].available_stock = item_stock;
    max_item_index += 1;
    printf("\nNew item added sucessfully\n");
}

void update_item() {
    int item_no;
    int quantity;
    int new_price;
    printf("\nEnter item number : ");
    scanf("%d", &item_no);
    printf("\nEnter stock you want to add : ");
    scanf("%d", &quantity);
    inventory_store[item_no].available_stock += quantity;
    printf("\nStock updated sucessfully\n");
    char ans;
    printf("\n\nDo you want to change price of product?\n\nEnter y for yes and any other key for no : ");
    scanf(" %c",&ans);
    if (ans=='y') {
        printf("\n\nEnter new price : ");
        scanf("%d",&new_price);
        inventory_store[item_no].item_price = new_price;
        printf("\n\nPrice updated successfully");
    }
    else return;
}

void admin_panel() {
    system("cls");
    printf("Welcome Admin\n");
    char op;
    while (op != 'e'){
        printf("\n---press 'a' to ADD new product to inventory---\n");
        printf("---press 'u' to UPDATE product details---\n");
        printf("---press 'v' to VIEW INVENTORY---\n");
        printf("---press 'e' to EXIT---\n");
        printf("\nEnter input: ");
        scanf(" %c", &op);
            switch(op) {
                case 'a':
                    add_new_product();
                    char p1;
                    printf("\nEnter any character to exit: ");
                    scanf(" %c", &p1);
                    system("cls");
                    break;
                case 'u':
                    update_item();
                    char p2;
                    printf("\nEnter any character to exit: ");
                    scanf(" %c", &p2);
                    system("cls");
                    break;
                case 'v':
                    view_inventory();
                    char p3;
                    printf("\nEnter any character to exit: ");
                    scanf(" %c", &p3);
                    system("cls");
                    break;
                case 'e':
                    update_inventory_stock();
                    printf("\n\n---------INVENTORY UPDATED, EXITING!---------\n");
                    exit(0);
                default:
                    printf("\nNot valid input\n");
        }
    }
}

void admin_login(){
    char password[20];
    printf("\nEnter password:");
    scanf("%s", password);
    if(!strcmp(password, PASSWORD)) {
        admin_panel();
    }
    else {
        printf("\nIncorrect Password!!\nAccess Denied\n");
    }
}

int main() {
    printf("Enter customer name: ");
    gets(customer_name);
    update_inventory_store();

    if (!strcmp(customer_name, "admin")) {
        admin_login();
    }
    else {
        welcome_screen();
    }
    return 0;
}




