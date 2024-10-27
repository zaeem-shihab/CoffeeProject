#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ingredient quantities in grams and milliliters for each type of coffee
#define BEANS_g 8             // grams of coffee beans per coffee
#define WATER_ESP_ml 30      // water for Espresso in milliliters

#define MILK_CAP_ml 70        // milk for Cappuccino in milliliters
#define WATER_CAP_ml 30      // water for Cappuccino in milliliters

#define MILK_MOCHA_ml 160      // milk for Mocha in milliliters
#define CHOCOSYRUP_ml 30      // chocolate syrup for Mocha in milliliters
#define WATER_MOCHA_ml 39    // water for Mocha in milliliters

// Prices for different types of coffees
#define PRICE_ESP 3.50        // price of Espresso in AED
#define PRICE_CAP 4.50       // price of Cappuccino in AED
#define PRICE_MOCHA 5.50    // price of Mocha in AED

// Setting thresholds for low ingredients
#define LOW_BEANS_g 25        // low beans threshold in grams
#define LOW_WATER_ml 95       // low water threshold in milliliters
#define LOW_MILK_ml 250       // low milk threshold in milliliters
#define LOW_CHOCOSYRUP_ml 65  // low chocolate syrup threshold in milliliters

// Admin password to access admin-operator mode
#define ADMIN_PW "08840"        // admin password

// Declaring global variables for ingredient levels and total sales
int beans, water, milk, chocosy;  // Ingredient levels in grams and milliliters
float total_sales;               // Total sales from the coffee machine

// Function prototypes for each tasks
void init_machine();
void display_menu();
void place_order();
int accept_payment(float price);
void admin_access();
void check_levels();
void show_ingredients();
int low_alert();
void replenish_ingredients();
void change_prices();
void reset_sales();

// Initialize ingredient levels for all coffees and reset total sales from the machine
void init_machine() {
    beans = 25;
    water = 95;
    milk = 250;
    chocosy = 65;

    total_sales = 0.0;
}


// Main function that controls the flow of the program
int main() {
    int option = 0;

    // To initialize the coffee machine at the start
    init_machine();

    // This is the main loop that displays the menu and processes user inputs
    while (option != 3) {

        display_menu();

        printf("Please choose an option from above: ");
        
         // Checks if the input received from the user is a valid integer or not
        if (scanf("%d", &option) != 1) {     // If invalid, it returns a value other than 1 
            printf("Invalid input. Please enter a number.\n");
            
            // Clears the input buffer to remove left-over characters to prevent reading invalid data  
            while (getchar() != '\n');      // Discards all characters until a new line is found
            // Restarts the loop again to allow the user to enter a valid option
            continue;  // To go back to the menu
        }
        // Handle the users choice
        if (option == 1) {
            place_order();
        } else if (option == 2) {
            admin_access();
        } else if (option != 3) {
            printf("That’s not a valid option. Please try again.\n");
        }
    }

    // Ending the program
    printf("The coffee machine will turn off now...\n");
    return 0;
}


// Function to display the main menu
void display_menu() {
    printf("\n...........................\n");
    printf("        MAIN MENU         \n");
    printf("...........................\n");
    printf("  [1] Brew Coffee        \n");    // Option to brew coffee for the customer
    printf("  [2] Admin Area         \n");    // Option to access the admin panel for the user/admin 
    printf("  [3] Exit               \n");    // Option to exit the program
    printf("...........................\n");
}


