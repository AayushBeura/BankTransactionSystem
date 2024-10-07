#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "accounts.h"

// Transaction Structure for storing last 3 transactions with a reference to Account Details
struct Transaction
{
    int transactionID; // Random transaction ID
    long int toAccNo;  // Account number to which it is transferred
    long int fromAccNo;
    char timestamp[20]; // Time of transaction
    float amount;       // Amount transferred
    char trans_type[20];
    struct Transaction* next;
};

// Function to generate a random transaction ID
int updateTransactionId() {
    long int trxid = 0;
    FILE *trxfp;

    // Open the file to read the last transaction ID
    trxfp = fopen("trxid.csv", "r");
    if (trxfp == NULL) {
        printf("Error: Could not open trxid.csv for reading\n");
        return -1;  // Return error code
    }

    // Read the transaction ID from the file
    char buffer[20];
    if (fgets(buffer, sizeof(buffer), trxfp) != NULL) {
        sscanf(buffer, "%d", &trxid);
    } else {
        printf("Error: Could not read transaction ID from file\n");
        fclose(trxfp);
        return -1;  // Return error code
    }

    fclose(trxfp); // Close the file after reading

    // Generate a new transaction ID
    trxid += 1;  // Increment transaction ID

    // Open the file again in write mode to update the transaction ID
    trxfp = fopen("trxid.csv", "w");
    if (trxfp == NULL) {
        printf("Error: Could not open trxid.csv for writing\n");
        return -1;  // Return error code
    }

    // Write the new transaction ID to the file
    fprintf(trxfp, "%d\n", trxid);
    fclose(trxfp);

    return trxid;
}

// Function to get the current timestamp in DD-MM-YYYY HH:MM format
void getCurrentTimestamp(char *timestampBuffer) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    sprintf(timestampBuffer, "%02d-%02d-%d %02d:%02d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900, local->tm_hour, local->tm_min);
}

// Function to log a transaction
void logTransaction(long int toAccNo, long int fromAccNo, float amount, const char *trans_type, int transactionID) {
    // Get the current timestamp
    char timestamp[20];
    getCurrentTimestamp(timestamp);

    // Open the transactions CSV file in append mode
    FILE *fp = fopen("transactions.csv", "a");
    if (fp == NULL) {
        printf("Error: Could not open transactions file\n");
        return;
    }

    // Log the transaction in the file
    fprintf(fp, "%d, %ld, %ld, %s, %.2f, %s\n",
            transactionID, toAccNo, fromAccNo, timestamp, amount, trans_type);

    // Close the file
    fclose(fp);

    printf("Transaction logged successfully.\n");
}



#endif