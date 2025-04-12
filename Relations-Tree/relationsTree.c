#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INIT_DEG 0

typedef struct Node {
    void * data;
    struct Node * next;
} Node;

typedef struct Person {
    char * name;
    int age;
    struct Person * merriedTo;
    Node * children;
    struct Person * parent;
    int visited;
} Person;

typedef struct Persons {
    Node * head;
} Persons;


void printMenu(void);
int handleUserChoice(char, Persons *);
void exitProgram(Node * head);
Person * getPerson(Node * head, char *name);
void unVisitTree(Node * head);
void exitProgram(Node * head);
void incrementAge(Node * head, unsigned int amount);
void getIncrementNumber(Node * head);
void findCommonParent(Person *, char ***, unsigned int ,\
                      unsigned int , unsigned int *, Node * persons);
void getRelativeFromDegree(Persons * persons);
void getPersonsCount(Node * head);
Person * makePerson(Persons * persons, char * name, int age, Person * parent);
void addNewPerson(Persons * persons, Node ** head, Person * person);
void merryTwoPeps(Person * p1, Person * p2);
int validateMerriege(Person * p1, Person * p2);

/**
 @brief Main function to initalize persons list and run scanning the user choice also prints the program menu
 @return 0
 */
int main() {
    //init choice
    char choice;
    //init persons list
    Persons persons = {NULL};
    
    //run printing the menu and scanning the user choice
    while (1) {
        //print the menu
        printMenu();
        //get user choice
        scanf("%c", &choice);
        //clear buffer
        scanf("%*c");
        //calls to handle the user choice
        handleUserChoice(choice, &persons);
    }
    
    return 0;
}

/**
 @brief function to free a given list from heap
 @param head of the list
 */
void freeList(Node * head) {

    //init
    Node * temp;
    Person * p;
    
    //runs through the list
    while (head) {
        //store current person
        p = head->data;
        //edge case to avoid freeing the childrens twice
        if (p->merriedTo) {
            p->merriedTo->children = NULL;
        }
        //free the person name and free its childrens
        free(p->name);
        freeList(p->children);
        //free the allocated place for the person
        free(p);
        //keep next and free allocated place for data
        temp = head->next;
        free(head);
        //advance head to next
        head = temp;
    }
}

/**
 @brief function to traverse the tree and set the visited variable to 0
 @param head of the tree
 */
void unVisitTree(Node * head) {

    //if no head return
    if (head == NULL) {
        return;
    }
    //init person
    Person * person = (Person*)(head->data);

    //unset the person visited
    person->visited = 0;
    //unset the person merried to visited
    if (person->merriedTo) {
        person->merriedTo->visited = 0;
    }
    
    //calls recursively with the person childrens and next
    unVisitTree(person->children);
    unVisitTree(head->next);
}

/**
 @brief function to free all allocated memory and exit the program
 @param head of the list
 */
void exitProgram(Node * head) {
    //call free list and exit
    freeList(head);
    exit(1);
}

/**
 @brief function to get from the user unlimited size string and retuning pointer to it
 @param customString custom input string to print to the user
 @param persons lists
 @return pointer of type character that holds the start of the formed string
 */
char *getInfiniteString(char * customString, Persons * persons) {
    
    //clear the buffer from unwanted characters
    char userInput;
    printf("%s\n", customString);
    scanf("%c", &userInput);

    //intialize index to 0
    int index = 0;
    //allocates memory for the new string
    char *userInputString = (char *)malloc(2 * sizeof(char));
    
    //if failed to allocate return
    if (userInputString == NULL) {
        exitProgram(persons->head);
    }
    //run all while the buffer isn't empty
    while (userInput != '\n') {
        //set the string at index to the current char
        userInputString[index] = userInput;
    
        //increment index
        index++;
        //reallocate the memory for the next character
        char * temp = (char*)realloc(userInputString, sizeof(char) * (index + 2));
        if (temp == NULL) {
            free(userInputString);
            exitProgram(persons->head);
        }
        //set the allocated string to the reallocated string
        userInputString = temp;
        //get the next character from the buffer
        scanf("%c", &userInput);
    }
    //set end of string
    userInputString[index] = '\0';
    
    //return the formed string
    return userInputString;
}

