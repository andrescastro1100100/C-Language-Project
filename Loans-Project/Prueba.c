#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Global Constants
#define PASSWORD_LENGTH 12

// Global Structures
struct User
{
    int id;
    char firstName[50];
    char lastName[50];
    char email[100];
    char password[PASSWORD_LENGTH + 1];
};


struct Client
{
    int id;
    char firstName[50];
    char lastName[50];
    double loanAmount;
    int dueDays;
    bool isLoanPaid;
};




// Global variables
int system_UserId = 0;
char system_Password[PASSWORD_LENGTH] = "password";
int userCount = 0;
struct User users[100];
struct User currentUser;



int clientCount = 0;
struct Client clients[100];



void clearScreen()
{
    // Clear screen and move cursor to top-left corner
    printf("\033[2J\033[H");
}

void lockTillEnter()
{
    // Clear the input buffer
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;

    printf("Press Enter to continue...\n");

    do
    {
        ch = getchar(); // Read a character from the console
    } while (ch != '\n' && ch != EOF);
}

bool loadUsers()
{
    FILE *file = fopen("file_database/user_data.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return false;
    }

    // Read users from the file
    while (fscanf(file, "%d %s %s %s %s", &users[userCount].id, users[userCount].firstName, users[userCount].lastName, users[userCount].email, users[userCount].password) == 5)
    {
        userCount++;
    }

    fclose(file);
    return true;
}

void showUsers()
{
    clearScreen();
    for (int i = 0; i < userCount; i++)
    {
        printf("Id: %d\n", users[i].id);
        printf("Name: %s\n", users[i].firstName);
        printf("Last Name: %s\n", users[i].lastName);
        printf("Email: %s\n", users[i].email);
        printf("------------------------------\n");
    }
    lockTillEnter();
}

bool login()
{
    int userId;
    char password[50];
    bool login_successful = false;

    while (!login_successful)
    {
        clearScreen();
        printf("\n===================================\n");
        printf("          Login Menu\n");
        printf("===================================\n");
        printf("Enter user id: ");
        scanf("%d", &userId);
        printf("Enter password: ");
        scanf("%s", password);

        if (userId == system_UserId && strcmp(password, system_Password) == 0)
        {
            login_successful = true;
        }
        else
        {
            for (int i = 0; i < userCount; i++)
            {
                if (!login_successful)
                {
                    if (userId == users[i].id && strcmp(password, users[i].password) == 0)
                    {
                        login_successful = true;
                        currentUser = users[i];
                    }
                }
            }
        }

        if (!login_successful)
        {
            clearScreen();
            printf("Login failed. Do you want to try again?\n(yes/no): ");

            char tryAgain[50];
            scanf("%s", tryAgain);

            if (strcmp(tryAgain, "yes") != 0)
            {
                return false;
            }
        }
    }

    return true;
}

void displayMainMenu()
{
    clearScreen();
    printf("\n===================================\n");
    printf("          Console Menu\n");
    printf("===================================\n");
    printf("1. Add Users\n");
    printf("2. Show Users\n");
    printf("3. Add Client\n");
    printf("4. Show Clients Pending Payments\n");
    printf("5. Apply late payment interest\n");
    printf("6. Offer Payment Plan\n ");
    printf("7. Print Account Status\n");
    printf("8. Exit\n");
    printf("===================================\n");
    printf("Please select an option: ");
}

