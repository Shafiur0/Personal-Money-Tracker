#include <stdio.h>    // Standard Input/Output
#include <stdlib.h>   // Memory allocation, free()
#include <string.h>   // String handling functions

// ==================== Macros ====================
#define MAX_CATEGORIES 10   // Max number of categories
#define MAX_NAME 50         // Max length for strings
#define MAX_STACK 50        // Stack size
#define MAX_QUEUE 50        // Queue size

// ==================== Array ====================
struct Category {
    char name[MAX_NAME];   // Category name
};
struct Category categories[MAX_CATEGORIES];
int categoryCount = 0;     // How many categories currently exist

// ==================== Linked List ====================
struct Transaction {
    int id;                 // Unique transaction ID
    char type[15];          // Add Money / Spend Money
    char category[MAX_NAME];// Which category this belongs to
    float amount;           // Amount of money
    float currentBalance;   // Balance after this transaction
    struct Transaction *next; // Pointer to next transaction
};
struct Transaction *head = NULL;  // Head pointer for linked list
int transactionId = 1;            // Start IDs from 1

// ==================== Stack ====================
// Used to store transaction IDs (history tracking)
int stack[MAX_STACK];
int top = -1;

// ==================== Queue ====================
// Used for payment reminders
struct Payment {
    char note[MAX_NAME];
    float amount;
};
struct Payment paymentQueue[MAX_QUEUE];
int front = -1, rear = -1;

// ==================== Extra Globals ====================
float monthlyBudget = 0;   // Budget limit for the month
float totalSpent = 0;      // Track total spending

// ==================== Function Prototypes ====================
void seedCategories();
void addTransaction(float *currentBalance);
void viewTransactions();
void insertTransactionFront(int id, char type[], char category[], float amount, float currentBalance);
void addPaymentReminder();
void viewPaymentReminders();
void clearTransactions();
void pauseEnter();

// Extra features
void setBudget();
void viewMonthlyReport();
void savingsSuggestion();

// ==================== Main Function ====================
int main() {
    int choice;
    float currentBalance = 0;
    seedCategories();   // Load default categories

    printf("=== Personal Budget Tracker ===\n");

    while(1) {
        // Menu
        printf("\n--- Menu ---\n");
        printf("1. Add Transaction\n");
        printf("2. View Transactions\n");
        printf("3. Add Payment Reminder\n");
        printf("4. View Payment Reminders\n");
        printf("5. Set Monthly Budget\n");
        printf("6. View Monthly Report\n");
        printf("7. Get Savings Suggestions\n");
        printf("0. Exit\nEnter choice: ");

        // Input validation
        if(scanf("%d",&choice)!=1){
            int c; while((c=getchar())!='\n' && c!=EOF){}
            printf("Invalid input!\n");
            continue;
        }
        if(choice==0) break; // Exit program

        // Call functions based on menu choice
        switch(choice) {
            case 1: addTransaction(&currentBalance); break;
            case 2: viewTransactions(); break;
            case 3: addPaymentReminder(); break;
            case 4: viewPaymentReminders(); break;
            case 5: setBudget(); break;
            case 6: viewMonthlyReport(); break;
            case 7: savingsSuggestion(); break;
            default: printf("Invalid choice!\n");
        }
        pauseEnter(); // Wait for Enter before showing menu again
    }

    clearTransactions(); // Free memory
    printf("Thanks for using Budget Tracker. Bye!\n");
    return 0;
}

// ==================== Seed Categories ====================
// Add some default categories
void seedCategories() {
    strcpy(categories[0].name,"Food");
    strcpy(categories[1].name,"Transport");
    strcpy(categories[2].name,"Entertainment");
    strcpy(categories[3].name,"Salary");
    strcpy(categories[4].name,"Misc");
    categoryCount =5;
}

// ==================== Add Transaction ====================
void addTransaction(float *currentBalance) {
    char type[15], cat[MAX_NAME];
    float amount;
    int tchoice, cchoice;

    // Choose transaction type
    printf("Enter type (1 for Add Money, 2 for Spend Money): ");
    scanf("%d", &tchoice);
    if(tchoice == 1) strcpy(type, "Add Money");
    else if(tchoice == 2) strcpy(type, "Spend Money");
    else {
        printf("Invalid choice! Defaulting to Spend Money.\n");
        strcpy(type, "Spend Money");
    }

    // Show available categories
    printf("\n--- Categories ---\n");
    for(int i = 0; i < categoryCount; i++)
        printf("%d. %s\n", i+1, categories[i].name);

    // Select category
    printf("Enter category number: ");
    scanf("%d", &cchoice);
    if(cchoice >= 1 && cchoice <= categoryCount)
        strcpy(cat, categories[cchoice - 1].name);
    else {
        printf("Invalid category! Defaulting to Others.\n");
        strcpy(cat, "Others");
    }

    // Enter amount
    printf("Enter amount: ");
    scanf("%f",&amount);

    // Update balance and spending
    if(tchoice == 1) *currentBalance += amount;
    else {
        *currentBalance -= amount;
        totalSpent += amount;
    }

    // Budget warning
    if(monthlyBudget > 0 && totalSpent > monthlyBudget)
        printf("⚠ Warning: You exceeded your budget (%.2f)!\n", monthlyBudget);

    // Insert transaction into linked list
    insertTransactionFront(transactionId, type, cat, amount, *currentBalance);

    // Push ID into stack
    stack[++top] = transactionId;

    transactionId++;
    printf("Transaction added. Current Balance: %.2f\n", *currentBalance);
}