/**
 @brief function to get a person from a given name
 the function will travarse the tree and look for the person name in it
 @param head of the list
 @param nameCheck the name to check
 @return person struct type
 */
Person * getPersonByName(Node * head, char * nameCheck) {
    
    //if no persons on the list return
    if (head == NULL) {
        return NULL;
    }
    //init
    Person * person = (Person*)(head->data);
    Person * wantedPerson = NULL;
    
    //avoid checking the same person twice
    if (person->visited) {
        return NULL;
    }
    //as mentioned avoid checking the same person twice
    person->visited = 1;
    //checks for the name
    if (!strcmp(person->name, nameCheck)) {
        return person;
    }
    //if the wanted person was not found call recursively with the current person childrens
    if (!wantedPerson){
        wantedPerson = getPersonByName(person->children, nameCheck);
    }
    //if still not found call recursively with the current person next
    if (!wantedPerson) {
        wantedPerson = getPersonByName(head->next, nameCheck);
    }
    
    return wantedPerson;
}

/**
 @brief function to make a person from given person details
 @param persons list
 @param name of the new person
 @param age of the new person
 @param parent of the new person
 @return Person struce type
 */
Person * makePerson(Persons * persons, char * name, int age, Person * parent){
    
    //allocate for the new person
    Person * person = (Person*)malloc(sizeof(Person));
    //checks for allocation faliure
    if (!person) {
        exitProgram(persons->head);
    }
    //sets the person details
    person->name = name;
    person->age = age;
    person->visited = 0;
    person->parent = parent;
    //return the person creted
    return person;
}

/**
 @brief function to add person to persons list
 @param persons the persons object to free in case of a faliure of allocation
 @param head of the list
 @param person to add
 */
void addNewPerson(Persons * persons, Node ** head, Person * person) {

    //if no head add on here
    if (*head == NULL) {
        *head = (Node*)malloc(sizeof(Node));
        //checks for memory allocation faliure
        if (!head) {
            exitProgram(persons->head);
        }
        //set the head to new person and next to point null
        (*head)->data = person;
        (*head)->next = NULL;
        return;
    }
    //call recursively with the next person of the list
    addNewPerson(persons, &(*head)->next, person);
}

/**
 @brief function to get unsigned int from the user
 @param customString to print to the user
 @return the user inserted number
 */
unsigned int getNumberInput(char * customString) {
    unsigned int userInput;
    printf("%s\n", customString);
    scanf("%u", &userInput);
    scanf("%*c");
    return userInput;
}


/**
 @brief fucntion to add ont head of a new family
 @param persons list
 */
void addFamilyHead(Persons * persons) {
    //prompt the user to enter name and age
    char * name = getInfiniteString("Enter a name:", persons);
    unsigned int age = getNumberInput("Enter age:");

    //get the person from the persons list
    Person * personPtr = getPerson(persons->head, name);

    //checks for person with that name already exists in persons list
    if (!personPtr) {
        //make and add the person to the list
        Person * person = makePerson(persons, name, age, NULL);
        addNewPerson(persons, &persons->head, person);
        return;
    }

    //prints error and free memory name
    printf("The name is already taken\n");
    free(name);
}

/**
 @brief function to compare two given strings
 @param name1 the first string
 @param name2 the second string
 @return strcmp result activated on the two strings
 */
int compareNames(char * name1, char * name2) {
    return strcmp(name1, name2);
}

/**
 @brief functoin to check any relation between two persons on the list
 @param p1 the person #1
 @param p2 the person #2
 @return 1 if related 0 otherwise
 */
