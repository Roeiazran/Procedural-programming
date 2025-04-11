#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE 10
#define ZERO_ACCOUNT 0

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct Account {
    unsigned int accountNumber;
    char *accountHolder;
    int balance;
} Account;

typedef struct Transaction {
    unsigned int fromAccount;
    unsigned int toAccount;
    int amount;
} Transaction;

typedef struct Bank {
    Node *accounts;
    Node *transactions;
} Bank;


void printMenu(void);
int handleUserChoice(char choice, Bank ** bank);
void printAccounts(Node * account);
void freeBank(Bank ** bank);
void printTransactions(Node * transaction);
void reverseList(Node ** list);
Account * makeAccount(unsigned int accountNumber, char * holderName, Bank ** bank);
Transaction * makeTransaction(int amount, unsigned int from, unsigned int to, Bank ** bank);
void addNewAccount(Node ** accounts, Account * newAccount, Bank ** bank);
void addNewTransaction(Bank ** bank, Node ** transactions,  Transaction * transaction);
void freeListOfTransaction(Node * transactions);

/**
 @brief Main function to initalize bank and run scanning the user choice and printing the menu
 @return 0
 */
int main() {
    
    //initalize bank
    Bank * bank = (Bank*)malloc(sizeof(Bank));
    
    //if no space for the bank
    if (bank == NULL) {
        freeBank(&bank);
    }

    char choice;
    //run printing the menu and scanning the user choice
    while (1) {
        //print the menu
        printMenu();
        //get user choice
        scanf("%c", &choice);
        scanf("%*c");
        //satus is 0 in case of user choice is 0 other wise handle the users choice
        handleUserChoice(choice, &bank);
    }
    return 0;
}

/**
 @brief function to recieve account number from the user
 @return the account number entered by the user
 */
unsigned int getAccountNumberInput(void) {
    //initialize and scanf from the user
    unsigned int accountNumber;
    printf("Enter account number:\n");
    scanf("%u", &accountNumber);
    //clear the buffer from '\n' and return the acconut number
    scanf("%*c");
    return accountNumber;
}

/**
 @brief function to print account not found
 */
void printAccountNotFound(void) {
    printf("Account not found\n");
}

/**
 @brief function to free all allocated memory in the program
 the function will iterate over the bank transaction and free all memory
 @param bank the main bank
 */
void freeBank(Bank ** bank) {

    //initalize temp so we can free without loosing the pointer
    Node *temp;
    
    //free the transactions
    freeListOfTransaction((*bank)->transactions);

    //runs for every account
    for (Node * account = (*bank)->accounts; account != NULL;) {
        //free holder name
        free(((Account *)account->data)->accountHolder);
        //free the data
        free(account->data);
        //save next so we wont loos access to it
        temp = account->next;
        //free the account itself
        free(account);
        //point to next transaction
        account = temp;
    }
    //free the main allocated bank and exit
    free(*bank);
    exit(1);
}

/**
 @brief function to get account data by account number if a given account already exists on the system
 @param accounts our bank accounts
 @param check the account number to check
 @return account  if account with the given number was found or NULL otherwise
 */
Node * getAccountByNumber(Node * accounts, unsigned int check) {
    
    //run for all the account in the system
    while (accounts != NULL) {
        //checks for account with account number matching the check
        if (((Account*)accounts->data)->accountNumber == check) {
            return accounts;
        }
        //advance pointer
        accounts = accounts->next;
    }
    //no account with that number was found
    return NULL;
}


/**
 @brief function to get from the user unlimited size string and retuning pointer to it
 @param customString custom input string to print to the user
 @param bank the main bank to free incase of memory allocation faliure
 @return pointer of type character that holds the start of the formed string
 */
char *getInfiniteString(char * customString, Bank ** bank) {
    
    //clear the buffer from unwanted characters
    char accountHolderName;
    printf("%s\n", customString);
    scanf("%c", &accountHolderName);

    //intialize index to 0
    int index = 0;
    //allocates memory for the new string
    char *accountHolderStr = (char *)malloc(2 * sizeof(char));
    
    //if failed to allocate return
    if (accountHolderStr == NULL) {
        freeBank(bank);
    }
    //run all while the buffer isn't empty
    while (accountHolderName != '\n') {
        //set the string at index to the current char
        accountHolderStr[index] = accountHolderName;
        
        //increment index
        index++;
        //reallocate the memory for the next character
        char * temp = (char*)realloc(accountHolderStr, sizeof(char) * (index + 2));
        if (temp == NULL) {
            free(accountHolderStr);
            freeBank(bank);
        }
        //set account holder to the reallocated string
        accountHolderStr = temp;
        //get the next character from the buffer
        scanf("%c", &accountHolderName);
    }
    //set end of string
    accountHolderStr[index] = '\0';
    
    //return the formed string
    return accountHolderStr;
}

