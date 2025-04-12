# Bank
--- 

This programm illustrate a bank, where the main focus in on memory allocations and leaks.

The programm supports the following back operations:

1. create new account.

    Given an account number of an account who's not already exists in the bank the programm will create the account with a given account's Holder name.

2. delete existing accoung.

    Given an account number of an existing account the programm will delete the account.

3. updating existing account.

    Given an account number of an existing account the programm will update the account's holder name.

4. withdraw and deposite form and to an account.

5. Transferring - excuting multiple transactions.

   example: ```30-25:40, 20-60,100``` is a transfer of 40 from account 30 to 25 and 100 from account 20 to 60;
   this option includes a special mechanism assures that the transfers was made only if all of them are a valid transfers.

7. View an account.

   Option to view the account owner details including the past transactions made on the account.

The programm use's the concept of structures in C, linked list and memory allocations (the programm has zero leaks)

Running demonstration:

```

Welcome to the bank!
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
1
Enter account number:
100
Enter account holder:
Albert Einstein ֿAccount created successfully
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
1
Enter account number:
200
Enter account holder:
Audrey Hepburn ֿAccount created successfully
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
4
Enter account number:
100
Would you like to deposit or withdraw money?
deposit
How much money would you like to deposit?
1000
Money deposited successfully; your new balance is 1000
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
4
Enter account number:
100
Would you like to deposit or withdraw money?
idk
Invalid action
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
4
Enter account number:
200
Would you like to deposit or withdraw money?
withdraw
How much money would you like to withdraw?
50
Not enough money
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
5
Enter instructions:
100-200:300,200-100:50
Instructions executed successfully
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
5
Enter instructions:
30-45:50
Invalid instruction
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
6
Enter account number:
300
Account not found
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
6
Enter account number:
100
Account #100 (Albert Einstein)
Balance: 750
Transactions:
Deposited 1000
300 to 200
50 from 200
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
6
Enter account number:
200
Account #200 (Audrey Hepburn)
Balance: 250
Transactions:
300 from 100
50 to 100
Please select an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
0

```

 

