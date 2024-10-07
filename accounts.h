#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transactions.h"

struct Account{
    char username[25];
    char password[25];
    long int accountNo;
    char name[50];
    float balance;
    char accCreatedOn[20];
    char branch[50];
    char ifsc[15];
    char accountType[20];
    float dailyLimit;
    float interestRate;
    float dailySpend;
    char lastTransactionDate[20];
    struct Transaction* transactions;
};

// Function to create a new user entry in the database
void createAccount(char *str) {
    FILE* fp;

    // Open the file in append mode to add new data
    fp = fopen("db.csv", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Append new user data
    fprintf(fp, "%s\n", str);
    fclose(fp); // Close the file after writing
}

// Function to check if that Username exists or not [use in Login Function in 1st Step]
int userExists(const char *username) {
    FILE *fp = fopen("db.csv", "r");
    if (fp == NULL) {
        return 0; // File not found or no users exist yet
    }

    char buffer[512];
    // Check each line for the username
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char existingUsername[25];
        sscanf(buffer, "%[^,]", existingUsername);  // Read the username from the line

        if (strcmp(existingUsername, username) == 0) {
            fclose(fp);
            return 1;  // User exists
        }
    }

    fclose(fp);
    return 0;  // User does not exist
}


struct Account* findUser(const char* username, const char* password) {
    FILE* fp = fopen("db.csv", "r");
    if (fp == NULL) {
        printf("Error: Could not open file db.csv\n");
        return NULL;
    }

    char buffer[512];
    struct Account* user = (struct Account*)malloc(sizeof(struct Account));
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        sscanf(buffer, "%[^,], %[^,], %ld, %[^,], %f, %[^,], %[^,], %[^,], %[^,], %f, %f, %f, %s",
                user->username, user->password, &user->accountNo, user->name,
                &user->balance, user->accCreatedOn, user->branch,
                user->ifsc, user->accountType, &user->dailyLimit,
                &user->interestRate, &user->dailySpend, user->lastTransactionDate);

        // Check if the username and password match
        if (strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0) {
            fclose(fp); // Close file before returning
            return user; // Return the user struct
        }
    }

    free(user); // Free allocated memory if not found
    fclose(fp); // Close the file
    return NULL; // Return NULL if not found
}

struct Account* findUserusingAcct(long int accno){
    FILE* fp = fopen("db.csv", "r");
    if (fp == NULL) {
        printf("Error: Could not open file db.csv\n");
        return NULL;
    }

    char buffer[512];
    struct Account* user = (struct Account*)malloc(sizeof(struct Account));
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        sscanf(buffer, "%[^,], %[^,], %ld, %[^,], %f, %[^,], %[^,], %[^,], %[^,], %f, %f, %f, %s",
                user->username, user->password, &user->accountNo, user->name,
                &user->balance, user->accCreatedOn, user->branch,
                user->ifsc, user->accountType, &user->dailyLimit,
                &user->interestRate, &user->dailySpend, user->lastTransactionDate);

        if (user->accountNo == accno) {
            fclose(fp); // Close file before returning
            return user; // Return the user struct
        }
    }

    free(user); // Free allocated memory if not found
    fclose(fp); // Close the file
    return NULL; // Return NULL if not found
}

struct Account* findUserusingUsername(const char* usern){
    FILE* fp = fopen("db.csv", "r");
    if (fp == NULL) {
        printf("Error: Could not open file db.csv\n");
        return NULL;
    }

    char buffer[512];
    struct Account* user = (struct Account*)malloc(sizeof(struct Account));
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        sscanf(buffer, "%[^,], %[^,], %ld, %[^,], %f, %[^,], %[^,], %[^,], %[^,], %f, %f, %f, %s",
                user->username, user->password, &user->accountNo, user->name,
                &user->balance, user->accCreatedOn, user->branch,
                user->ifsc, user->accountType, &user->dailyLimit,
                &user->interestRate, &user->dailySpend, user->lastTransactionDate);

        if (strcmp(user->username, usern)==0) {
            fclose(fp); // Close file before returning
            return user; // Return the user struct
        }
    }

    free(user); // Free allocated memory if not found
    fclose(fp); // Close the file
    return NULL; // Return NULL if not found
}

