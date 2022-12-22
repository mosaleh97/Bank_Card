//
// Created by mohamed on 18/12/22.
//
#include <stdio.h>
#include <bits/types/time_t.h>
#include <time.h>
#include <stdlib.h>
#include "terminal.h"
#include "string.h"
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
   // Get current date from system
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    // Set the date in the terminal data structure in the format dd/mm/yyyy
    sprintf(termData->transactionDate, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData){
    if(cardData->cardExpirationDate[3] < termData->transactionDate[8] || cardData->cardExpirationDate[4] < termData->transactionDate[9] || cardData->cardExpirationDate[0] < termData->transactionDate[3] || cardData->cardExpirationDate[1] < termData->transactionDate[4]){
        return EXPIRED_CARD;
    }
    else{
        return TERMINAL_OK;
    }
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData){
    printf("Enter transaction amount: \n");
    scanf("%f", &(termData->transAmount));
    // Check that amount is positive
    if(termData->transAmount <= 0)
    {
        return INVALID_AMOUNT;
    }
    return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData){
    // Check that transaction amount is below the maximum transaction amount
    if(termData->transAmount > termData->maxTransAmount)
    {
        return EXCEED_MAX_AMOUNT;
    }
    return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount){
    // Check that maximum transaction amount is positive
    if(maxAmount <= 0)
    {
        return INVALID_MAX_AMOUNT;
    }
    termData->maxTransAmount = maxAmount;
    return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData){
   // Check that number is a luhn number
    int nDigits = strlen(cardData->primaryAccountNumber);

    int nSum = 0, isSecond = false;
    for (int i = nDigits - 1; i >= 0; i--) {

        int d = cardData->primaryAccountNumber[i] - '0';

        if (isSecond == true)
            d = d * 2;

        // We add two digits to handle
        // cases that make two digits after
        // doubling
        nSum += d / 10;
        nSum += d % 10;

        isSecond = !isSecond;
    }
    return (nSum % 10 == 0) ? TERMINAL_OK : INVALID_CARD;
}