void AddUser()
{
    clearScreen();
    struct User newUser;

    printf("Enter First Name: ");
    scanf("%49s", newUser.firstName);

    printf("Enter Last Name: ");
    scanf("%49s", newUser.lastName);

    printf("Enter Email: ");
    scanf("%99s", newUser.email);

    // Validate the password has at most 12 characteres
    char passwordPreview[PASSWORD_LENGTH + 1];
    do
    {
        printf("Enter Password (max 12 characters): ");
        scanf("%s", passwordPreview);
        if (strlen(passwordPreview) <= PASSWORD_LENGTH)
        {
            strcpy(newUser.password, passwordPreview);
            break;
        }
        else
        {
            printf("Password exceeds 12 characters. Please try again!\n");
        }

    } while (true);

    FILE *file = fopen("file_database/user_data.txt", "a");
    if (file == NULL)
    {
        perror("Error opening user_data.txt file");
        return;
    }

    // Assign the new user Id
    newUser.id = ++userCount;

    fprintf(file, "%d %s %s %s %s\n", newUser.id, newUser.firstName, newUser.lastName, newUser.email, newUser.password);

    fclose(file);

    // Add new user to the users collection
    users[userCount - 1] = newUser;

    clearScreen();
    printf("User has been saved:\n");
    printf("Id %d:\n", newUser.id);
    printf("Name %d:\n", newUser.firstName);
    printf("Last Name: %s\n", newUser.lastName);
    printf("Email: %s\n", newUser.email);
    printf("---------------\n");

    lockTillEnter();
}

// new code


void AddClient()
{
    clearScreen();
    struct Client newClient;

    printf("Enter Client Name: ");
    scanf("%99s", newClient.firstName);

    printf("Enter Clients Lastname: ");
    scanf("%99s", newClient.lastName);

    printf("Enter Loan Amount: ");
    scanf("%lf", &newClient.loanAmount);

    newClient.isLoanPaid = false;

    printf("Enter Due Days: ");
    scanf("%d", &newClient.dueDays);

    
    newClient.id = ++clientCount;

    clients[clientCount - 1] = newClient;

    // Open the client data file for appending
    FILE *clientFile = fopen("file_database/client_data.txt", "a");
    if (clientFile == NULL)
    {
        perror("Error opening client_data.txt file");
        return;
    }

  

    // Write client information to the file
    fprintf(clientFile, "%d %s %s %.2lf %d %d\n", newClient.id, newClient.firstName, newClient.lastName, newClient.loanAmount, newClient.dueDays, newClient.isLoanPaid ? 1 : 0);

    fclose(clientFile);



    clearScreen();
    printf("Client has been added:\n");
    printf("ID: %d\n", newClient.id);
    printf("Name: %s\n", newClient.firstName);
    printf("Lastname: %s\n", newClient.lastName);
    printf("Loan Amount: %.2lf\n", newClient.loanAmount);
    printf("Due Days: %d\n", newClient.dueDays);
    printf("Loan Status: %s\n", newClient.isLoanPaid ? "Paid" : "Not Paid");
    printf("------------------------------\n");
    lockTillEnter();
}