// Function to handle the users coffee order
void place_order() {
    int choice = 0;
    float price = 0.0;
    const char* coffee_name;

    while (1) {
        printf("\n_________________________\n");
        printf("     SELECT YOUR BREW    \n");
        printf("_________________________\n");

        // Displays the available coffee types depending on the ingredients
        if (beans >= BEANS_g && water >= WATER_ESP_ml)
            printf(" 1. Espresso     - %.2f AED\n", PRICE_ESP);
        else
            printf(" 1. Espresso     - Unavailable\n");

        if (beans >= BEANS_g && water >= WATER_CAP_ml && milk >= MILK_CAP_ml)
            printf(" 2. Cappuccino   - %.2f AED\n", PRICE_CAP);
        else
            printf(" 2. Cappuccino   - Unavailable\n");

        if (beans >= BEANS_g && water >= WATER_MOCHA_ml && milk >= MILK_MOCHA_ml && chocosy >= CHOCOSYRUP_ml)
            printf(" 3. Mocha        - %.2f AED\n", PRICE_MOCHA);
        else
            printf(" 3. Mocha        - Unavailable\n");

        printf(" 0. Cancel Order\n");
        printf("_________________________\n");

        // Asks the user to select a coffee type
        printf("Choose your cup of joe: ");
        scanf("%d", &choice);

        if (choice == 0) return;

        // Checks if the users choice is valid and if the machine has enough ingredients for that coffee
        if (choice == 1 && beans >= BEANS_g && water >= WATER_ESP_ml) {
            coffee_name = "Espresso";
            price = PRICE_ESP;
            beans -= BEANS_g;
            water -= WATER_ESP_ml;
        } else if (choice == 2 && beans >= BEANS_g && water >= WATER_CAP_ml && milk >= MILK_CAP_ml) {
            coffee_name = "Cappuccino";
            price = PRICE_CAP;
            beans -= BEANS_g;
            water -= WATER_CAP_ml;
            milk -= MILK_CAP_ml;
        } else if (choice == 3 && beans >= BEANS_g && water >= WATER_MOCHA_ml && milk >= MILK_MOCHA_ml && chocosy >= CHOCOSYRUP_ml) {
            coffee_name = "Mocha";
            price = PRICE_MOCHA;
            beans -= BEANS_g;
            water -= WATER_MOCHA_ml;
            milk -= MILK_MOCHA_ml;
            chocosy -= CHOCOSYRUP_ml;
        } else {
            printf("Sorry, we can't serve that right now. Please try again.\n");
            continue;
        }
        // Double-checks if the user is sure about their selected coffee choice
        char confirm;
        printf("You've selected a %s for %.2f AED.\n", coffee_name, price);
        printf("\nWould you like to confirm this delightful brew before we proceed further? [ Y / N ]: ");
        scanf(" %c", &confirm);

        // If not confirmed, go back to coffee choice
        if (confirm != 'Y' && confirm != 'y') {
            printf("Order confirmation failed. Redirecting to Main Menu...\n");
            return;  // Go back to coffee choice section
        }
        // To process the payment for the selected coffee type
        if (accept_payment(price)) {
            total_sales += price;
            printf(" - You ordered a %s for %.2f AED.\n", coffee_name, price);
            printf("\nEnjoy this warm cup of coffee :)\n");
            
            // Checks the machine for low ingredients and returns the number of alerts for low ingredients
            int alerts = low_alert();  // Call the low_alert() function to check for alerts
            
            if (alerts > 0) {
                printf("\n[ ADMIN PANEL ] - %d notification(s) pending review.\n", alerts);  // Notifies the admin that there are pending ingredient alerts.
                printf("\nPress 'V' to View, 'C' to Cancel, or 'N' to be Notified Later: "); // Asks the admin/user if they want to view the alerts now, cancel, or be reminded later
                
                char choice;
                scanf(" %c", &choice);                  // Reads the users/admins input for the selected option
                if (choice == 'V' || choice == 'v') {
                    show_ingredients();                 // Calls the function to display the ingredient levels with alerts.
                } else if (choice == 'N' || choice == 'n') {
                    printf("Reminder: Check ingredient levels later in the Admin Panel.\n");
                } else {
                    printf("Notification dismissed. Continue brewing!\n");
                }
            }
        }
    }
}

// Function to manage the payment for the coffee orders
int accept_payment(float price) {
    float paid = 0.0, coin;

    printf("\nYour total is %.2f AED. Kindly insert a 0.5 or 1 AED coin or a 5 AED note.\n", price);

    // Continues looping until the total amount is fully paid
    while (paid < price) {
        printf("Insert your coins: ");
        scanf("%f", &coin);

        if (coin == 0.5 || coin == 1.0 || coin == 5.0) {
            paid += coin;
            printf("You've paid %.2f AED so far.\n", paid);
        } else {
            printf("Error: Invalid coin or note. Please retrieve it and insert a valid currency.\n");
        }
    }
    printf("\nThank you! Payment complete.\n");
    
    if (paid > price) {
    printf("\nPlease collect your change of %.2f AED.\n", paid - price);   // Returning extra coins back to the user
    paid = price;                                                         // To make sure no excess change is left
    }
    return 1;
}


// Function to grant the admin access to machine operations
void admin_access() {
    char password[10];
    int option = -1;

    printf("\n[SECURE LOGIN]  Please enter admin credentials: ");
    scanf("%9s", password);

    // Checks if the password matches the admin password as set previously
    if (strcmp(password, ADMIN_PW) != 0) {
        printf("\n[ACCESS DENIED]  Authentication Failed...\n");
        printf(">> Redirecting to Main Menu <<\n\n");
        return;
    }
    // If correct, it displays a successful authentication message to the user/admin
    printf("\n[ACCESS GRANTED]  Authentication Successful...\n");
    printf(">> System Access: ADMIN PANEL Initiating <<\n\n");


    // Repeat admin menu until exit
    while (option != 0) {
        printf("\n=====================================\n");
        printf("             ADMIN PANEL            \n");
        printf("=====================================\n");
        printf("   > [1] Show Ingredient Levels     \n");
        printf("   > [2] Replenish Ingredients      \n");
        printf("   > [3] Change Coffee Prices       \n");
        printf("   > [4] Reset Sales Amount         \n");
        printf("   > [0] Exit Admin Panel           \n");
        printf("=====================================\n");

        // [APEX] - Admin Panel Execution Hub: Optimizing operations, one command at a time.
        printf("[APEX] Engage module >> Action Set 0 - 4 << "); // Asking for the users/admins choice of action 
        scanf("%d", &option);

        // Execute the users/admins chosen option
        switch (option) {
            case 1:
                show_ingredients();
                break;
            case 2:
                replenish_ingredients();
                break;
            case 3:
                change_prices();
                break;
            case 4:
                reset_sales();
                break;
            case 0:
                printf("Exiting Admin Panel...\n");
                break;
            default:
                printf("Error: Invalid selection. Please try again.\n");
        }
    }
}

