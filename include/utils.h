#ifndef UTILS_H
#define UTILS_H

#include "card_manager.h"

void showMenu();

void clearInputBuffer();

void freeCardManager(CardManager *manager);

void clearTerminal();

bool isValidCardID(int cardID);

bool isCardUnique(CardManager *manager, int cardID);

void mainMenu();

#endif