/**
 @brief function to add new account to the bank system
 the function will allocate new account and puts it at the head  accounts list
 @param accounts head of the account list
 */
void addNewAccount(Node ** accounts, Account * newAccount, Bank ** bank) {
    
    //allocate memory for a new account
    Node * temp = (Node*)malloc(sizeof(Node));
    
    //check if allocation failed
    if (temp == NULL) {
        freeBank(bank);
    }
    
    //initialize data to the new account address that was send to the function
    temp->data = (Account*)newAccount;
    
    //point next to the last account on the system
    temp->next = *accounts;
    
    //point the head of accounts to the new account
    *accounts = temp;
}

/**
 @brief function to get accunt structure with given details
 @param accountNumber the number of the new account
 @param holderName users holder name
 @param bank the main bank
 @return pointer to Account structure
 */
Account * makeAccount(unsigned int accountNumber, char * holderName, Bank ** bank) {
    //allocate memory for the new account
    Account * account = (Account*)malloc(sizeof(Account));
    
    //free the bank if allocation failed
    if (account == NULL) {
        freeBank(bank);
    }
    
    //initalize accout detalis and return it
    account->accountNumber = accountNumber;
    account->accountHolder = holderName;
    account->balance = 0;
    return account;
}

/**
 @brief function to get the user new account details
 the function will allocate new memory for the account and initialize its data with the data collected from the user
 @param bank  main bank
 */
void getAccountDetails(Bank ** bank) {
    //gets the account number from user
    unsigned int accountNumber = getAccountNumberInput();
    
    //check if account is 0 or if its already exsits
    if (accountNumber == ZERO_ACCOUNT || getAccountByNumber((*bank)->accounts, accountNumber)) {
        printf("Account number already exists\n");
        return;
    }

    //get from the user its holderName
    char * holderName = getInfiniteString("Enter account holder:", bank);

    //make the account with the user detalis
    Account * account = makeAccount(accountNumber, holderName, bank);

    //add account to the list and print
    addNewAccount(&((*bank)->accounts), account, bank);
    printf("Account created successfully\n");
}
/**
 @brief function to free a single account node
 @param account to be freed
 */
void freeSingleAccount(Node ** account) {
    //free the account string pointer and data
    free(((Account*)(*account)->data)->accountHolder);
    free((Account*)(*account)->data);
    free(*account);
}


/**
 @brief function to delete account from the system
 the function will recieve accouts and get  the account number and will delete it if exists
 @param accounts the bank accounts
 */
void deleteAccount(Node ** accounts) {
    //gets the account number from user
    unsigned int userAccNum = getAccountNumberInput();
    //initialize ptr to the curr head of accounts
    Node * curr = *accounts;
    Node * head = *accounts;
    Node * temp;
    while (curr != NULL) {
        //if we found the account in the head of the list advamce account to its next
        if (((Account*)curr->data)->accountNumber == userAccNum) {
            temp = curr->next;
            //free the deleted Node
            freeSingleAccount(&curr);
            //assignes account to the deleted Node next
            *accounts = temp;
            //prints and return
            printf("Account deleted successfully\n");
            return;
        }
        //else for the next account chenge pointer values
        if (((Account*)curr->next) != NULL && ((Account*)curr->next->data)->accountNumber == userAccNum) {
            //store temp to be freed later
            temp = curr->next;
            //set curr next to its next next
            curr->next = curr->next->next;
            //free temp
            freeSingleAccount(&temp);
            //restore account to the previuos head
            *accounts = head;
            //prints and return
            printf("Account deleted successfully\n");
            return;
        }
        //advance curr to its next and advance main pointer to its next
        curr = curr->next;
        (*accounts) = (*accounts)->next;
    }
    //restore account to the previuos head
    *accounts = head;
    //if account wasn't found print and return
    printAccountNotFound();
}

/**
 @brief function to update account holder name
 the function will validate the existence of the account and will recieve new holder name from the user and update the account
 @param bank the bank
 */
