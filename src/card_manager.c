#include "../include/card_manager.h"
#include "../include/SafeInput.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <windows.h>

void initCardManager(CardManager *manager) {
    manager->allocatedCardCount = 10;  
    manager->cards = malloc(sizeof(Card) * manager->allocatedCardCount);
    if (manager->cards == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);  
    }
    manager->cardsCount = 0;
}

void remoteTest() {
    clearTerminal();
    printf("Door is open.\n");
    printf("3");
    Sleep(1000);
    printf(" 2");
    Sleep(1000);
    printf(" 1\n");
    Sleep(1000);
    printf("Door is closed.\n");
    printf("Press enter to return to menu...");
    getchar();
}

void printCards(const CardManager *manager) {
    clearTerminal();
    printf("***************************\n");
    printf("All cards in system\n");
    if(manager->cardsCount == 0) {
        printf("There are currently no cards registered in the system.\n");
    } 
    else {
        for(int i = 0; i < manager->cardsCount; i++) {
            printf("Card ID: %d", manager->cards[i].ID);
            printf(" | Access status: %s\n", manager->cards[i].Access ? "Access" : "No access");
            printf(" | Date Added: %s\n", manager->cards[i].dateAdded);
            printf("\n");
        }
    }
    mainMenu();
}

void addCard(CardManager *manager) {
    int access;
    int choice;
    int cardID = 0;
    bool correctInput = true;

    clearTerminal();

    printf("***************************\n");
    printf("1. Add new card.\n");
    printf("2. Change access for an already existing card.\n");
    printf("3. Return to main menu.\n");
    GetInputInt("Choice: ", &choice);

    while (choice != 1 && choice != 2 && choice != 3) {
        printf("Not a valid input, try again.\n");
        GetInputInt("Choice: ", &choice);
    }

    if (choice == 1) {
        while (correctInput) {
            GetInputInt("Scan or type in the Card ID (6 digits long, starts with 1-9): \n", &cardID);

            if (!isValidCardID(cardID)) {
                printf("Invalid Card ID! It must be 6 digits long and start with 1-9.\n");
                continue;
            }

            if (!isCardUnique(manager, cardID)) {
                printf("Card ID already exists. Please try again with a different ID.\n");
                continue;
            }

            correctInput = false;
        }

        Card newCardInput;
        newCardInput.ID = cardID;
        correctInput = true;

        printf("Type 1 to give access or 0 to not give access: \n");
        GetInputInt("1 or 0: ", &access);

        if (access != 1 && access != 0) {
            correctInput = false;
        }

        while (!correctInput) {
            printf("Not a valid input. Please try again.\n");
            GetInputInt("1 or 0: ", &access);
            if (access == 1 || access == 0) {
                correctInput = true;
            }
        }

        newCardInput.Access = (access == 1);

        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(newCardInput.dateAdded, sizeof(newCardInput.dateAdded), "%Y-%m-%d", t);
        
        if (manager->cardsCount >= manager->allocatedCardCount) {
            manager->allocatedCardCount *= 2;
            manager->cards = realloc(manager->cards, sizeof(Card) * manager->allocatedCardCount);
            if (manager->cards == NULL) {
                printf("Memory reallocation failed!\n");
                exit(1);  
            }
        }

        printf("Card with ID: %d added on: %s\n", cardID, newCardInput.dateAdded);
        manager->cards[manager->cardsCount] = newCardInput;
        manager->cardsCount++;

        FILE *file = fopen("id.txt", "a");
        if (file == NULL) {
            printf("Error opening the file.\n");
            return;
        }

        if (manager->cardsCount > 1) {
            fprintf(file, "\n");
        }
        fprintf(file, "%d,%d,%s", newCardInput.ID, newCardInput.Access, newCardInput.dateAdded);
        fclose(file);

        mainMenu();

    } else if (choice == 2) {

        if(manager->cardsCount == 0) {
            printf("There are currently no cards in the system.\n");
            mainMenu();
        } 
        else {

        bool keepTrying = true;

        while (keepTrying) {
            printf("Please type the ID for the card you would like to change access for:\n");

            do {
                GetInputInt("ID: ", &cardID);
                if (!(cardID >= 100000 && cardID <= 999999)) {
                    printf("Not a valid ID. Please ensure it is 6 digits long and starts with 1-9.\n");
                }
            } while (!(cardID >= 100000 && cardID <= 999999));

            int found = 0;

            for (int i = 0; i < manager->cardsCount; i++) {
                if (manager->cards[i].ID == cardID) {
                    found = 1;

                    printf("Current access status: %s\n", manager->cards[i].Access ? "Access" : "No access");

                    printf("Type 1 to grant access or 0 to revoke access:\n");
                    do {
                        GetInputInt("1 or 0: ", &access);
                        if (access != 1 && access != 0) {
                            printf("Not a valid input. Please type 1 to grant access or 0 to revoke access:\n");
                        }
                    } while (access != 1 && access != 0);

                    manager->cards[i].Access = (access == 1);
                    printf("Access updated for card ID %d\n", cardID);

                    FILE *file = fopen("id.txt", "w");
                    if (file == NULL) {
                        printf("Error opening file.\n");
                        return;
                    }

                    for (int j = 0; j < manager->cardsCount; j++) {
                    
                        fprintf(file, "%d,%d,%s", manager->cards[j].ID, manager->cards[j].Access, manager->cards[j].dateAdded);

                        if (j != manager->cardsCount - 1) {
                            fprintf(file, "\n");
                        }
                    }

                    fclose(file);
                    keepTrying = false;
                    break;
                }
            }

            if (!found) {
                printf("Card ID %d not found.\n", cardID);
                printf("What would you like to do?\n");
                printf("1. Try a different ID\n");
                printf("2. Return to main menu\n");

                int userChoice;
                do {
                    GetInputInt("Choice: ", &userChoice);
                    if (userChoice == 1) {
                        break;
                    } else if (userChoice == 2) {
                        printf("Returning to main menu...\n");
                        keepTrying = false;
                        break;
                    } else {
                        printf("Invalid choice. Please enter 1 or 2.\n");
                    }
                } while (true);
            }
        }
        mainMenu();
        }
    } else if (choice == 3) {
        return;
    }
}