// Function to check the ingredient levels and to return the number of low-level alerts to the user/admin
int low_alert() {
    int alert_count = 0;

    if (beans < LOW_BEANS_g) alert_count++;
    if (water < LOW_WATER_ml) alert_count++;
    if (milk < LOW_MILK_ml) alert_count++;
    if (chocosy < LOW_CHOCOSYRUP_ml) alert_count++;

    return alert_count;
}

// Function to display the current ingredient levels and total sales to the user/admin
void show_ingredients() {
    printf("\n Current Ingredient Levels:\n");
    printf(" - Beans              : %d g\n", beans);
    printf(" - Water              : %d ml\n", water);
    printf(" - Milk               : %d ml\n", milk);
    printf(" - Chocolate Syrup    : %d ml\n", chocosy);
    printf("\nTotal Sales:\n");
    printf("-> %.2f AED\n", total_sales);
    int alerts = low_alert();
    
    // Alerts the operator when the ingredient levels are low
    if (alerts > 0) {
        printf("\nYou have %d low ingredient alert(s):\n", alerts);
        if (beans < LOW_BEANS_g) printf("\n Caution: Coffee bean levels are low!!!\n");
        if (water < LOW_WATER_ml) printf("\n Warning: Water levels are running low!!\n");
        if (milk < LOW_MILK_ml) printf("\n Alert: Low milk supply detected!\n");
        if (chocosy < LOW_CHOCOSYRUP_ml) printf("\n Notice: Refill chocolate syrup soon.\n");
    }
}


// Function to replenish the ingredients to random levels
void replenish_ingredients() {
    beans = 65 + rand() % 51;
    water = 120 + rand() % 51;
    milk = 365 + rand() % 51;
    chocosy = 100 + rand() % 51;

    printf("\n[STATUS]  All ingredients have been restocked and replenished to optimal levels...\n");
    show_ingredients();
}


// Function to modify the prices for different coffee types
void change_prices() {
    printf("\n");
    printf("       Current Coffee Prices       \n");
    printf("       -----------------------       \n");
    printf(" 1. Espresso     | %.2f AED |\n", PRICE_ESP);
    printf(" 2. Cappuccino   | %.2f AED |\n", PRICE_CAP);
    printf(" 3. Mocha        | %.2f AED |\n", PRICE_MOCHA);

    // To update the coffee prices in admin mode
    puts("\nUpdate Coffee Prices:");

    // Asks the user/admin to enter new prices for different types of coffee
    float new_price_esp, new_price_cap, new_price_mocha;

    printf("> Enter a new price for Espresso: ");
    scanf("%f", &new_price_esp);
    // Validation for Espresso price
    if (new_price_esp < 0) {
        printf("Error: Price cannot be negative. Please try again.\n");
        return; // Exit the function if the price is invalid
    }

    printf("> Enter a new price for Cappuccino: ");
    scanf("%f", &new_price_cap);
    // Validation for Cappuccino price
    if (new_price_cap < 0) {
        printf("Error: Price cannot be negative. Please try again.\n");
        return; // Exit the function if the price is invalid
    }

    printf("> Enter a new price for Mocha: ");
    scanf("%f", &new_price_mocha);
    // Validation for Mocha price
    if (new_price_mocha < 0) {
        printf("Error: Price cannot be negative. Please try again.\n");
        return; // Exit the function if the price is invalid
    }

    // Displays the updated prices to the user/admin
    printf("\n ***** REVISED COFFEE PRICES ***** \n");
    printf(" --------------------------------- \n");
    printf(" 1. Espresso    :  %.2f AED\n", new_price_esp);
    printf(" 2. Cappuccino  :  %.2f AED\n", new_price_cap);
    printf(" 3. Mocha       :  %.2f AED\n", new_price_mocha);
    printf(" --------------------------------- \n");

    printf("\n* Prices updated successfully. *\n");
}


// Function to reset the total sales amount to zero
void reset_sales() {
    char confirm;

    // Displays the current total sales amount to the user/admin
    printf("Total Sales: %.2f AED\n", total_sales);
    printf("\nDo you want to reset the total sales amount? [ Y / N ]: ");
    scanf(" %c", &confirm);

    // Asks the user/admin to confirm before resetting and then updates the total sales amount to zero
    if (confirm == 'Y' || confirm == 'y') {
        total_sales = 0.0;
        printf("\nThe total sales amount has been re-set to zero. Please collect the cash.\n");
    } else {
        printf("\nNo changes have been made to the sales amount.\n");
    }
}

