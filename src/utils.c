#include <stdio.h>
#include "../include/card_manager.h"
#include <stdlib.h>


void showMenu() {
    printf("***************************\n");
    printf("Admin menu\n");
    printf("1. Remote open door\n");
    printf("2. List all cards in system\n");
    printf("3. Add/Remove access\n");
    printf("4. Exit\n");
    printf("9. TEST SCAN CARD\n");
    printf("***************************\n");
}

void clearInputBuffer() {
    while (getchar() != '\n');  
}

void freeCardManager(CardManager *manager) {
    if (manager->cards != NULL) {
        free(manager->cards); 
        manager->cards = NULL; 
    }
    manager->cardsCount = 0; 
}

void clearTerminal() {
    system("cls");
}

bool isValidCardID(int cardID) {
    return cardID >= 100000 && cardID <= 999999;
}

bool isCardUnique(CardManager *manager, int cardID) {
    for (int i = 0; i < manager->cardsCount; i++) {
        if (manager->cards[i].ID == cardID) {
            return false;
        }
    }
    return true;
}

void mainMenu() {
    printf("***************************\n");
    printf("Press enter to return to menu...");
    getchar();
}