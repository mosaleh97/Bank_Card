//
// Created by mohamed on 18/12/22.
//

#include "server.h"
#include <bits/types/FILE.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

EN_transStat_t recieveTransactionData(ST_transaction *transData){
    ST_accountsDB_t accountRefrence;
    if(isValidAccount(&(transData->cardHolderData), &accountRefrence) == ACCOUNT_NOT_FOUND){
        return FRAUD_CARD;
    }
    if(isBlockedAccount(&accountRefrence) == BLOCKED_ACCOUNT){
        return DECLINED_STOLEN_CARD;
    }
    if(isAmountAvailable(&(transData->terminalData), &accountRefrence) == LOW_BALANCE){
        return DECLINED_INSUFFECIENT_FUND;
    }
    if(saveTransaction(transData) == SAVING_FAILED){
        return INTERNAL_SERVER_ERROR;
    }
    FILE *outfile;

    // open file for writing and reading
    outfile = fopen ("Accounts_DB", "r+");
    // Update account balance
    bool accountFound = false;
    // read file contents till end of file
    while(fread(&accountRefrence, sizeof(ST_accountsDB_t), 1, outfile))
    {
        if(strcmp(accountRefrence.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber) == 0){
            accountFound = true;
            break;
        }
    }

    fseek(outfile, -sizeof(ST_accountsDB_t), SEEK_CUR);
    fwrite(&accountRefrence, sizeof(ST_accountsDB_t), 1, outfile);

    return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence){
    // Check if account is in database
    // Open database file
    FILE *infile;
    // open file for reading and writing
    infile = fopen ("Accounts_DB", "r+");

    if (infile == NULL)
    {
        return ACCOUNT_NOT_FOUND;
    }

    bool accountFound = false;
    // read file contents till end of file
    while(fread(accountRefrence, sizeof(ST_accountsDB_t), 1, infile))
    {
        if(strcmp(accountRefrence->primaryAccountNumber, cardData->primaryAccountNumber) == 0){
            accountFound = true;
            break;
        }
    }

    // close file
    fclose (infile);

    if(accountFound == true){
        return SERVER_OK;
    }
    // Set accountRefrence to null
    accountRefrence = NULL;

    return ACCOUNT_NOT_FOUND;

}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence){
    // check if the account is blocked
    if(accountRefrence->state == BLOCKED){
        return BLOCKED_ACCOUNT;
    }
    return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence){
    // check if the amount is available
    if(termData->transAmount > accountRefrence->balance){
        return LOW_BALANCE;
    }
    return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction *transData){
    // Get sequence number from DB
    FILE *file = fopen("sequence.txt", "r");
    if(file == NULL){
        return SAVING_FAILED;
    }
    size_t size = fread(&transData->transactionSequenceNumber, sizeof(uint32_t), 1, file);
    // Increment sequence number
    transData->transactionSequenceNumber++;
    // Save sequence number
    file = fopen("sequence.txt", "w");
    if(file == NULL){
        return SAVING_FAILED;
    }
    size = fwrite(&transData->transactionSequenceNumber, sizeof(uint32_t), 1, file);
    if(size != 1){
        return SAVING_FAILED;
    }
    // Save transaction
    file = fopen("transactions.txt", "a");
    if(file == NULL){
        return SAVING_FAILED;
    }
    size = fwrite(transData, sizeof(ST_transaction), 1, file);
    if(size != 1){
        return SAVING_FAILED;
    }
    listSavedTransactions();
    return SERVER_OK;
}

void listSavedTransactions(void) {
    FILE *file = fopen("transactions.txt", "r");
    if (file == NULL) {
        return;
    }
    ST_transaction transData;
    size_t size = fread(&transData, sizeof(ST_transaction), 1, file);
    while (size == 1) {
        printf("#########################");
        printf("Transaction Sequence Number: %d", transData.transactionSequenceNumber);
        printf("Transaction Date: %s", transData.terminalData.transactionDate);
        printf("Transaction Amount: %f", transData.terminalData.transAmount);
        printf("Transaction State: %d", transData.transState);
        printf("Terminal Max Amount: %f", transData.terminalData.maxTransAmount);
        printf("Cardholder Name: %s", transData.cardHolderData.cardHolderName);
        printf("PAN: %s", transData.cardHolderData.primaryAccountNumber);
        printf("Card Expiration Date: %s", transData.cardHolderData.cardExpirationDate);
        printf("#########################");
    }
}