void readCardsFromFile(CardManager *manager, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Something went wrong opening the file: %s\n", filename);
        return;
    }

    char buffer[255];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        
        manager->cards = realloc(manager->cards, (manager->cardsCount + 1) * sizeof(Card));
        if (manager->cards == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file);
            return;
        }

        Card *card = &manager->cards[manager->cardsCount];
        int access;

        sscanf(buffer, "%d,%d,%10s", &card->ID, &access, card->dateAdded);
        card->Access = (access == 1);

        manager->cardsCount++;
    }
    fclose(file);
}

void fakeTest(CardManager *manager) {
    int cardID;
    bool found = false;
    bool keepTrying = true;

    clearTerminal();

    printf("Please scan your card ID\n");
    printf("CURRENTLY LAMP IS: OFF\n");

    while (keepTrying) {
        do {
            GetInputInt("ID: ", &cardID);
            if (!isValidCardID(cardID)) {
                printf("Not a valid input. Please ensure the ID is 6 digits long and starts with 1-9.\n");
            }
        } while (!isValidCardID(cardID));

        found = false;
        for (int i = 0; i < manager->cardsCount; i++) {
            if (manager->cards[i].ID == cardID) {
                found = true;
                if (manager->cards[i].Access) {
                    printf("CARD FOUND. CURRENTLY LAMP IS: GREEN\n");
                } else {
                    printf("ACCESS DENIED. LAMP IS STILL: OFF\n");
                }
                mainMenu();
            }
        }

        if (found) {
            keepTrying = false;
        } else {
            printf("No ID in database matches ID: %d\n", cardID);
            printf("What would you like to do?\n");
            printf("1. Try a different card\n");
            printf("2. Return to main menu\n");

            int choice;
            do {
                GetInputInt("Choice: ", &choice);
                if (choice == 1) {
                    break;
                } else if (choice == 2) {
                    printf("Returning to main menu...\n");
                    keepTrying = false; 
                    break;
                } else {
                    printf("Invalid choice. Please enter 1 or 2.\n");
                }
            } while (true);
        }
    }
}
