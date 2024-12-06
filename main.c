#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "safeinput.h"
#include <windows.h>

typedef struct {
    int ID;
    bool Access;
    char dateAdded[11];
}Card;

void showMenu(){
    printf("***************************\n");
    printf("Admin menu\n");
    printf("1. Remote open door\n");
    printf("2. List all cards in system\n");
    printf("3. Add/Remove access\n");
    printf("4. Exit\n");
    printf("9. TEST SCAN CARD\n");
    printf("***************************\n");
}

void clearTerminal() {
    system("cls");
}

void readIdsFromFile(Card **cards, int *cardsCount) {

    FILE *file = fopen("id.txt", "r");
        if(file == NULL) {
            printf("Something went wrong opening the file.");
            return;
        }
    char buffer[255];
    int count = 0;

    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        *cards = realloc(*cards, (count + 1) * sizeof(Card));
        if(*cards == NULL) {
            printf("Memory storing failed.");
            fclose(file);
            return;
        }

        Card *card = &(*cards)[count];
        int access;

        sscanf(buffer, "%d,%d,%10s", &card->ID, &access, card->dateAdded);
        card->Access = (access == 1);

        count++;
    }

    *cardsCount = count;
    fclose(file);
}


void addCard(Card *card, int *cardsCount) {
    int access;
    int choice;
    int cardID = 0;
    bool emptyFile = false;

    clearTerminal();

    do{
    printf("***************************\n");
    printf("1. Add new card. Must be 6 numbers\n");
    printf("2. Change access for an allready existing card.\n");
    printf("3. Return to main menu.\n");
    GetInputInt("Choice: ", &choice);
    } while(choice != 1 && choice != 2 && choice && 3);
    
    if(choice == 1) {
        do{
            GetInputInt("Card ID:", &cardID);
            if(!(cardID >= 100000 && cardID <= 999999)) {
                printf("Card ID must be 6 didgits long.\n");
            }
        } while(!(cardID >= 100000 && cardID <= 999999));
        

        if(*cardsCount == 0) {
            card->ID = cardID;
            emptyFile = true;
        }
        else {
            for(int i = 0; i < *cardsCount; i++) {
                if(cardID == card[i].ID) {
                    printf("There is allready a card with that ID in the database...");
                    return;
                }
                else {
                    card->ID = cardID;
                }
            }
        }
         
        printf("Type 1 to give access or 0 to not give access: ");
        GetInputInt("1 or 0: ", &access);
        if(access != 1 && access != 0) {
            printf("%d is not a valid input...", access);
            return;
        }
        else {
            card->Access = (access == 1);
        }
        
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(card->dateAdded, sizeof(card->dateAdded), "%Y-%m-%d", t);
        
        printf("Card with ID: %d added on: %s\n", cardID, card->dateAdded);
        (*cardsCount)++;
    

        FILE *file = fopen("id.txt", "a");
        if(file == NULL) {
            printf("Error opening the file");
            return;
        }

        if(emptyFile == true){
            fprintf(file, "%d,%d,%10s", card->ID, card->Access, card->dateAdded);
        }
        else {
            fprintf(file, "\n%d,%d,%10s", card->ID, card->Access, card->dateAdded);
        }
        fclose(file);
    } 
    
    else if (choice == 2) {
        if(emptyFile == true) {
            printf("There are no cards in the system...\n");
            return;
        }

        printf("Please type the ID for the card you would like to change access for: ");
        if(!GetInputInt("ID: ", &cardID)) {
            printf("Invalid input. Enter a valid ID.\n");
            return;
        }

        int found = 0;
        for(int i = 0; i < *cardsCount; i++) {
            if(card[i].ID == cardID) {
                found = 1;
                printf("Current access status: %s\n", card[i].Access ? "Access" : "No access");
                printf("Type 1 to grant access or 0 to revoke access: ");
                GetInputInt("1 or 0: ", &access);
                if(access != 1 && access != 0) {
                    printf("%d is not a valid input...", access);
                }
                else {
                    card[i].Access = (access == 1);
                    printf("Access updated for card ID %d\n", cardID);
                    break;
                }
            }
        }

        if(!found) {
            printf("ID number %d not found", cardID);
        }
        else {

            FILE *file = fopen("id.txt", "w");
            if(file == NULL) {
                printf("Error opening file.\n");
                return;
            }
            if(*cardsCount > 0) {
                for(int i = 0; i < *cardsCount; i++) {
                    if(i == *cardsCount - 1) {
                        fprintf(file, "%d,%d,%s", card[i].ID, card[i].Access, card[i].dateAdded);
                    }
                    else {
                        fprintf(file, "%d,%d,%s\n", card[i].ID, card[i].Access, card[i].dateAdded);
                    }
                }
            }
            fclose(file);
        }
    } else if(choice == 3){
        return;
    }
    else {
        printf("Invalid choice.\n");
    }
    printf("***************************\n");
}

void fakeTest(Card *card, int *cardsCount) {
    int ID;
    int found = 0; 

    clearTerminal();

    printf("Please scan your card or enter 0 to go back to main menu.\n");
    printf("CURRENTLY LAMP IS: OFF\n");
    if(!GetInputInt("ID: ", &ID)) {
        printf("Invalid input. Enter a valid ID.\n");
        return;
    }

    for (int i = 0; i < *cardsCount; i++) {
        if (card[i].ID == ID) {
            found = 1; 
            if (card[i].Access) {
                printf("CURRENTLY LAMP IS: GREEN\n");
            } else {
                printf("ACCESS DENIED, LAMP IS STILL: OFF\n");
            }
            break; 
        }
    }

    if (!found) {
        printf("No ID in database matches ID: %d\n", ID);
    }
}

void printCards(Card *card) {
    printf("Card ID: %d", card->ID);
    printf(" | Access status: %s\n", card->Access ? "Access" : "No access");
    printf(" | Date Added: %s\n", card->dateAdded);
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
}


int main() {

    Card *dynamicAllCards = NULL;
    int cardsCount = 0;
    readIdsFromFile(&dynamicAllCards, &cardsCount);
    
    while(true) {
        int choice;    
        clearTerminal();    
        showMenu();
        GetInputInt("Choice: ", &choice);

        if(choice == 1) {
            remoteTest();
            getchar();
            getchar();
        }
        else if(choice == 2) {
            clearTerminal();
            printf("***************************\n");
            printf("All cards in system\n");
            if(cardsCount == 0) {
                printf("There are currently no cards registered in the system.\n");
            } 
            else {
                for(int i = 0; i < cardsCount; i++) {
                printCards(&dynamicAllCards[i]);
                printf("\n");
            }
            }
            printf("***************************\n");
            printf("Press enter to return to menu...");
            getchar();
            getchar();
        }
        else if(choice == 3) {
            if(dynamicAllCards == NULL) {
                dynamicAllCards = malloc( sizeof(Card) * 1);
            }
            else{
                int currentSize = cardsCount * sizeof(Card);
                dynamicAllCards = realloc(dynamicAllCards,currentSize + sizeof(Card));
            }
            addCard(dynamicAllCards, &cardsCount);
            readIdsFromFile(&dynamicAllCards, &cardsCount);
            printf("Press enter to return to menu...");
            getchar();
            getchar();
        }
        else if(choice == 9) {
            fakeTest(dynamicAllCards, &cardsCount);
            printf("Press enter to return to menu...");
            getchar();
            getchar();
        }
        else if(choice == 4) {
            break;
        }
    }
    free(dynamicAllCards);
    return 0;
}