void updateAccount(Bank ** bank) {
    //gets the account number from user and the account from the list of account
    unsigned int accountNumber = getAccountNumberInput();
    Node * accounts = (*bank)->accounts;
    Node * account = getAccountByNumber(accounts, accountNumber);
    //checks for account not exists
    if (account != NULL) {
        //gets the new name and frees prevoius name
        char * holderName = getInfiniteString("Enter account holder:", bank);
        free(((Account*)account->data)->accountHolder);
        //assing new name
        ((Account*)account->data)->accountHolder = holderName;
        return;
    }
    
    //prints not found
    printAccountNotFound();
}

/**
 @brief function to make a single transaction
 the function get the transaction detalis and allocating new transaction
 @param amount transaction
 @param from transaction
 @param to transaction
 @param bank the main bank
 @return single Transaction sturcture
 */
Transaction * makeTransaction(int amount, unsigned int from, unsigned int to, Bank ** bank) {
    Transaction * transaction = (Transaction*)malloc(sizeof(Transaction));
    //check for allocation faliure and frees the bank
    if (transaction == NULL) {
        freeBank(bank);
    }
    //assing transaction details and return it
    transaction->amount = amount;
    transaction->fromAccount = from;
    transaction->toAccount = to;
    return transaction;
}
/**
 @brief function to add new transaction to the end of transactions list
 the function recursively advance transactions head to the end and add new transaction to it
 @param bank the main bank
 @param transactions of the bank
 @param transaction structure to add to transactions
 */
void addNewTransaction(Bank ** bank, Node ** transactions,  Transaction * transaction) {
    
    //exit condition
    if (*transactions == NULL) {
        //allocate memory for a new transaction
        Node * temp = (Node*)malloc(sizeof(Node));
        
        //check if allocation failed
        if (temp == NULL) {
            //free all memory
            freeBank(bank);
        }
        
        //initialize data to the new transaction address that was send to the function
        temp->data = (Transaction*)transaction;
        
        //point last transaction to NULL
        temp->next = NULL;
        *transactions = temp;
        return;
    }
    //call itself next
    addNewTransaction(bank, &(*transactions)->next , transaction);
}

/**
 @brief function to check strtol output
 the function will get strtol endptr and validate the amount
 @param amount inserted by the user
 @param endptr strtol endptr
 @return 1 if the amount is 0 or positive and its a valid number
 */
int isValidInformation(int amount, char * endptr) {
    //check if endptr point to char or the amount if negative and return
    if (*endptr != '\0' || amount <= 0) {
        return 0;
    }
    return 1;
}

/**
 @brief function to handle user withdraw actions
 the function will validate the user inserted data and withdraw the money from its account
 @param userAccount the user account
 @param amount the user inserted amount
 @param endptr strtol endptr to validate the amount
 @return 1 if the money withdraned  successfully and 0 otherwise
 */
int handleWithdraw(Node * userAccount, int amount, char * endptr) {
    //validate the user amount and print error
    if (!isValidInformation(amount, endptr)) {
        printf("Invalid amount");
        return 0;
    }
    //check for user balance
    if (((Account*)userAccount->data)->balance < amount) {
        printf("Not enough money\n");
        return 0;
    }
    //updating user account balace and return
    ((Account*)userAccount->data)->balance -= amount;
    return 1;
}

/**
 @brief function to handle deposit action from the user
 the function will validate the user information and deposit money to its account
 @param userAccount the user account
 @param amount transaction
 @param endptr the endptr of strtol to validate the amount
 @return 1 if operation succeeded 0 otherwise
 */
int handleDeposit(Node * userAccount, int amount, char * endptr) {
    //checks for input validity and prints
    if (!isValidInformation(amount, endptr)) {
        printf("Invalid amount\n");
        return 0;
    }
    //updating the user account and returns success
    ((Account*)userAccount->data)->balance += amount;
    return 1;
}
/**
 @brief function to add transaction to a gievn transactions list
 the function works as a middleman making adding transaction to the list more approachable
 @param from  transcation
 @param to transactions
 @param amount transaction
 @param bank the main bank
 @param transactions list of transactions
 */
void addTransactionToList(unsigned int from, unsigned int to, int amount, Bank ** bank,\
                          Node ** transactions) {
    //makes single transaction and adding it to transactions
    Transaction * transaction = makeTransaction(amount, from, to, bank);
    addNewTransaction(bank, transactions, transaction);
}

/**
 @brief function to make deposit or withdraw
 the function will handle each case calling the appropriate function to deal with the action
 @param bank bank on the main
 */
