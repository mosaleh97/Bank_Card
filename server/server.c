//
// Created by mohamed on 18/12/22.
//

#include "server.h"
#include <bits/types/FILE.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

EN_transStat_t recieveTransactionData(ST_transaction *transData) {
    ST_accountsDB_t accountRefrence;
    transData->transState = APPROVED;
    if (isValidAccount(&(transData->cardHolderData), &accountRefrence) == ACCOUNT_NOT_FOUND) {
        transData->transState = FRAUD_CARD;
        //return FRAUD_CARD;
    }
    if (isBlockedAccount(&accountRefrence) == BLOCKED_ACCOUNT) {
        transData->transState = DECLINED_STOLEN_CARD;
        //return DECLINED_STOLEN_CARD;
    }
    if (isAmountAvailable(&(transData->terminalData), &accountRefrence) == LOW_BALANCE) {
        transData->transState = DECLINED_INSUFFECIENT_FUND;
        //return DECLINED_INSUFFECIENT_FUND;
    }
    if (saveTransaction(transData) == SAVING_FAILED) {
        transData->transState = INTERNAL_SERVER_ERROR;
        //return INTERNAL_SERVER_ERROR;
    }
    if (transData->transState == APPROVED) {
        {
            FILE *outfile;

            // open file for writing and reading
            outfile = fopen("../Accounts_DB.txt", "r+");
            // Update account balance
            accountRefrence.balance -= transData->terminalData.transAmount;
            // read file contents till account number is found
            while (fread(&accountRefrence, sizeof(ST_accountsDB_t), 1, outfile)) {
                if (strcmp(accountRefrence.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber) == 0) {
                    break;
                }
            }

            fseek(outfile, -sizeof(ST_accountsDB_t), SEEK_CUR);
            fwrite(&accountRefrence, sizeof(ST_accountsDB_t), 1, outfile);
            fclose(outfile);
        }
    }
    return transData->transState;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence){
    // Check if account is in database
    // Open database file
    FILE *infile;
    // open file for reading and writing
    infile = fopen ("../Accounts_DB.txt", "r+");

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
    FILE *file = fopen("../Sequence_number.txt", "r+");
    if(file == NULL){
        printf("Error opening file");
        return SAVING_FAILED;
    }
    size_t size = fread(&transData->transactionSequenceNumber, sizeof(uint32_t), 1, file);
    if(size != 1){
        printf("Error reading file");
        return SAVING_FAILED;
    }
    rewind(file);
    // Increment sequence number
    transData->transactionSequenceNumber = transData->transactionSequenceNumber + 1;
    size = fwrite(&(transData->transactionSequenceNumber), sizeof(uint32_t), 1, file);
    if(size != 1){
        printf("Error writing to file");
        return SAVING_FAILED;
    }
    fclose(file);
    // Save transaction
    file = fopen("../Transactions_DB.txt", "a");
    if(file == NULL){
        return SAVING_FAILED;
    }
    size = fwrite(transData, sizeof(ST_transaction), 1, file);
    if(size != 1){
        return SAVING_FAILED;
    }
    // Close file
    fclose(file);
    listSavedTransactions();
    return SERVER_OK;
}

void listSavedTransactions(void) {
    FILE *file = fopen("../Transactions_DB.txt", "r");
    if (file == NULL) {
        return;
    }
    ST_transaction transData;
    size_t size = fread(&transData, sizeof(ST_transaction), 1, file);
    while (size == 1) {
        printf("#########################\n");
        printf("Transaction Sequence Number: %d\n", transData.transactionSequenceNumber);
        printf("Transaction Date: %s\n", transData.terminalData.transactionDate);
        printf("Transaction Amount: %f\n", transData.terminalData.transAmount);
        if(transData.transState == APPROVED){
            printf("Transaction State: APPROVED\n");
        } else if(transData.transState == DECLINED_INSUFFECIENT_FUND){
            printf("Transaction State: DECLINED_INSUFFECIENT_FUND\n");
        } else if (transData.transState == DECLINED_STOLEN_CARD){
            printf("Transaction State: DECLINED_STOLEN_CARD\n");
        } else if(transData.transState == FRAUD_CARD){
            printf("Transaction State: FRAUD_CARD\n");
        } else if(transData.transState == INTERNAL_SERVER_ERROR){
            printf("Transaction State: INTERNAL_SERVER_ERROR\n");
        }
        printf("Terminal Max Amount: %f\n", transData.terminalData.maxTransAmount);
        printf("Cardholder Name: %s\n", transData.cardHolderData.cardHolderName);
        printf("PAN: %s\n", transData.cardHolderData.primaryAccountNumber);
        printf("Card Expiration Date: %s\n", transData.cardHolderData.cardExpirationDate);
        printf("#########################\n");
        size = fread(&transData, sizeof(ST_transaction), 1, file);
    }
}
