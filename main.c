
#include <stdio.h>
#include "terminal/terminal.h"
#include <stdlib.h>
#include <string.h>

int main() {
    ST_terminalData_t *termData = malloc(sizeof(ST_terminalData_t));
    ST_cardData_t *cardData = malloc(sizeof(ST_cardData_t));
    strcpy(cardData->primaryAccountNumber, "1234567890123456");
    strcpy(cardData->cardExpirationDate, "13/21");

    getTransactionDate(termData);
    printf("%d", isCardExpired(cardData, termData));
    return 0;
}
