//
// Created by mohamed on 15/12/22.
//

#include "card.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

EN_cardError_t getCardHolderName(ST_cardData_t* cardData){
    char name[27];
    printf("Please enter the card holder name: \n");
    // Check if name is scanned correctly
    if(fgets(name, 27, stdin) == NULL){
        return WRONG_NAME;
    }
    name[strlen(name) - 1] = '\0';
    // Check that name length is 20 characters minimum and 25 characters maximum
    if(strlen(name) < 20 || strlen(name) > 24){
        return WRONG_NAME;
    }
    // Check that name is all alphabetic characters and spaces
    for(int i = 0; i < strlen(name); i++){
        if(!isalpha(name[i]) && name[i] != ' '){
            return WRONG_NAME;
        }
    }

    // Set the name in the card data structure
    strcpy(cardData->cardHolderName, name);
    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData){
    char date[8];
    printf("Please enter the card expiry date: \n");
    // Check if date is scanned correctly
    if(fgets(date, 8, stdin) == NULL){
        return WRONG_EXP_DATE;
    }
    //scanf("%s\n", date);
    date[strlen(date) - 1] = '\0';
    // Check that date length is 5 characters
    if(strlen(date) != 5){
        return WRONG_EXP_DATE;
    }
    // Check that date is in the format MM/YY
    if(date[2] != '/'){
        return WRONG_EXP_DATE;
    }
    if(date[0] == '0'){
        if(date[1] > '9' || date[1] < '1'){
            return WRONG_EXP_DATE;
        }
    }
    else if(date[0] == '1'){
        if(date[1] > '2' || date[1] < '0'){
            return WRONG_EXP_DATE;
        }
    }
    else{
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
    char pan[22];
    printf("Please enter the card PAN: \n");
    // Check if PAN is scanned correctly
    if(fgets(pan, 22, stdin) == NULL){
        return WRONG_PAN;
    }
    pan[strlen(pan) - 1] = '\0';
    // Check that PAN length is 19 characters maximum and 16 characters minimum
    if(strlen(pan) < 16 || strlen(pan) > 19){
        return WRONG_PAN;
    }
    // Check that PAN is all numeric characters
    for(int i = 0; i < strlen(pan); i++){
        if(!(pan[i] >= '0' && pan[i] <= '9')){
            return WRONG_PAN;
        }
    }
    // Set the PAN in the card data structure
    strcpy(cardData->primaryAccountNumber, pan);
    return CARD_OK;
}

