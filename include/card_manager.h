#ifndef CARD_MANAGER_H
#define CARD_MANAGER_H

#include "card.h"  

typedef struct {
    Card *cards;  
    int cardsCount;    
    int allocatedCardCount;
} CardManager;

void initCardManager(CardManager *manager);

void remoteTest();

void addCard(CardManager *manager);

void readCardsFromFile(CardManager *manager, const char *filename);

void printCards(const CardManager *manager);

void fakeTest(CardManager *manager);


#endif  