void withdrawOrDeposit(Bank ** bank) {
    //init consts
    const char withdraw[] = "withdraw";
    const char deposit[] = "deposit";
    
    //gets the account number from user and the user account
    unsigned int accountNumber = getAccountNumberInput();
    Node * userAccount = getAccountByNumber((*bank)->accounts, accountNumber);
    
    //if no user was found print
    if (userAccount == NULL) {
        printAccountNotFound();
        return;
    }
    
    //gets the user choice
    char *str = getInfiniteString("Would you like to deposit or withdraw money?", bank);
    //init transactions detalis variables
    int amount;
    char * amountStr;
    char * endptr;
    //checks for the users different choices
    if (!strcmp(str, withdraw)) {
        //get the amount and run strtol on it
        amountStr = getInfiniteString("How much money would you like to withdraw?", bank);
        amount = (int)strtol(amountStr, &endptr, BASE);
        
        //handles withdraw and recording the transaction if details are currect
        if (handleWithdraw(userAccount, amount, endptr)) {
            //make transaction from user account to account number 0 and print
            addTransactionToList(accountNumber, ZERO_ACCOUNT, amount, bank, &(*bank)->transactions);
            printf("Money withdrawn successfully; your new balance is %d\n",\
                   ((Account*)userAccount->data)->balance);
        }
        //free allocated memory
        free(amountStr);
        free(str);
        return;
        //check for deposit
    } else if (!strcmp(str, deposit)) {
        //get the amount from the user and run strtol on it
        amountStr = getInfiniteString("How much money would you like to deposit?", bank);
        amount = (int)strtol(amountStr, &endptr, BASE);
        
        //handles deposit and recording the transaction if details are currect
        if (handleDeposit(userAccount, amount, endptr)) {
            //make transaction from account number 0 to user account and print
            addTransactionToList(ZERO_ACCOUNT, accountNumber, amount, bank, &(*bank)->transactions);
            printf("Money deposited successfully; your new balance is %d\n",\
                   ((Account*)userAccount->data)->balance);
        }
        //free allocated memory
        free(amountStr);
        free(str);
        return;
    }
    
    //handles invalid transactions by free allocatoed memory and printing
    free(str);
    printf("Invalid action\n");
    return;
}

/**
 @brief function to get make transactions list from given transactions string
 @param instructionsString  the instructions string
 @param bank the main bank
 @return pointer to the head of the new list
 */
Node * getTransactionsFromString(char * instructionsString, Bank ** bank) {
    
    //initalize transcations variable
    Node * transactions = NULL;
    //initalize tok and gets the first of the string
    char * tok = strtok(instructionsString, "-");
    //initalize endptr to strtol
    char * endptr;
    while (tok != NULL) {
        //gets the from part and run strtol on it
        unsigned int from = (unsigned int) strtol(tok, &endptr, BASE);
        //get the next part
        tok = strtok(0, ":");
        //validate the part and check for unexpected end of string
        if (!isValidInformation(from, endptr) || tok == NULL)  {
            //frees transcations
            free(transactions);
            return NULL;
        }
        //gets the from part and run strtol on it
        unsigned int to = (unsigned int) strtol(tok, &endptr, BASE);
        
        //get the next part
        tok = strtok(0, ",");
        
        //validate the part and check for unexpected end of string
        if (!isValidInformation(to, endptr) || tok == NULL)  {
            free(transactions);
            return NULL;
        }
        
        //gets the from part and run strtol on it
        int amount = (int) strtol(tok, &endptr, BASE);
        
        //skip all commas and jump next
        tok = strtok(0, "[^,]-");
        //validate the part
        if (!isValidInformation(amount, endptr)) {
            free(transactions);
            return NULL;
        }
        
        //edge case
        if (to == from) {
            free(transactions);
            return NULL;
        }
    
        //adds the transaction
        addTransactionToList(from, to, amount, bank, &transactions);
        
        //check for end of string
        if (tok == NULL) {
            return transactions;
        }
    }
    
    //free transactions if the string wasn't valid
    free(transactions);
    return NULL;
}

/**
 @brief function to check if a given character is a digit
 @param ch char to check
 @return 1 if its a digit 0 otherwise
 */
int isPositivedigit(char ch) {
    return ch <= '9' && ch >= '0';
}


/**
 @brief function to stil all positive digits from string
 the function will advance the pointer to the string
 @param str address of the string to advance
 */