int isRelated(Person * p1, Person * p2) {

    //init
    int related = 0;
    //checks if the persons or the parents are null - exit condition of the recursive call
    if (p1 == NULL || p2 == NULL || p1->parent == NULL || p2->parent == NULL) {
        return 0;
    }
 
    //checks for the names of the parents
    if (!compareNames(p1->parent->name, p2->parent->name)) {
        return 1;
    }
    //call recursively with person p2 parent
    related = isRelated(p1, p2->parent);
    
    //checks if relation was found
    if (related) {
        return 1;
        //else call with the parent merriedTo - i.e wife
    } else {
        related = isRelated(p1, p2->parent->merriedTo);
        if (related) {
            return 1;
        }
    }
    //if no relation found so far call with p1 parent
    related = isRelated(p1->parent, p2);
    //and do so the same with p1 parent wife
    if (related) {
        return 1;
    } else {
        related = isRelated(p1->parent->merriedTo, p2);
    }
    
    return related;
}

/**
 @brief function to merry two persons
 @param p1 person #1
 @param p2 person #2
 */
void merryTwoPeps(Person * p1, Person * p2) {
    //set merried attribute
    p1->merriedTo = p2;
    p2->merriedTo = p1;
    //prints
    printf("%s and %s are now married\n", p1->name, p2->name);
}

/**
 @brief function to validate two persons details before merriege
 @param p1 person #1
 @param p2 person #2
 @return 1 if the merriege of the two is legit 0 otherwise
 */
int validateMerriege(Person * p1, Person * p2) {
    //init consts
    const int MIN_AGE_TO_MERRY = 18;
    //checks if the persons actually exists
    if (p1 == NULL || p2 == NULL) {
        printf("One of the persons does not exist\n");
        return 0;
        //checks if they are old enough and have no common predeccessor or already merried
    } else if (p1->age < MIN_AGE_TO_MERRY || p2->age < MIN_AGE_TO_MERRY || isRelated(p1, p2)\
               || p1->merriedTo != NULL || p2->merriedTo != NULL){
        printf("Invalid marriage\n");
        return 0;
    }

    //merrige are good - mazal tov
    return 1;
}


/**
 @brief function to get the couple wanting to get merried there details
 @param persons the persons list
 */
void getDetailsToMerriege(Persons * persons) {
    //get the two persons names
    char * p1Name = getInfiniteString("Enter the name of the first person:", persons);
    char * p2Name = getInfiniteString("Enter the name of the second person:", persons);
    
    //get the acctual persons from the list
    Person * p1 = getPerson(persons->head, p1Name);
    Person * p2 = getPerson(persons->head, p2Name);

    //validate there details and merry them if its all good
    if (validateMerriege(p1, p2)) {
        merryTwoPeps(p1, p2);
    }
    //free the allocated space for the names
    free(p1Name);
    free(p2Name);
}

/**
 @brief function to free array dymacially allocated  with char pointers
 @param string array
 @param size of the array
 */
void freeStringsArray(char * string[], int size) {
    //run for each array index and free its content
    for (int i = 0; i < size; i++) {
        free(string[i]);
    }
}

/**
 @brief function to avoid duplicate lines of code of commends commonly executed
 the function will get the person by its name and unset all visited
 @param head of the list
 @param name of the persons to fetch
 @return Person struct of the person
 */
Person * getPerson(Node * head, char *name) {
    Person * p = getPersonByName(head, name);
    unVisitTree(head);
    return p;
}


/**
 @brief function to validate the new offspring details
 @param p1 parent #1
 @param p2 paren #2
 @return 1 if valid 0 otherwise
 */
int validateOffSpringParents(Person * p1, Person * p2, Person * child) {
    //if one of the persons not exists
    if (p1 == NULL || p2 == NULL) {
        printf("One of the parents does not exist\n");
        return 0;
        //if the persons not merried
    } else if (!p1->merriedTo || strcmp(p1->merriedTo->name, p2->name)) {
        printf("The parents are not married\n");
        return 0;
        //if the child name exists
    }else if (child != NULL) {
        printf("The name is already taken\n");
        return 0;
    }
    return 1;
}


/**
 @brief function to add new offspring to merried couple
 @param persons list of persons
 */
