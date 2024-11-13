#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "safeinput.h"

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
    printf("3. Add/Remove acces\n");
    printf("4. Exit\n");
    printf("9. TEST SCAN CARD\n");
    printf("***************************\n");
}

void addCard(Card *card, int *cardsCount) {
    int access;
    int choice;
    int cardID;
    printf("***************************\n");

    printf("1. Add new card.\n");
    printf("2. Change access for an allready existing card.\n");
    scanf("%d", &choice);
    if(choice == 1) {
        printf("Card ID: ");
    scanf(" %d", &card->ID);
    printf("Type 1 to give access or 0 to not give access: ");
    scanf(" %d", &access);
    card->Access = (access == 1);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(card->dateAdded, sizeof(card->dateAdded), "%Y-%m-%d", t);
    
    printf("Card added on: %s\n", card->dateAdded);
    (*cardsCount)++;
    } 

    else if (choice == 2) {
        printf("Please type the ID for the card you would like to change access for: ");
        scanf("%d", &cardID);

        int found = 0;
        for(int i = 0; i < *cardsCount; i++) {
            if(card[i].ID == cardID) {
                found = 1;
                printf("Current access status: %s\n", card[i].Access ? "Access" : "No access");
                printf("Type 1 to grant access or 0 to revoke access: ");
                scanf("%d", &access);
                card[i].Access = (access == 1);
                printf("Access updated for card ID %d\n", cardID);
                break;
            }
        }
        if(!found) {
            printf("ID number %d not found", cardID);
        }
    } 
    printf("\n***************************");
}

void fakeTest(Card *card, int *cardsCount) {
    int ID;
    printf("Please scan your card or enter 0 to go back to main menu.\n");
    printf("CURRENTLY LAMP IS: OFF\n");
    scanf("%d", &ID);
        for(int i = 0; i < *cardsCount; i++) {
            if(card[i].ID == ID && card[i].Access == true) {
                printf("CURRENTLY LAMP IS: GREEN\n");
            }
            else if (card[i].ID != ID) {
                printf("No ID in database matches ID: %d\n", ID);
            }
            else if(card[i].ID == ID && card[i].Access == false) {
                printf("ACCESS DENIED, LAMP IS STILL: OFF\n");
            }
        }

}

void printCards(Card *card) {
    printf("Card ID: %d", card->ID);
    printf(" | Access status: %s\n", card->Access ? "Access" : "No access");
    printf(" | Date Added: %s\n", card->dateAdded);
}

int selection() {
    int selection;
    scanf("%d", &selection);
    return selection;
}

int main() {

    Card *dynamicAllCards = NULL; // 20056
    //dynamicAllCards = malloc( sizeof(Card) * 1 );
    //addCard(&dynamicAllCards[0]);  
    //Card allCards[5]; // 1000
    int cardsCount = 0;

    while(true) {
        int choice;        
        showMenu();
        choice = selection();

        if(choice == 1) {
            printf("Door is open.");
        }
        else if(choice == 2) {
            printf("***************************\n");
            printf("All cards in system\n");
            for(int i = 0; i < cardsCount; i++) {
                printCards(&dynamicAllCards[i]);
                printf("\n");
            }
            printf("***************************\n");
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
        }
        else if(choice == 9) {
            fakeTest(dynamicAllCards, &cardsCount);
        }
        else if(choice == 4) {
            break;
        }

        /*char namn[50];

        INPUT_RESULT ok;

        do{
        ok = GetInput("Type your name\n", namn, sizeof(namn) - 1);
        if(ok == INPUT_RESULT_OK){
            printf("%s", namn);
            
        }
        else if(ok == INPUT_RESULT_NO_INPUT){
            printf("You need to submit something...\n");
        }
        else {
            printf("The sentence can't be that long...\n");
        }
        } while(ok != INPUT_RESULT_OK);
    */
    }
    free(dynamicAllCards);
    return 0;
}