#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ACCOUNTS 10
#define MAX_NAME_LENGTH 50

typedef struct {
    char IBAN[20];
    char coin[4];
    float amount;
} BankAccount;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    BankAccount accounts[MAX_ACCOUNTS];
    int numAccounts;
} User;

void displayGUI() {
    printf("********************************************************\n");
    printf("*                                                      *\n");
    printf("*             Bank Management System                   *\n");
    printf("*                                                      *\n");
    printf("********************************************************\n");
    printf("*  Programming Project: Dragomir_Andrei_Raul           *\n");
    printf("*                                                      *\n");
    printf("********************************************************\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void LoginRegister();

void manageBankAccounts(User *user);

void viewBankAccounts(User *user);

void addBankAccount(User *user, char IBAN[20], char coin[4], float amount);

void deleteBankAccount(User *user, int accountNumber);

void editBankAccount(User *user, int accountNumber);

void saveUserData(User *user);

void loadUserData(User *user);

int main() {
    LoginRegister();
    return 0;
}

void LoginRegister() {
    displayGUI();

    User user;
    int choice;
    bool loggedIn = false;
    bool loginAttempted = false;

    loadUserData(&user); // Load user data when the program starts.

    do {
        if (!loginAttempted) {
            printf("Press 1 to login or 2 to create account (1/2): ");
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input. Please enter a number.\n");
                clearInputBuffer();
                continue;
            }
        }

        switch (choice) {
            case 1:
                printf("\nPlease login\n");
                printf("Enter your NAME: ");
                scanf("%49s", user.name);
                printf("Enter your SURNAME: ");
                scanf("%49s", user.surname);

                FILE *file = fopen("input.txt", "r");
                if (file == NULL) {
                    printf("Error opening file.\n");
                    return;
                }

                char line[100];
                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, user.name) != NULL && strstr(line, user.surname) != NULL) {
                        printf("\nWelcome to your account, %s %s!\n", user.name, user.surname);
                        loggedIn = true;
                        break;
                    }
                }
                fclose(file);

                if (!loggedIn) {
                    printf("Login failed. Please try again.\n");
                    loginAttempted = true;
                }

                break;

            case 2:
                printf("\nPlease register\n");
                printf("Enter your NAME: ");
                scanf("%49s", user.name);
                printf("Enter your SURNAME: ");
                scanf("%49s", user.surname);

                FILE *newFile = fopen("input.txt", "a");
                if (newFile == NULL) {
                    printf("Error opening file.\n");
                    return;
                }
                fprintf(newFile, "%s %s\n", user.name, user.surname);
                fclose(newFile);

                printf("\nThank you for registering, %s %s!\n", user.name, user.surname);
                loggedIn = true;
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                clearInputBuffer();
                break;
        }
    } while (!loggedIn);

    manageBankAccounts(&user);
}

void manageBankAccounts(User *user) {
    int choice;
    char IBAN[20];
    char coin[4];
    float amount;
    bool hasAccount;

    do {
        printf("\nSelect an option:\n");
        printf("1. View Bank Accounts\n");
        printf("2. Add Bank Account\n");
        printf("3. Edit Bank Account\n");
        printf("4. Delete Bank Account\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewBankAccounts(user);
                break;

            case 2:
                printf("Enter IBAN: ");
                scanf("%19s", IBAN);
                do {
                    printf("Enter coin (EUR/USD/RON): ");
                    scanf("%3s", coin);
                    if (strcmp(coin, "EUR") != 0 && strcmp(coin, "USD") != 0 && strcmp(coin, "RON") != 0) {
                        printf("Unrecognized coin. Please try again.\n");
                        clearInputBuffer();
                    }
                } while (strcmp(coin, "EUR") != 0 && strcmp(coin, "USD") != 0 && strcmp(coin, "RON") != 0);
                printf("Enter amount: ");
                scanf("%f", &amount);
                addBankAccount(user, IBAN, coin, amount);
                saveUserData(user); // Save user data after adding an account.
                break;

            case 3:
                if (user->numAccounts == 0) {
                    printf("You have no bank accounts to edit.\n");
                } else {
                    int accountNumber;
                    printf("Enter the account number to edit: ");
                    scanf("%d", &accountNumber);
                    if (accountNumber >= 1 && accountNumber <= user->numAccounts) {
                        editBankAccount(user, accountNumber);
                        saveUserData(user); // Save user data after editing an account.
                    } else {
                        printf("Invalid account number.\n");
                    }
                }
                break;

            case 4:
                if (user->numAccounts == 0) {
                    printf("You have no bank accounts to delete.\n");
                } else {
                    int accountNumber;
                    printf("Enter the account number to delete: ");
                    scanf("%d", &accountNumber);
                    if (accountNumber >= 1 && accountNumber <= user->numAccounts) {
                        deleteBankAccount(user, accountNumber);
                        saveUserData(user); // Save user data after deleting an account.
                    } else {
                        printf("Invalid account number.\n");
                    }
                }
                break;

            case 5:
                printf("Exiting...\n");
                return;

            default:
                printf("Invalid choice. Please try again.\n");
                clearInputBuffer();
                break;
        }
    } while (true);
}

