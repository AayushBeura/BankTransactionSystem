#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "format.h"
#include "accounts.h"
#include "transactions.h"

struct Account *current_session_user = NULL;

void formatDBCSV();
void WelcomePage();
void Login();
void createUser();
void viewBalance();
void deposit();
void transfera2a();
void withdrawal();
void statement();
void settings();
void changePassword();
void deleteAccount();
void logout();

void proceedOrCancel()
{
    char key;
    printf("Press any Key to proceed with the Transaction or Esc to cancel...\n");
    key = _getch(); // Wait for keypress
    if (key == 27)
    { // Esc key is pressed
        printf("Transaction cancelled.\n");
        Home();
        return;
    }
    else
    { // Any key is pressed
        printf("Proceeding with the transaction...\n");
        return;
    }
}

void HomeLogoutCheck()
{
    char key;
    printf("Press any Key to go to Home Page or Esc to Logout...\n");
    key = _getch(); // Wait for keypress
    if (key == 27)
    {
        // Esc key is pressed
        logout();
    }
    else
    { // Any key is pressed
        Home();
    }
}

void depositChecker()
{
    char key;
    printf("Enter Any Key for Confirmation...\n");
    key = _getch(); // Wait for keypress
    if (key == 27)
    {
        // Esc key is pressed
        printf("Transaction cancelled.\n");
        Home();
    }
    else
    { // Any key is pressed
        return;
    }
}

void checkPoint()
{
    HomeLogoutCheck();
}