void skipDigits(char ** str) {
    //advance string all while ite value is a positive number
    while (isPositivedigit(**str)) {
        (*str)++;
    }
}

/**
 @brief function to validate transactions string
 the function will recursively validate if the string in formated as from-to:amount with no characters that isn't digit
 @param str the string to validate
 @return 1 if string is valid 0 otherwise
 */
int validateTransactionString(char * str){
    //checks for non digit in the begining
    if (!isPositivedigit(*str)) {
        return 0;
    }
    //skips all digits
    skipDigits(&str);
    //checks for the cheracter '-'
    if (!*str || *str != '-') {
        return 0;
    }
    //increment
    str++;
    //skips all digits
    skipDigits(&str);
    //checks for the cheracter ':'
    if (!*str || *str != ':') {
        return 0;
    }
    //increment
    str++;
    //skips all digits
    skipDigits(&str);
    //checks for end of string
    if (!*str) {
        return 1;
    }
    //check for multiple transactions
    if (*str != ',') {
        return 0;
    }
    //if multiple transactions skip the commas
    while (*str == ',') {
        str++;
        //if no transaction after the comma return error
        if (!*str) {
            return 0;
        }
    }
    //if end of string return
    if (!*str) {
        return 1;
    }
    //calls
    return validateTransactionString(str);
}

/**
 @brief function to execute all transactions from given string
 the function will run recursivly executing every commend and on faliure reveser the made transactions
 @param accounts bank accounts list
 @param transactios transactions list to execute
 @return 1 if execution completed 0 otherwise
 */
int executeTransferInstructions(Node * accounts, Node * transactios) {

    //checks for exit condition - no transactions
    if (transactios == NULL) {
        return 1;
    }
    //init transcations details
    unsigned int from = ((Transaction*)transactios->data)->fromAccount;
    unsigned int to = ((Transaction*)transactios->data)->toAccount;
    int amount = ((Transaction*)transactios->data)->amount;
    //init the accounts invloved in the transcation
    Node * fromAccount = getAccountByNumber(accounts, from);
    Node * toAccount = getAccountByNumber(accounts, to);
    //checks for account that does not exists
    if (fromAccount == NULL || toAccount == NULL) {
        return 0;
    }
    //check for balance less than transaction amount
    if (((Account*)fromAccount->data)->balance < amount) {
        return 0;
    }
    //update the account
    ((Account*)fromAccount->data)->balance -= amount;
    ((Account*)toAccount->data)->balance += amount;
    
    //calls the function with next
    int opSucceed = executeTransferInstructions(accounts, transactios->next);

    //checks for operations faliure and undo every operation made so far
    if (!opSucceed) {
        ((Account*)toAccount->data)->balance -= amount;
        ((Account*)fromAccount->data)->balance += amount;
        return 0;
    }
    return 1;
}

/**
 @brief function to concate to lists
 @param list1 the list to cancate to
 @param list2 the list to concate
 */
void concateTwoList(Node ** list1, Node ** list2) {

    //store the head of the list to return to it later
    Node * head = *list1;

    //if the list is empty assign it and return
    if ((*list1) == NULL) {
        (*list1) = (*list2);
        return;
    }

    //advane list1 to its end
    while ((*list1)->next != NULL) {
        (*list1) = (*list1)->next;
    }
    //assing its next to point to list2
    (*list1)->next = *list2;
    //restore its head
    (*list1) = head;
}


/**
 @brief function to free list of transactions
 the function will free the date from the transaction and also free the transactions list
 @param transactions to be freed
 */
void freeListOfTransaction(Node * transactions) {
    
    Node * temp;
    //runs for every transaction
    for (Node * transaction = transactions; transaction != NULL;) {
        //free the data
        free(transaction->data);
        //save transaction next to not lost access to it
        temp = transaction->next;
        //free the transaction itself
        free(transaction);
        //point to next transaction
        transaction = temp;
    }
}


/**
 @brief function to exec multyple transactions
 the function stored seperate transactions list and execute them and conact them to the bank
 @param bank the bank on the main
 @param insructionsString the instructions inserted by the user
 */