void addOffSpring(Persons * persons) {
    char *strings[3];
    const int ARRAY_SIZE = 3;
    //get the persons name
    strings[0] = getInfiniteString("Enter the name of the first parent:", persons);
    strings[1] = getInfiniteString("Enter the name of the second parent:", persons);
    strings[2] = getInfiniteString("Enter offspring's name:", persons);

    //if the persons list is NULL return
    if (persons == NULL) {
        printf("One of the parents does not exist\n");
        return;
    }

    //get the persons
    Person * p1 = getPerson(persons->head, strings[0]);
    Person * p2 = getPerson(persons->head, strings[1]);
    Person * child = getPerson(persons->head, strings[2]);

    //validate the details of the offspring and parents
    if (!validateOffSpringParents(p1, p2, child)) {
        freeStringsArray(strings, ARRAY_SIZE);
        return;
    }

    //add the offspring to the list of childrens of the parants
    Person *newPerson = makePerson(persons,strings[2], 0, p1);
    addNewPerson(persons,&p1->children, newPerson);
    //set the merry couple children to point to the same list
    p1->merriedTo->children = p1->children;
    //free the array
    freeStringsArray(strings, ARRAY_SIZE - 1);
    printf("%s was born\n", newPerson->name);
}

/**
 @brief function to prints the persons tree from a given person
 @param head of the list - the person
 @param tabCounter to print in thw wanted format
 */
void printPersons(Node * head, int tabCounter) {
    
    //check if head is null
    if (head == NULL) {
        return;
    }
    
    //init the curr person
    Person * person = (Person*)(head->data);
    
    //avoid printing the same person twice
    if (person->visited) {
        return;
    }
    
    //prints tab
    for (int i = 0; i < tabCounter; i++) {
        printf("\t");
    }
    //avoid print the same person as mentioned
    person->visited = 1;
    //prints the person
    printf("%s (%d)",person->name,person->age);
    //check for its wife and prints it if exists
    if (person->merriedTo) {
        printf(" - %s (%d)\n", person->merriedTo->name, person->merriedTo->age);
        person->merriedTo->visited = 1;
    } else {
        printf("\n");
    }
    //call recurively with person childrens and next
    printPersons(person->children, tabCounter + 1);
    printPersons(head->next, tabCounter);
}

/**
 @brief function to print the familiy
 @param persons list
 */
void printFamily(Persons * persons) {

    //init
    const int TAB_COUNTER = 0;
    //get the name of the person to print from
    char * name = getInfiniteString("Enter the name of the person:", persons);
    //get the person
    Person * p = getPerson(persons->head, name);
    
    //if the person not exists print error
    if (!p) {
        printf("The person does not exist\n");
        return;
    }
    //keep the person data and next in a temp variable
    Node * tempHeadData = persons->head->data;
    Node * tempNext = persons->head->next;
    //change the list data to point to the head of family
    persons->head->data = p;
    //change next to point to null
    persons->head->next = NULL;
    
    //print the person and unvisit the tree
    printPersons(persons->head, TAB_COUNTER);
    unVisitTree(persons->head);
    //restore the list and free the name
    persons->head->data = tempHeadData;
    persons->head->next = tempNext;
    free(name);
}
/**
 @brief function to increment the age of every person on the list
 @param head of the list
 @param amount to increment
 */
void incrementAge(Node * head, unsigned int amount) {
    
    //checks for head
    if (head == NULL) {
        return;
    }
    
    //get the person and its visited
    Person * p = head->data;
    if (p->visited) {
        return;
    }
    //increment
    p->age += amount;
    
    //set visited
    p->visited = 1;
    incrementAge(p->children, amount);
    incrementAge(head->next, amount);
}

/**
 @brief function to get the number to increment from the user
 the function will increment all persons age
 @param head the head of the list
 */
void getIncrementNumber(Node * head) {

    //get the increment amount
    printf("Enter number of years:\n");
    unsigned int amount;
    scanf("%u", &amount);
    scanf("%*c");
    
    //increment the age
    incrementAge(head, amount);
    unVisitTree(head);
}