// ==================== Insert into Linked List ====================
// Always adds at the front (head) of the list
void insertTransactionFront(int id, char type[], char category[], float amount, float currentBalance) {
    struct Transaction *newNode = (struct Transaction*)malloc(sizeof(struct Transaction));
    if(newNode==NULL){ printf("Memory error!\n"); return; }
    newNode->id=id;
    strcpy(newNode->type,type);
    strcpy(newNode->category,category);
    newNode->amount=amount;
    newNode->currentBalance = currentBalance;
    newNode->next=head; // link old head
    head=newNode;       // new node becomes head
}

// ==================== View Transactions ====================
void viewTransactions() {
    struct Transaction *t=head;
    if(t==NULL){ printf("No transactions.\n"); return; }
    printf("\n--- Transactions ---\n");
    while(t!=NULL){
        printf("ID:%d Type:%s Category:%s Amount:%.2f Current Balance:%.2f\n",
               t->id, t->type, t->category, t->amount, t->currentBalance);
        t=t->next;
    }
}

// ==================== Add Payment Reminder (Queue) ====================
void addPaymentReminder() {
    if(rear == MAX_QUEUE-1){ printf("Queue full!\n"); return; }
    if(front==-1) front=0;

    printf("Enter payment note: ");
    getchar(); // clear newline
    char note[MAX_NAME];
    fgets(note, MAX_NAME, stdin);
    note[strcspn(note,"\n")]=0; // remove newline

    printf("Enter payment amount: ");
    float amt;
    scanf("%f",&amt);

    strcpy(paymentQueue[++rear].note, note);
    paymentQueue[rear].amount = amt;

    printf("Payment reminder added.\n");
}

// ==================== View Payment Reminders ====================
void viewPaymentReminders() {
    if(front==-1 || front>rear){ printf("No payment reminders.\n"); return; }
    printf("\n--- Payment Reminders ---\n");
    for(int i=front;i<=rear;i++){
        printf("%d. Note: %s Amount:%.2f\n", i-front+1, paymentQueue[i].note, paymentQueue[i].amount);
    }
}

// ==================== Budget Planning ====================
void setBudget() {
    printf("Enter monthly budget: ");
    scanf("%f", &monthlyBudget);
    printf("Monthly budget set to: %.2f\n", monthlyBudget);
}

// ==================== Monthly Report ====================
void viewMonthlyReport() {
    struct Transaction *t=head;
    if(t==NULL){ printf("No transactions to report.\n"); return; }
    float income=0, expense=0;
    while(t!=NULL){
        if(strcmp(t->type,"Add Money")==0) income+=t->amount;
        else expense+=t->amount;
        t=t->next;
    }
    printf("\n--- Monthly Report ---\n");
    printf("Total Income : %.2f\n", income);
    printf("Total Expense: %.2f\n", expense);
    printf("Net Balance  : %.2f\n", income-expense);
}

// ==================== Savings Suggestion ====================
void savingsSuggestion() {
    if(totalSpent==0){ printf("No spending data yet.\n"); return; }

    float food=0, transport=0, ent=0, misc=0;
    struct Transaction *t=head;
    while(t!=NULL){
        if(strcmp(t->type,"Spend Money")==0){
            if(strcmp(t->category,"Food")==0) food+=t->amount;
            else if(strcmp(t->category,"Transport")==0) transport+=t->amount;
            else if(strcmp(t->category,"Entertainment")==0) ent+=t->amount;
            else misc+=t->amount;
        }
        t=t->next;
    }

    // Suggest where to cut spending
    printf("\n--- Savings Suggestions ---\n");
    if(food/totalSpent > 0.4) printf("You are spending too much on Food! Try to reduce eating out.\n");
    if(ent/totalSpent > 0.3) printf("Entertainment costs are high. Reduce movies/subscriptions.\n");
    if(transport/totalSpent > 0.3) printf("Transport is costly. Try carpool or public transport.\n");
    if(misc/totalSpent > 0.3) printf("Miscellaneous costs are high. Track unnecessary buys.\n");
    printf("Total Spent: %.2f. Smart saving can help you a lot!\n", totalSpent);
}

// ==================== Clear Linked List ====================
// Free memory for all transactions
void clearTransactions() {
    struct Transaction *t=head;
    while(t!=NULL){
        struct Transaction *temp=t;
        t=t->next;
        free(temp);
    }
    head=NULL; // list is now empty
}

// ==================== Pause Enter ====================
// Wait until user presses Enter
void pauseEnter(){
    int c;
    printf("\n(Press Enter to continue)");
    while((c=getchar())!='\n' && c!=EOF){} // clear buffer
    getchar();
}