void makeInstructionsList(Bank ** bank, char * insructionsString) {
    //initialize transactions from the string
    Node * transcations = getTransactionsFromString(insructionsString, bank);
    
    //intializee operation flag to print invalid instructions
    int operationResult = 0;
    //runs on every transaction
    if (transcations != NULL) {
        //tryies execute the transactions and store the result of the opeartiion
        operationResult = executeTransferInstructions(((*bank)->accounts), transcations);
        //checks for operation faliure and concate and print on success
        if (operationResult) {
            concateTwoList(&((*bank)->transactions), &transcations);
            printf("Instructions executed successfully\n");
        }
    }
    //free and prints if the operation falied
    if (!operationResult) {
        freeListOfTransaction(transcations);
        printf("Invalid instructions\n");
        return;
    }
    
}
/**
 @brief function to get instruction string frmo the user validate it partially and making the transactions
 @param bank bank on the main
 */
void getInstructionsString(Bank ** bank) {
    //initialize the instruction string from the user
    char * str = getInfiniteString("Enter instructions:", bank);
    
    //validating the string for its structure i.e from-to:amount
    if (!validateTransactionString(str)) {
        //frees str and prints error
        free(str);
        printf("Invalid instructions\n");
        return;
    }
    //makes the transaction and frees str
    makeInstructionsList(bank, str);
    free(str);
}


/**
 @brief function to print the user transactions
 @param transactions the transactions made on the bank
 @param accountNum user account number
 */
void printUserTransactions(Node * transactions, unsigned int accountNum) {
    //initalize
    unsigned int from;
    unsigned int to;
    int amount;
    //initalize flag to print "no transactions"
    int hasTransactions = 0;
    //runs on ecery transaction on bank
    while (transactions != NULL) {
        //assign the variable with the transaction detalis
        from = ((Transaction *)transactions->data)->fromAccount;
        to = ((Transaction *)transactions->data)->toAccount;
        amount = ((Transaction *)transactions->data)->amount;
        
        //checks if the user account wes part of the transaction
        if (from == accountNum || to == accountNum) {
            //prints transcation line once
            if (transactions && !hasTransactions) {
                printf("Transactions:\n");
                hasTransactions = 1;
            }
            //check for the nature of the transactions, from 0 - deposit
            if (from == ZERO_ACCOUNT) {
                printf("Deposited %d\n", amount);
                //to 0 - withdraw
            } else if (to == ZERO_ACCOUNT) {
                printf("Withdrew %d\n", amount);
                //deposited to the account from another account
            } else if (to == accountNum){
                printf("%d from %u\n",amount, from);
                //user deposited to another account
            } else {
                printf("%d to %u\n", amount, to);
            }
        }
        //advancing transactions to next transaction
        transactions = transactions->next;
    }
    //check for no transaction for the user and prints
    if (!hasTransactions) {
        printf("No transactions\n");
    }
}

/**
 @brief function that prints one account detalis
 the function will get the account number from user and print its detalis
 @param bank bank stores in the maim
 */
void viewAccount(Bank ** bank) {
    //get the user account number and fetch the user account
    unsigned int accountNum = getAccountNumberInput();
    Node * account = getAccountByNumber((*bank)->accounts, accountNum);
    
    //check for account not found and prints error msg
    if (account == NULL) {
        printAccountNotFound();
        return;
    }
    
    //initialize account number for better visibility and print the user account details
    unsigned int accountNumber = ((Account*)account->data)->accountNumber;
    printf("Account #%u (%s)\nBalance: %d\n", accountNumber,\
           ((Account*)account->data)->accountHolder, ((Account*)account->data)->balance);
   
    //initialize transaction from the bank and calls prints it
    Node * transactions = (*bank)->transactions;
    printUserTransactions(transactions, accountNumber);
}

/**
 @brief function to handle the user input for the menu choice
 @param choice the user entered choice
 @param bank  bank stores in the main
 @return 0 in case the player want to exit 1 if not
 */
int handleUserChoice(char choice, Bank ** bank) {

    //check for every possible choice case
    switch (choice) {
        case '0':
            freeBank(bank);
            break;
        case '1':
            getAccountDetails(bank);
            break;
        case '2':
            deleteAccount(&((*bank)->accounts));
            break;
        case '3':
            updateAccount(bank);
            break;
        case '4':
            withdrawOrDeposit(bank);
            break;
        case '5':
            getInstructionsString(bank);
            break;
        case '6':
            viewAccount(bank);
            break;
        default:
            printf("Invalid option\n");
    }
    
    //return 1 to keep the main's loop
    return 1;
}

/**
 @brief function to print the menu
 */
void printMenu() {
    printf("Choose an option:\n\
0. Exit\n\
1. Create account\n\
2. Delete account\n\
3. Update account\n\
4. Deposit / Withdraw money\n\
5. Transferring\n\
6. View account\n");
}


