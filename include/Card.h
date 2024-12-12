#ifndef CARD_H
#define CARD_H

#include <stdbool.h> 

typedef struct {
    int ID;
    bool Access;
    char dateAdded[11];
} Card;

#endif 