/**
 @brief a function to add char pointer to array of pointers
 @param name the name pointer
 @param arr the array
 @param sizeArr arr size
 @param persons the list
 */
void addNameToArray(char * name, char *** arr, unsigned int * sizeArr, Node * persons) {
    
    char *** tempArr = arr;
    //allocate more place in arr.
    (*arr) = (char**)realloc(*arr, (*sizeArr + 1) * sizeof(char*));
    //checks for allocation faliure
    if (!*arr) {
        free(tempArr);
        exitProgram(persons);
    }
    //point the new place to the name and increment size
    (*arr)[*sizeArr] = name;
    (*sizeArr)++;
}

/**
 @brief function to add  names to array of names
 the function will recieve parent from findCommonParent function and go down the tree the given deg times
 and when reaches the given degree times it adds the level to the names array
 @param person parent given from findCommonParent function
 @param deg the deg to go down the tree
 @param currDeg used to keep the times the function call itself recursively
 @param sizeArr the size of arr
 @param arr the array of names
 */
void addPersonsToArray(Node * person, unsigned int deg, unsigned int currDeg,\
                       unsigned int * sizeArr, char *** arr, Node * persons) {
    //if the person has no children do nothing
    if (person == NULL) {
        return;
    }
    
    if (currDeg > deg) {
        return;
    }
    //get the current person
    Person * p = person->data;
    //checks for the degree and if reaches add the name
    if (deg == currDeg) {
        addNameToArray(p->name, arr, sizeArr, persons);
        //adding the nodes from the same side
    } else {
        addPersonsToArray(p->children, deg, currDeg + 1, sizeArr, arr, persons);
    }
    
    //calls next to get the next node when we on the wanted degree
    addPersonsToArray(person->next, deg, currDeg, sizeArr, arr, persons);
}


/**
 @brief function to get the commot parent with given related degree
 the function will go up in the family tree the given degree times toward the left and right parent of every
 node it incounters and stops when it found person whos the parent distant the degree amount from the start node
 @param person the start node
 @param arr the array to fill with the names of relatives
 @param currDeg used to keep the times the function calls recursively
 @param sizeArr the size of arr
 */
void findCommonParent(Person * person, char *** arr, unsigned int deg,\
                      unsigned int currDeg, unsigned int * sizeArr, Node * persons) {

    //check for the currDeg
    if (currDeg == deg) {
        //send degree minus 1 since we send the children
        int addDeg = deg - 1;
        //sends to addPersonsToArray the parent that was found
        addPersonsToArray(person->children, addDeg, INIT_DEG, sizeArr, arr, persons);
        //return because parent can only be one
        return;
    }
        
    //check for parent and call recursively with left and right parent
    if (person->parent) {
        findCommonParent(person->parent, arr, deg, currDeg + 1, sizeArr, persons);
        findCommonParent(person->parent->merriedTo, arr, deg, currDeg + 1, sizeArr, persons);
    } else {
        //if the deg is to big and the person is parent is null send person with deg - 1
        findCommonParent(person, arr, deg - 1, currDeg, sizeArr, persons);
    }
    
    return;
}


/**
 @brief function to print the array
 @param arr the array to print
 @param size of arr
 */
void printArr(char ** arr, unsigned int size) {
    
    //prints the first in the array is size is not 0
    if (size) {
        printf("%s\n", arr[0]);
    }
    //run from one
    for(unsigned int i = 1; i < size; i++) {
        //checks for duplicate names and continue in so
        if (!strcmp(arr[i], arr[i - 1])) {
            continue;
        }
        printf("%s\n", arr[i]);
    }
}

/**
 @brief function to swap indexes of array
 @param arr to swap
 @param i index
 @param j index
 */
void swap (char ** arr, int i, int j) {
    char * temp = *(arr + i);
    *(arr + i) = *(arr + j);
    *(arr + j) = temp;
}

/**
 @brief function to sort arr
 @param arr to sort
 @param size of arr
 */