bool loadClients() {
    FILE *file = fopen("file_database/client_data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    // Read clients from the file
    while (fscanf(file, "%d %99s %99s %lf %d %d", &clients[clientCount].id, clients[clientCount].firstName, clients[clientCount].lastName, &clients[clientCount].loanAmount, &clients[clientCount].dueDays, &clients[clientCount].isLoanPaid) == 6) {
        clientCount++;
    }

    fclose(file);
    return true;
}

void showOverdueClients() {
    clearScreen();
    printf("Overdue Clients:\n");

    for (int i = 0; i < clientCount; i++) {
        if (!clients[i].isLoanPaid && clients[i].dueDays >= 60) {
            printf("Client ID: %d\n", clients[i].id);
            printf("Name: %s\n", clients[i].firstName);
            printf("Name: %s\n", clients[i].lastName);
            printf("Loan Amount: %.2lf\n", clients[i].loanAmount);
            printf("Due Days: %d\n", clients[i].dueDays);
            printf("Loan Status: %s\n", clients[i].isLoanPaid ? "Paid" : "Not Paid");
            printf("------------------------------\n");
        }
    }

    lockTillEnter();
}


//NO FUNCIONA ESTO

void applyLatePaymentInterestToFile() {
    const double latePaymentInterestRate = 0.05; // Example interest rate (5%)

    FILE *clientFile = fopen("file_database/client_data.txt", "r");
    if (clientFile == NULL) {
        perror("Error opening client_data.txt file");
        return;
    }

    FILE *tempFile = fopen("file_database/temp_client_data.txt", "w");
    if (tempFile == NULL) {
        perror("Error opening temp_client_data.txt file");
        fclose(clientFile);
        return;
    }

    struct Client client;
    while (fscanf(clientFile, "%d %99s %99s %lf %d %d", &client.id, client.firstName, client.lastName, &client.loanAmount, &client.dueDays, &client.isLoanPaid) == 6) {
        if (!client.isLoanPaid && client.dueDays >= 60) {
            client.loanAmount += client.loanAmount * latePaymentInterestRate;
        }
        fprintf(tempFile, "%d %s %s %.2lf %d %d\n", client.id, client.firstName, client.lastName, client.loanAmount, client.dueDays, client.isLoanPaid);
    }

    fclose(clientFile);
    fclose(tempFile);

    // Replace the original file with the temporary file
    remove("file_database/client_data.txt");
    rename("file_database/temp_client_data.txt", "file_database/client_data.txt");

    printf("Late payment interest applied to overdue clients in the file.\n");
    lockTillEnter();
}


void offerPaymentPlanToFile()
{
    FILE *clientFile = fopen("file_database/client_data.txt", "r+");
    if (clientFile == NULL)
    {
        perror("Error opening client_data.txt file");
        return;
    }

    struct Client client;
    while (fscanf(clientFile, "%d %99s %99s %lf %d %d", &client.id, client.firstName, client.lastName, &client.loanAmount, &client.dueDays, &client.isLoanPaid) == 6)
    {
        if (!client.isLoanPaid && client.dueDays >= 60)
        {
            printf("Offering a 3-month payment plan to Client ID %d (%s %s):\n", client.id, client.firstName, client.lastName);
            double monthlyPayment = client.loanAmount / 3.0;
            printf("Monthly Payment: %.2lf\n", monthlyPayment);
            printf("------------------------------\n");
        }
    }

    fclose(clientFile);
    lockTillEnter();
}

void printAccountStatusFromFile()
{
    clearScreen();
    printf("Account Status:\n");

    FILE *clientFile = fopen("file_database/client_data.txt", "r");
    if (clientFile == NULL)
    {
        perror("Error opening client_data.txt file");
        return;
    }

    struct Client client;
    while (fscanf(clientFile, "%d %99s %99s %lf %d %d", &client.id, client.firstName, client.lastName, &client.loanAmount, &client.dueDays, &client.isLoanPaid) == 6)
    {
        printf("Client ID: %d\n", client.id);
        printf("Name: %s\n", client.firstName);
        printf("Lastname: %s\n", client.lastName);
        printf("Loan Amount: %.2lf\n", client.loanAmount);
        printf("Due Days: %d\n", client.dueDays);
        printf("Loan Status: %s\n", client.isLoanPaid ? "Paid" : "Not Paid");

        if (!client.isLoanPaid && client.dueDays >= 60)
        {
            double monthlyPayment = client.loanAmount / 3.0;
            printf("Monthly Payment (3-month plan): %.2lf\n", monthlyPayment);
        }

        printf("------------------------------\n");
    }

    fclose(clientFile);
    lockTillEnter();
}

void startApp()
{
    int menuOption = 0;
    do
    {
        displayMainMenu();
        scanf("%d", &menuOption);
        switch (menuOption)
        {
        case 1:
            AddUser();
            break;
        case 2:
            showUsers();
            break;
        case 3:
            AddClient();
            break;
        case 4:
            showOverdueClients();
            break;
        case 5:
            applyLatePaymentInterestToFile();
            break;
        case 6:
            offerPaymentPlanToFile();
            break;
        case 7:
            printAccountStatusFromFile();
            break;
        case 8:
            printf("Application is closing...\n");
            break;
        default:
            printf("Menu option is invalid. Please select a valid option.\n");
            break;
        }
    } while (menuOption != 8);
}

int main()
{
    bool loginSuccessful;
    bool usersLoaded;
    bool clientsLoaded;


    usersLoaded = loadUsers();
    clientsLoaded = loadClients();

    if (!usersLoaded)
    {
        printf("\nError loading users from database.");
        return 0;
    }

    loginSuccessful = login();
    if (loginSuccessful)
    {
        startApp();
    }
    else
    {
        printf("\nApplication closed successfully!");
    }

    return 0;
}