void updateInterest(struct Account *current_session_user)
{
    // Get the current date
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    char current_date[11]; // Reserve space for date (DD-MM-YYYY)
    sprintf(current_date, "%02d-%02d-%d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);

    // Extract the account opening date
    char *opening_date = current_session_user->accCreatedOn; // Assuming this is in DD-MM-YYYY format

    // Convert the opening date to struct tm for comparison
    struct tm opening_tm;
    sscanf(opening_date, "%02d-%02d-%d", &opening_tm.tm_mday, &opening_tm.tm_mon, &opening_tm.tm_year);
    opening_tm.tm_mon -= 1;     // tm_mon is 0-based in struct tm
    opening_tm.tm_year += 1900; // tm_year is years since 1900

    // Increment the year for comparison
    opening_tm.tm_year += 1; // Check for the next anniversary

    // Calculate the anniversary date
    char anniversary_date[11]; // Buffer for anniversary date
    strftime(anniversary_date, sizeof(anniversary_date), "%d-%m-%Y", &opening_tm);

    // Check if the current date matches the anniversary date
    if (strcmp(current_date, anniversary_date) == 0)
    {
        // Calculate interest: Balance * (Interest Rate / 100)
        float interest = current_session_user->balance * (current_session_user->interestRate / 100);

        // Add the interest to the account balance
        current_session_user->balance += interest;

        // Log the interest added as a "Credit" transaction
        // addTransaction(current_session_user->account, current_session_user->account->accno, interest, 1);

        // Print a confirmation message
        printf("Happy Anniversary! Interest of %.2f has been credited to your account.\n", interest);
        printf("Updated Balance: Rs. %.2f\n", current_session_user->balance);

        // Update the last transaction date to the current date
        getCurrentDate(current_session_user->lastTransactionDate);
    }
}

void updateLimit(struct Account *current_session_user)
{
    // Get the current date
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    char current_date[11]; // Reserve space for the date part only
    sprintf(current_date, "%02d-%02d-%d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);

    // Check if the user has any transactions (referencing the last transaction timestamp from `logTransaction`)
    if (current_session_user->lastTransactionDate[0] == '\0')
    {
        // If no transaction exists, set the last transaction date to the current date
        strcpy(current_session_user->lastTransactionDate, current_date);
        return;
    }

    // Compare the current date with the last transaction date
    if (strcmp(current_date, current_session_user->lastTransactionDate) != 0)
    {
        // The date has changed, so reset the daily spend
        current_session_user->dailySpend = 0;

        // Update the last transaction date to the current date
        strcpy(current_session_user->lastTransactionDate, current_date);
        updateUserInDB(current_session_user);
        printf("Daily transaction limit has been reset. You can start fresh today.\n");
    }
}

// First Page after main() gets executed
void WelcomePage()
{
    char choice;
    system("cls");
    printf("--- E-Banking System ---\n 1. Existing User\n 2. New User\n 3. Exit\n");
    choice = _getch();
    switch (choice)
    {
    case '1':
        Login();
        break;
    case '2':
        createUser();
        break;
    case '3':
        printf("Thank You! \n");
        return;
    default:
        printf("Invalid Operation!\n");
        WelcomePage();
    }
}

void createUser()
{
    system("cls");
    printf("--- New User Registration ---\n");
    char newusername[25];
    while (1)
    {
        printf("Enter E-Banking Username: ");
        fgets(newusername, 25, stdin);
        newusername[strcspn(newusername, "\n")] = '\0';
        if (userExists(newusername) == 1)
        {
            printf("Username already exists! Please choose another username.\n");
        }
        else
        {
            break;
        }
    }

    char newpass[25];
    printf("Enter Password: ");
    fgets(newpass, 25, stdin);
    newpass[strcspn(newpass, "\n")] = '\0';

    long int accno;
    accno = updateAccNo();
    char name[50];
    printf("Enter your full name: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';
    capitalizeName(name);

    char branch[50];
    printf("Enter Branch Name: ");
    fgets(branch, 50, stdin);
    branch[strcspn(branch, "\n")] = '\0';

    char ifsc[15];
    printf("Enter IFSC code: ");
    fgets(ifsc, 15, stdin);
    ifsc[strcspn(ifsc, "\n")] = '\0';
    formatIFSC(ifsc);

    char accType[20];
    printf("Enter Account Type (Savings/Current): ");
    fgets(accType, 20, stdin);
    accType[strcspn(accType, "\n")] = '\0';
    printf("Creating User...\n");
    sleep(1);
    float balance = 0.00;
    float dailyLimit = 40000.00;
    float dailySpend = 0.00;
    float interestRate = 4.50;
    char accCreateDate[20];
    getCurrentDate(accCreateDate);
    char lastTransactionDate[20];
    getCurrentDate(lastTransactionDate);

    char buffer[512];
    snprintf(buffer, sizeof(buffer), "%s, %s, %ld, %s, %.2f, %s, %s, %s, %s, %.2f, %.2f, %.2f, %s\n",
             newusername, newpass, accno, name, balance, accCreateDate,
             branch, ifsc, accType, dailyLimit, interestRate,
             dailySpend, lastTransactionDate);

    createAccount(buffer);
    printf("User Creation Successful! Redirecting to the Login Portal...\n");
    sleep(1);
    Login();
}

int attemptCount = 3;
void Login()
{
    system("cls");
    char uname[25];
    printf("--- Login Portal ---\n");
    printf("Enter E-Banking Username: ");
    fgets(uname, sizeof(uname), stdin);
    uname[strcspn(uname, "\n")] = '\0';

    int usernameExists = userExists(uname);

    // If username does not exist, prompt to create a new account
    if (!usernameExists)
    {
        char ch;
        printf("Username does not exist. Do you want to create a new account? (y/n): ");
        scanf(" %c", &ch);
        clearInputBuffer();
        if (ch == 'y' || ch == 'Y')
        {
            createUser();
        }
        else
        {
            printf("Returning to Welcome Page...\n");
            sleep(1);
            WelcomePage();
            return;
        }
    }

    // Username exists; now check the password
    char upass[25];
    while (attemptCount > 0)
    {
        printf("Enter Password: ");
        fgets(upass, sizeof(upass), stdin);
        upass[strcspn(upass, "\n")] = '\0';
        current_session_user = findUser(uname, upass);
        if (current_session_user != NULL)
        {
            attemptCount = 3;
            printf("Login Successful! Redirecting to the Home Banking Page...\n");
            sleep(1);
            Home();
            return;
        }
        else
        {
            attemptCount--;
            if (attemptCount > 0)
            {
                printf("Incorrect Password! %d Attempt(s) remaining!\n", attemptCount);
            }
            else
            {
                printf("You have entered too many wrong attempts. Please try again later!\n");
                return;
            }
        }
    }
}

void Home()
{
    system("cls");
    // update limit and interest to be added
    updateInterest(current_session_user);
    updateLimit(current_session_user);
    printf("--- Welcome to the Home Banking Page! ---\n");
    char choice;
    printf(" 1. View Balance\n 2. Deposit Money\n 3. Transfer Funds\n 4. ATM Withdrawal\n 5. Account Statement\n 6. Account Settings\n 7. Logout\n ");
    choice = _getch();
    printf("\n");
    switch (choice)
    {
    case '1':
        viewBalance();
        break;
    case '2':
        deposit();
        break;
    case '3':
        transfera2a();
        break;
    case '4':
        withdrawal();
        break;
    case '5':
        statement();
        break;
    case '6':
        settings();
        break;
    case '7':
        logout();
        break;
    default:
        printf("Invalid choice!\n");
        Home();
    }
}

// As the name suggests, to check ki aapki dhanrashi kitni hai
void viewBalance()
{
    system("cls");
    printf("--- Balance Details of Account Holder ---\n");
    printf("Account Holder Name: %s\n", current_session_user->name);
    printf("Account Number: %ld\n", current_session_user->accountNo);
    printf("Account Type: %s\n", current_session_user->accountType);
    printf("Current Balance: Rs. %.2f\n", current_session_user->balance);
    char ch;
    sleep(1);
    checkPoint();
}

void deposit()
{
    system("cls");
    float amount;
    printf("--- Deposit Money ---\n");
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    // Validate deposit amount
    if (amount <= 0)
    {
        printf("Invalid amount! Deposit must be greater than zero.\n");
        return;
    }
    sleep(1);
    printf("Kindly proceed to the Counter %d to deposit Rs. %.2f\n", rand() % 4 + 1, amount);
    depositChecker();
    sleep(1);
    printf("Confirming status...\n");
    sleep(1);
    current_session_user->balance += amount;
    getCurrentDate(current_session_user->lastTransactionDate);

    printf("Rs. %.2f has been successfully deposited to your account.\n", amount);
    printf("Updated Balance: Rs. %.2f\n", current_session_user->balance);
    sleep(1);
    // Update the corresponding user's balance in the CSV file
    updateUserInDB(current_session_user);

    int trid = updateTransactionId();
    // Optionally log the transaction
    logTransaction(current_session_user->accountNo, -1, amount, "Deposit", trid); // To create a transaction log
    checkPoint();
}

void transfera2a()
{
    system("cls");
    updateLimit(current_session_user); // Update the daily limit based on the last transaction date
    float amt;
    int found = 0;
    char ch;
    printf("--- Transfer to Another Account ---\n");
    printf("1. Pay Via Account No. \n");
    printf("2. Pay Via Username\n");
    ch = _getch();

    switch (ch)
    {
    case '1':
    {
        long int traccno;
        printf("Enter Account No.: ");
        scanf("%ld", &traccno);
        printf("Enter Amount to Transfer: ");
        scanf("%f", &amt);

        // Validation for invalid amount
        if (amt <= 0)
        {
            printf("Invalid amount entered! Please enter a valid amount.\n");
            checkPoint();
        }

        // Check daily limit
        if (amt + current_session_user->dailySpend > current_session_user->dailyLimit)
        {
            printf("Daily Transaction Limit Exceeded! You can perform new transactions tomorrow!\n");
            checkPoint();
        }

        // Check for sufficient funds
        if (amt > current_session_user->balance)
        {
            printf("Insufficient Funds!\n");
            checkPoint();
        }

        // Find the receiver's account using account number
        struct Account *receiver = findUserusingAcct(traccno);
        if (receiver == NULL)
        {
            printf("Invalid Account Number!\n");
            checkPoint();
        }

        // Confirmation of transfer
        printf("You are transferring Rs. %.2f to %s\n", amt, receiver->name);
        proceedOrCancel();
        // Debit money from sender's account
        current_session_user->balance -= amt;
        current_session_user->dailySpend += amt;

        // Credit money to receiver's account
        receiver->balance += amt;

        // Generate a single transaction ID for both the sender and receiver logs
        int transactionID = updateTransactionId();

        // Log the transactions with the same transaction ID
        logTransaction(receiver->accountNo, current_session_user->accountNo, amt, "Transfer", transactionID);
        logTransaction(current_session_user->accountNo, receiver->accountNo, amt, "Received", transactionID);

        // Update both users in the database
        updateUserInDB(current_session_user); // Update the sender's balance in the database
        updateUserInDB(receiver);             // Update the receiver's balance in the database

        printf("Transaction Successful!!\nYour Updated Balance: Rs. %.2f\n", current_session_user->balance);
        checkPoint();
        break;
    }
    case '2':
    {
        char trusername[25];
        printf("Enter Username: ");
        scanf("%s", trusername);
        printf("Enter Amount to Transfer: ");
        scanf("%f", &amt);

        // Validation for invalid amount
        if (amt <= 0)
        {
            printf("Invalid amount entered! Please enter a valid amount.\n");
            checkPoint();
        }

        // Check daily limit
        if (amt + current_session_user->dailySpend > current_session_user->dailyLimit)
        {
            printf("Daily Transaction Limit Exceeded! You can perform new transactions tomorrow!\n");
            checkPoint();
        }

        // Check for sufficient funds
        if (amt > current_session_user->balance)
        {
            printf("Insufficient Funds!\n");
            checkPoint();
        }

        // Find the receiver's account using username
        struct Account *receiver = findUserusingUsername(trusername);
        if (receiver == NULL)
        {
            printf("Invalid Username!\n");
            checkPoint();
        }

        // Confirmation of transfer
        printf("You are transferring Rs. %.2f to %s\n", amt, receiver->name);
        proceedOrCancel();
        // Debit money from sender's account
        current_session_user->balance -= amt;
        current_session_user->dailySpend += amt;

        // Credit money to receiver's account
        receiver->balance += amt;

        // Generate a single transaction ID for both the sender and receiver logs
        int transactionID = updateTransactionId();

        // Log the transactions with the same transaction ID
        logTransaction(receiver->accountNo, current_session_user->accountNo, amt, "Transfer", transactionID);

        // Update both users in the database
        updateUserInDB(current_session_user); // Update the sender's balance in the database
        updateUserInDB(receiver);             // Update the receiver's balance in the database

        printf("Transaction Successful!!\nYour Updated Balance: Rs. %.2f\n", current_session_user->balance);
        checkPoint();
        break;
    }

    default:
        printf("Invalid Choice! Returning to Home...\n");
        Home();
        break;
    }

    // After transaction is complete, return to home or logout
    checkPoint();
}

void withdrawal()
{
    system("cls");
    float amt;
    printf("--- Withdraw Money from ATM ---\n");
    printf("Enter Amount to Withdraw: ");
    scanf("%f", &amt);
    // Validate the amount entered
    if (amt <= 0)
    {
        printf("Invalid amount entered! Please enter a valid amount.\n");
        Home();
        return;
    }
    // Update and check daily transaction limits
    updateLimit(current_session_user);
    if (amt + current_session_user->dailySpend > current_session_user->dailyLimit)
    {
        printf("Daily Transaction Limit Exceeded! You can perform new Transactions tomorrow!\n");
        checkPoint();
        return;
    }
    // Check if there's enough balance in the account
    if (amt > current_session_user->balance)
    {
        printf("Insufficient balance! Transaction cannot proceed.\n");
        checkPoint();
        return;
    }
    // Confirmation for proceeding with withdrawal
    printf("You are withdrawing Rs. %.2f from the ATM.\n", amt);
    proceedOrCancel();
    // Deduct amount from the user's balance
    printf("Debiting money from your account...\n");
    current_session_user->balance -= amt;
    current_session_user->dailySpend += amt;
    int trid = updateTransactionId();
    // Log the transaction as a withdrawal (ATM transaction: self-account to cash)
    logTransaction(-1, current_session_user->accountNo, amt, "Withdrawal", trid);
    updateUserInDB(current_session_user);
    sleep(1);
    printf("Please collect cash from Counter %d\n", rand() % 4 + 1); // Simulate ATM cash counter

    // Option to go back to the Home page or logout
    checkPoint();
}

// Prints recent 3 transactions of the user
void statement()
{
    system("cls");
    // Initialize current session user's transactions to NULL
    current_session_user->transactions = NULL;

    // Open the transactions CSV file for reading
    FILE *fp = fopen("transactions.csv", "r");
    if (fp == NULL)
    {
        printf("Error: Could not open transactions file\n");
        return;
    }

    char buffer[512];
    int prevtransid;
    // Read through the file to find transactions related to the current user
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        struct Transaction tempTrans; // Temporary transaction to hold data

        // Parse the line into transaction details
        char *token = strtok(buffer, ",");
        tempTrans.transactionID = atoi(token);

        token = strtok(NULL, ",");
        tempTrans.toAccNo = atol(token);

        token = strtok(NULL, ",");
        tempTrans.fromAccNo = atol(token);

        token = strtok(NULL, ",");
        strcpy(tempTrans.timestamp, token);

        token = strtok(NULL, ",");
        tempTrans.amount = atof(token);

        token = strtok(NULL, "\n"); // Capture remaining data as trans_type
        strcpy(tempTrans.trans_type, token);

        // Check the type of transaction and add accordingly
        if (tempTrans.fromAccNo == -1 && tempTrans.toAccNo == current_session_user->accountNo)
        { // This indicates a deposit
            current_session_user->transactions = linearAdd(tempTrans, current_session_user->transactions);
        }
        else if (tempTrans.toAccNo == -1 && tempTrans.fromAccNo == current_session_user->accountNo)
        { // This indicates a withdrawal
            current_session_user->transactions = linearAdd(tempTrans, current_session_user->transactions);
        }

        else
        {
            if (tempTrans.toAccNo == current_session_user->accountNo)
            { // Current user received money
                struct Transaction receivedTrans = tempTrans;
                strcpy(receivedTrans.trans_type, "Received"); // Change the type for received
                current_session_user->transactions = linearAdd(receivedTrans, current_session_user->transactions);
            }
            else if (tempTrans.fromAccNo == current_session_user->accountNo)
            {
                struct Transaction transferTrans = tempTrans;
                strcpy(transferTrans.trans_type, "Transfer"); // Change the type for transfer
                current_session_user->transactions = linearAdd(transferTrans, current_session_user->transactions);
            }
        }
    }

    fclose(fp); // Close the transactions file

    // Count the transactions
    int count = countTransactions(current_session_user->transactions);
    printf("--- Last %d Transactions for %s ---\n", count < 3 ? count : 3, current_session_user->username);

    // Print the last 3 transactions in reverse order
    printTransactionsInReverse(current_session_user->transactions, (count < 3 ? count : 3));
    checkPoint();
}

void settings()
{
    system("cls");
    char option;
    char ch;
    printf("--- Account Settings ---\n");
    printf("1. Change Password\n");
    printf("2. Set/Modify Daily Transaction Limit\n");
    printf("3. View Account Details\n");
    printf("4. Close Account\n");
    printf("5. Back to Home\n");
    option = _getch();
    switch (option)
    {
    case '1':
        changePassword();
        break;
    case '2': // Transaction Limit
        system("cls");
        printf("--- Update Transaction Limit ---\n");
        printf("Current Daily Transaction Limit: %.2f\n", current_session_user->dailyLimit);
        printf("Do you want to update this Limit ? (y/n) :");
        scanf(" %c", &ch);
        if (ch == 'y' || ch == 'Y')
        {
            float newLimit;
            printf("Enter New Limit: ");
            scanf("%f", &newLimit);
            printf("Do you want to apply for updating CDTL from Rs. %.2f to Rs. %.2f ?: (y/n): ", current_session_user->dailyLimit, newLimit);
            scanf(" %c", &ch);
            if (ch == 'y' || ch == 'Y')
            {
                printf("Waiting for Bank Approval!..\n");
                sleep(1);
                if (newLimit > 75000)
                {
                    printf("Request Not Approved!\nReason: Max Limit Rs. 75000/- as per RBI Guidelines.\n");
                }
                else if (newLimit <= 0)
                {
                    printf("Request Not Approved!\nReason: Invalid Limit (Zero or Negative).\n");
                }
                else
                {
                    current_session_user->dailyLimit = newLimit;
                    printf("Request Approved!\n");
                    updateUserInDB(current_session_user);
                    printf("Your new Daily Limit is: Rs. %.2f\n", current_session_user->dailyLimit);
                }
                checkPoint();
                settings();
            }
            else
            {
                printf("Request Cancelled by User!\nRedirecting to Settings..\n");
                sleep(1);
                settings();
            }
        }
        else
        {
            printf("Request Cancelled by User!\nRedirecting to Settings..\n");
            settings();
        }
        break;
    case '3':
        system("cls");
        printf("--- Account Details ---\n");
        printf("- Username: %s\n- Account No. : %ld\n- Account Holder Name: %s\n- Branch Name: %s\n- IFSC Code: %s\n- Account Opened on: %s\n- Account Type: %s\n- Current Balance: %.2f\n", current_session_user->username, current_session_user->accountNo, current_session_user->name, current_session_user->branch, current_session_user->ifsc, current_session_user->accCreatedOn, current_session_user->accountType, current_session_user->balance);
        checkPoint();
        settings();
        break;
    case '4':
        deleteAccount();
        break;
    case '5':
        Home();
        break;
    default:
        printf("Invalid Operation\n");
        break;
    }
}

void changePassword()
{   
    system("cls");
    printf("--- Password Changer ---\n");
    char oldpass[25];
    char newpass[25];
    char newpassr[25];
    fflush(stdin);

    printf("Enter OLD Password: ");
    fgets(oldpass, 25, stdin);
    oldpass[strcspn(oldpass, "\n")] = '\0';

    // Verify old password
    if (strcmp(oldpass, current_session_user->password) == 0)
    {
        printf("Enter NEW Password (at least 6 characters): ");
        fgets(newpass, 25, stdin);
        newpass[strcspn(newpass, "\n")] = '\0';

        // Check password length
        if (strlen(newpass) < 6)
        {
            printf("Password too short! Please enter at least 6 characters.\n");
            changePassword();
            return;
        }
        printf("Re-enter NEW Password: ");
        fgets(newpassr, 25, stdin);
        newpassr[strcspn(newpassr, "\n")] = '\0'; // Remove newline character

        sleep(1); // Simulate time for processing

        // Check if the new password matches the old one
        if (strcmp(oldpass, newpass) == 0)
        {
            printf("New Password cannot be the same as the Old Password.\n");
            changePassword();
        }
        // Check if the re-entered password matches
        else if (strcmp(newpass, newpassr) == 0)
        {
            strcpy(current_session_user->password, newpass);
            printf("Password Changed Successfully!\n");
            updateUserInDB(current_session_user);
        }
        else
        {
            printf("Passwords DO NOT Match.\n");
            changePassword();
        }
        settings();
    }
    else
    {
        printf("Wrong Password Entered.\n");
        settings();
    }

    // Clear sensitive data from memory
    memset(oldpass, 0, sizeof(oldpass));
    memset(newpass, 0, sizeof(newpass));
    memset(newpassr, 0, sizeof(newpassr));
}

void deleteAccount()
{
    system("cls");
    char ch;
    printf("Are you sure to Delete your Account? (y/n): ");
    scanf(" %c", &ch);

    if (ch == 'y' || ch == 'Y')
    {
        // Open the database file for reading
        FILE *fp = fopen("db.csv", "r");
        if (fp == NULL)
        {
            printf("Error: Could not open db.csv file for reading\n");
            return;
        }

        // Create a temporary file to store updated data
        FILE *temp = fopen("temp.csv", "w");
        if (temp == NULL)
        {
            printf("Error: Could not open temp.csv file for writing\n");
            fclose(fp);
            return;
        }

        char buffer[512];
        int userFound = 0;

        // Loop through each line in the original db.csv file
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            char username[25];

            // Extract the username from the current line
            sscanf(buffer, "%[^,]", username);

            // If the line is not the current session user, write it to the temp file
            if (strcmp(username, current_session_user->username) != 0)
            {
                fprintf(temp, "%s", buffer); // Copy line to temp file
            }
            else
            {
                userFound = 1; // Flag that user was found and will be deleted
            }
        }

        fclose(fp);
        fclose(temp);

        if (userFound)
        {
            // Delete the original db.csv file and rename temp.csv to db.csv
            remove("db.csv");
            rename("temp.csv", "db.csv");

            printf("Account Deleted Successfully! You can collect the cash from Counter %d.\n", rand() % 4 + 1);
            sleep(1);
            printf("Redirecting to the Welcome Page...\n");
            sleep(1);

            current_session_user = NULL; // Log the user out
            WelcomePage();               // Go back to the welcome page
        }
        else
        {
            // If the user was not found in the file
            printf("Error: Could not find the user in the database.\n");
            remove("temp.csv"); // Delete the temporary file
            settings();         // Go back to settings
        }
    }
    else
    {
        // If user cancels the operation
        printf("Operation Cancelled by User..\n");
        settings(); // Return to settings
    }
}

void logout()
{
    char ch;
    printf("Are you sure you want to end this session ? (y/n): ");
    scanf(" %c", &ch);
    if (ch == 'y' || ch == 'Y')
    {
        current_session_user = NULL;
        printf("Logged Out Successfully!!\nRedirecting to the Welcome Page...\n");
        sleep(1);
        WelcomePage();
    }
}

void formatDBCSV()
{
    // Open the original db.csv file for reading
    FILE *fp = fopen("db.csv", "r");
    if (fp == NULL)
    {
        printf("Error: Could not open db.csv for reading.\n");
        return;
    }

    // Create a temporary file for writing
    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL)
    {
        printf("Error: Could not open temp.csv for writing.\n");
        fclose(fp);
        return;
    }

    char buffer[512];

    // Read each line from the original file
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Check if the line is not empty (contains at least one non-whitespace character)
        if (buffer[0] != '\n' && buffer[0] != '\r')
        {
            // Write the non-empty line to the temporary file
            fprintf(temp, "%s", buffer);
        }
    }

    // Close both files
    fclose(fp);
    fclose(temp);

    // Replace the original db.csv file with the formatted temp.csv file
    remove("db.csv");
    rename("temp.csv", "db.csv");
}

int main()
{
    WelcomePage();
    formatDBCSV();
    return 0;
}