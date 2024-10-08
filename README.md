# Bank Transaction System in C
created by Aayush Beura


This project is a **Bank Transaction System** developed in **C**, designed to simulate basic banking operations such as deposits, withdrawals, fund transfers, and transaction history. Initially, the project used **run-time storage** but was later enhanced to store data persistently using **File I/O**, making it more practical and scalable for real-world use.

## Features

- **User Account Management**: 
  - Create new accounts with basic details.
  - View account information including balance and account type.
  
- **ATM Transactions**: 
  - Deposit money into an account.
  - Withdraw money from an account.
  
- **Fund Transfer**:
  - Transfer money between accounts.
  - Log each transaction with details.

- **Transaction Logging**:
  - Track and store every transaction (deposits, withdrawals, transfers) in a **CSV file** (`transactions.csv`).
  - View the last three transactions for any user.
  
- **File I/O for Database Management**:
  - User accounts are stored in a CSV file (`db.csv`) for persistent storage.
  - Transactions are logged in a CSV file (`transactions.csv`) to track transaction histories.
  
- **Modular Codebase**:
  - Separated code into **three header files** for better organization and readability.

## Files

- `db.csv` - Stores user account details such as username, balance, account type, etc.
- `transactions.csv` - Stores all transactions, including deposits, withdrawals, and transfers.
- **Header Files**:
  - `accounts.h` - Functions and structures related to account management.
  - `transactions.h` - Functions and structures related to transaction handling.
  - `format.h` - Helper functions for formatting user inputs and outputs.

## How It Works

1. **Account Management**:
   - Create a user account with details like username, password, balance, and account type.
   - Each user account is stored in the `db.csv` file.

2. **Transactions**:
   - Users can perform transactions like deposit, withdrawal, or transfer.
   - Each transaction is logged in `transactions.csv` with details like transaction ID, timestamp, amount, and type (Deposit, Withdrawal, Transfer).
   
3. **View Transaction History**:
   - The system retrieves and displays the last three transactions for the logged-in user.

## How to Run

1. **Clone the repository**:
   ```bash
   git clone https://github.com/username/BankTransactionSystem.git
   cd BankTransactionSystem
   ```

2. **Compile the program**:
   ```bash
   gcc main.c -o bank_system
   ```

3. **Run the program**:
   ```bash
   ./bank_system
   ```

## Future Enhancements
- Implementing a **graphical user interface (GUI)** for ease of use.
- Introducing **multi-threading** for concurrent transactions.
- Introducing **OTP_BasedLogin** for security purposes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