void updateUserInDB(struct Account *user) {
    // Open the existing database file for reading
    FILE *fp = fopen("db.csv", "r");
    if (fp == NULL) {
        printf("Error: Could not open file db.csv\n");
        return;
    }

    // Temporary file to store updated content
    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        printf("Error: Could not open temporary file\n");
        fclose(fp);
        return;
    }

    char buffer[512];
    int userUpdated = 0;

    // Read each line and copy to the temporary file with updates
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        struct Account tempUser; // Temporary user struct to hold the read data
        
        // Read the existing user data from the CSV
        sscanf(buffer, "%[^,], %[^,], %ld, %[^,], %f, %[^,], %[^,], %[^,], %[^,], %f, %f, %f, %s",
                tempUser.username, tempUser.password, &tempUser.accountNo, tempUser.name,
                &tempUser.balance, tempUser.accCreatedOn, tempUser.branch,
                tempUser.ifsc, tempUser.accountType, &tempUser.dailyLimit,
                &tempUser.interestRate, &tempUser.dailySpend, tempUser.lastTransactionDate);

        // Check if this is the current session user to be updated
        if (strcmp(tempUser.username, user->username) == 0) {
            // Update the entire user data with the new data from the input user parameter
            strcpy(tempUser.password, user->password); // Assuming password also needs to be updated
            tempUser.accountNo = user->accountNo; 
            strcpy(tempUser.name, user->name);
            tempUser.balance = user->balance;
            strcpy(tempUser.accCreatedOn, user->accCreatedOn);
            strcpy(tempUser.branch, user->branch);
            strcpy(tempUser.ifsc, user->ifsc);
            strcpy(tempUser.accountType, user->accountType);
            tempUser.dailyLimit = user->dailyLimit;
            tempUser.interestRate = user->interestRate;
            tempUser.dailySpend = user->dailySpend;
            strcpy(tempUser.lastTransactionDate, user->lastTransactionDate);

            // Write the updated user info to the temp file
            fprintf(temp, "%s, %s, %ld, %s, %.2f, %s, %s, %s, %s, %.2f, %.2f, %.2f, %s\n",
                    tempUser.username, tempUser.password, tempUser.accountNo, tempUser.name,
                    tempUser.balance, tempUser.accCreatedOn, tempUser.branch,
                    tempUser.ifsc, tempUser.accountType, tempUser.dailyLimit,
                    tempUser.interestRate, tempUser.dailySpend, tempUser.lastTransactionDate);

            userUpdated = 1; // Mark as updated
        } else {
            // Write the unchanged user info to the temp file
            fprintf(temp, "%s", buffer);
        }
    }

    fclose(fp);
    fclose(temp);

    // Replace the old file with the updated file
    remove("db.csv");
    rename("temp.csv", "db.csv");

    if (userUpdated) {
        printf("Your account details have been updated in the database.\n");
    } else {
        printf("Error: Could not find user in the database.\n");
    }
}

long int updateAccNo() {
    long int accno = 0;
    FILE *accfp;

    // Open the file to read the last account number
    accfp = fopen("accno.csv", "r");
    if (accfp == NULL) {
        printf("Error: Could not open accno.csv for reading\n");
        return -1;  // Return error code
    }

    // Read the account number from the file
    char buffer[20];
    if (fgets(buffer, sizeof(buffer), accfp) != NULL) {
        sscanf(buffer, "%ld", &accno);
    } else {
        printf("Error: Could not read account number from file\n");
        fclose(accfp);
        return -1;  // Return error code
    }

    fclose(accfp); // Close the file after reading

    // Generate a new account number
    accno += rand() % 100000;

    // Open the file again in write mode to update the account number
    accfp = fopen("accno.csv", "w");
    if (accfp == NULL) {
        printf("Error: Could not open accno.csv for writing\n");
        return -1;  // Return error code
    }

    // Write the new account number to the file
    fprintf(accfp, "%ld\n", accno);
    fclose(accfp);

    return accno;
}

// Function to store all transactions in a circular LinkedList (used in Statement())
// Function to store transactions in a linear LinkedList (used in Statement())
// Function to add a transaction to the linear linked list
struct Transaction* linearAdd(struct Transaction tempTrans, struct Transaction* head) {
    struct Transaction* newTransaction = (struct Transaction*)malloc(sizeof(struct Transaction));
    if (newTransaction == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    *newTransaction = tempTrans;
    newTransaction->next = head; // Add to the front of the list
    return newTransaction; // Return the new head of the list
}

// Function to count the number of transactions in the linked list
int countTransactions(struct Transaction* head) {
    int count = 0;
    struct Transaction* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}
// Recursive function to print the last 3 transactions

void printTransactionsInReverse(struct Transaction* head, int count) {
    if (head == NULL || count == 0) {
        return;
    }
    // Recursively go to the end
    printTransactionsInReverse(head->next, count - 1);

    // Print the transaction
    printf("Transaction ID: %d\n", head->transactionID);
    printf("To Account No: %ld\n", head->toAccNo);
    printf("From Account No: %ld\n", head->fromAccNo);
    printf("Type: %s\n", head->trans_type);
    printf("Amount: Rs. %.2f\n", head->amount);
    printf("Timestamp: %s\n", head->timestamp);
    printf("---------------------------\n");
}

#endif