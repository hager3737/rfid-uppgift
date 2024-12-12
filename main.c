#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <windows.h>
#include "include/card_manager.h"
#include "include/SafeInput.h"
#include "include/utils.h"

int main() {
    CardManager cardManager;  

    initCardManager(&cardManager);
    
    readCardsFromFile(&cardManager, "id.txt");
    
    while (true) {
        int choice;
        clearTerminal();
        showMenu();
        GetInputInt("Choice: ", &choice);
    
        if (choice == 1) {
            remoteTest();
        }
        else if (choice == 2) {
            printCards(&cardManager);
        }
        else if (choice == 3) {
            addCard(&cardManager);
        }
        else if (choice == 9) {
            fakeTest(&cardManager);
        }
        else if (choice == 4) {
            break;
        }
    }
    freeCardManager(&cardManager);
    return 0;
}