void viewBankAccounts(User *user) {
    printf("\nBank Accounts for %s %s:\n", user->name, user->surname);
    if (user->numAccounts == 0) {
        printf("You have no bank accounts.\n");
    } else {
        for (int i = 0; i < user->numAccounts; i++) {
            printf("Account %d:\n", i + 1);
            printf("IBAN: %s\n", user->accounts[i].IBAN);
            printf("Coin: %s\n", user->accounts[i].coin);
            printf("Amount: %.2f\n", user->accounts[i].amount);
            printf("\n");
        }
    }
}

void addBankAccount(User *user, char IBAN[20], char coin[4], float amount) {
    if (user->numAccounts < MAX_ACCOUNTS) {
        strcpy(user->accounts[user->numAccounts].IBAN, IBAN);
        strcpy(user->accounts[user->numAccounts].coin, coin);
        user->accounts[user->numAccounts].amount = amount;
        user->numAccounts++;
        printf("Bank account added successfully.\n");
    } else {
        printf("You have reached the maximum limit for bank accounts.\n");
    }
}

void deleteBankAccount(User *user, int accountNumber) {
    if (accountNumber >= 1 && accountNumber <= user->numAccounts) {
        for (int i = accountNumber - 1; i < user->numAccounts - 1; i++) {
            strcpy(user->accounts[i].IBAN, user->accounts[i + 1].IBAN);
            strcpy(user->accounts[i].coin, user->accounts[i + 1].coin);
            user->accounts[i].amount = user->accounts[i + 1].amount;
        }
        user->numAccounts--;
        printf("Bank account deleted successfully.\n");
    } else {
        printf("Invalid account number.\n");
    }
}

void editBankAccount(User *user, int accountNumber) {
    int choice;
    char IBAN[20];
    char coin[4];
    float amount;

    printf("Select what you want to edit:\n");
    printf("1. IBAN\n");
    printf("2. Coin\n");
    printf("3. Amount\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter new IBAN: ");
            scanf("%19s", IBAN);
            strcpy(user->accounts[accountNumber - 1].IBAN, IBAN);
            printf("IBAN updated successfully.\n");
            break;
        case 2:
            do {
                printf("Enter new coin (EUR/USD/RON): ");
                scanf("%3s", coin);
                if (strcmp(coin, "EUR") != 0 && strcmp(coin, "USD") != 0 && strcmp(coin, "RON") != 0) {
                    printf("Unrecognized coin. Please try again.\n");
                    clearInputBuffer();
                }
            } while (strcmp(coin, "EUR") != 0 && strcmp(coin, "USD") != 0 && strcmp(coin, "RON") != 0);
            strcpy(user->accounts[accountNumber - 1].coin, coin);
            printf("Coin updated successfully.\n");
            break;
        case 3:
            printf("Enter new amount: ");
            scanf("%f", &amount);
            user->accounts[accountNumber - 1].amount = amount;
            printf("Amount updated successfully.\n");
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
}

void saveUserData(User *user) {
    FILE *file = fopen("userdata.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s %s\n", user->name, user->surname);
    fprintf(file, "%d\n", user->numAccounts);
    for (int i = 0; i < user->numAccounts; i++) {
        fprintf(file, "%s %s %.2f\n", user->accounts[i].IBAN, user->accounts[i].coin, user->accounts[i].amount);
    }

    fclose(file);
}

void loadUserData(User *user) {
    FILE *file = fopen("userdata.txt", "r");
    if (file == NULL) {
        printf("No user data found.\n");
        return;
    }

    fscanf(file, "%s %s", user->name, user->surname);
    fscanf(file, "%d", &user->numAccounts);
    for (int i = 0; i < user->numAccounts; i++) {
        fscanf(file, "%s %s %f", user->accounts[i].IBAN, user->accounts[i].coin, &user->accounts[i].amount);
    }

    fclose(file);
}