void bubbleSortArr(char ** arr, unsigned int size) {
    
    //init swapped to break when sorted
    int isSwapped = 0;
    
    //loop throgh
    for (unsigned int i = 0; i < size - 1; i++) {
        for (unsigned int j = 0; j < size - 1 - i; j++) {
            //compare by name
            if (compareNames(arr[j], arr[j + 1]) > 0) {
                //make the swap
                swap(arr, j, j + 1);
                isSwapped = 1;
            }
        }
        //checks if no swaps made on the array then the array is sorted
        if (!isSwapped) {
            return;
        }
        //set it to check the next loop
        isSwapped = 0;
    }
}


/**
 @brief function to get all relative from given degree
 the function will prompt the user and allocate array of names
 @param persons list
 */
void getRelativeFromDegree(Persons * persons) {
    //prompt the user
    char * name = getInfiniteString("Enter the name of the person:", persons);
    unsigned int degree = getNumberInput("Enter degree:");
    
    //get the person
    Person * p = getPerson(persons->head, name);
    //checks if the person not exists end prints error
    if (!p) {
        printf("The person does not exist\n");
        //free the name
        free(name);
        return;
    }
    printf("Cousins:\n");
    //if degree is 0 just print the person name
    if (degree == 0) {
        printf("%s\n", p->name);
    } else {
        //init size
        unsigned int size = 0;
        //allocate array of names
        char ** namesArr = (char **)malloc(sizeof(char*));
        //stores the names inside the array
        findCommonParent(p, &namesArr, degree, INIT_DEG, &size, persons->head);
        
        //sort the array and prints it
        if (*namesArr) {
            bubbleSortArr(namesArr, size);
            printArr(namesArr, size);
        } else  {
            printf("%s\n", p->name);
        }
        //free arr
        free(namesArr);
    }
    //free the name
    free(name);
}

/**
 @brief function to get the number of persons on the list
 @param head of the list
 @param count pointer to increment by address
 */
void getNumberOfNodes(Node * head, unsigned int * count) {
    //checks for no list
    if (head == NULL) {
        return;
    }
    //init curr person
    Person * p = head->data;
    
    //if not visited this person increment count
    if (!p->visited) {
        (*count)++;
    }
    //set its visited to avoid printing twice
    p->visited = 1;
    //increment with the person wifes if exists
    if (p->merriedTo) {
        if (!p->merriedTo->visited) {
            (*count)++;
        }
        p->merriedTo->visited = 1;
    }
    //recursive call
    getNumberOfNodes(p->children, count);
    getNumberOfNodes(head->next, count);
}

/**
 @brief function to get the persons count from a given list
 the function will initalize the count and call the function to get the count
 @param head of the list
 */
void getPersonsCount(Node * head) {
    
    //init count
    unsigned int count = 0;
    //send count by address and call to unvisit the tree
    getNumberOfNodes(head, &count);
    unVisitTree(head);
    //if count is one prints
    if (count == 1) {
        printf("There is one person\n");
        return;
    }
    //else prints the count
    printf("There are %u people\n", count);
}


/**
 @brief function to handle the user input for the menu choice
 @param choice the user entered choice
 @param persons the persons list
 @return 0 in case the player want to exit 1 if not
 */
int handleUserChoice(char choice, Persons * persons) {

    //check for every possible choice case
    switch (choice) {
        case '0':
            exitProgram(persons->head);
            break;
        case '1':
            addFamilyHead(persons);
            break;
        case '2':
            getDetailsToMerriege(persons);
            break;
        case '3':
            addOffSpring(persons);
            break;
        case '4':
            printFamily(persons);
            break;
        case '5':
            getIncrementNumber(persons->head);
            break;
        case '6':
            getPersonsCount(persons->head);
            break;
        case '7':
            getRelativeFromDegree(persons);
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
1. Add a head\n\
2. Marry two people\n\
3. New offspring\n\
4. Print family top-down\n\
5. Years pass\n\
6. Count people\n\
7. Print cousins\n");
}
