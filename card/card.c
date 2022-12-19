//
// Created by mohamed on 15/12/22.
//

#include "card.h"
#include <stdio.h>
#include <string.h>

EN_cardError_t getCardHolderName(ST_cardData_t* cardData){
    char* name;
    printf("Please enter the card holder name: ");
    // Check if name is scanned correctly
    if(fgets(name, 25, stdin) == NULL){
        return WRONG_NAME;
    }
    // Check that name length is 20 characters minimum and 25 characters maximum
    if(strlen(name) < 20 || strlen(name) > 24){
        return WRONG_NAME;
    }
    // Set the name in the card data structure
    strcpy(cardData->cardHolderName, name);
    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData){
    char* date;
    printf("Please enter the card expiry date: ");
    // Check if date is scanned correctly
    if(fgets(date, 6, stdin) == NULL){
        return WRONG_EXP_DATE;
    }
    // Check that date length is 5 characters
    if(strlen(date) != 5){
        return WRONG_EXP_DATE;
    }
    // Check that date is in the format MM/YY
    if(date[2] != '/'){
        return WRONG_EXP_DATE;
    }
    // Check that month is between 1 and 12
    if(date[0] < '1' || date[0] > '2'){
        return WRONG_EXP_DATE;
    }
    // Check that year is between 0 and 99
    if(date[3] < '0' || date[3] > '9' || date[4] < '0' || date[4] > '9'){
        return WRONG_EXP_DATE;
    }
    // Set the date in the card data structure
    strcpy(cardData->cardExpirationDate, date);
    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData){
    char* pan;
    printf("Please enter the card PAN: ");
    // Check if PAN is scanned correctly
    if(fgets(pan, 20, stdin) == NULL){
        return WRONG_PAN;
    }
    // Check that PAN length is 19 characters maximum and 16 characters minimum
    if(strlen(pan) < 16 || strlen(pan) > 19){
        return WRONG_PAN;
    }
    // Set the PAN in the card data structure
    strcpy(cardData->primaryAccountNumber, pan);
    return CARD_OK;
}

