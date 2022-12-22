#include "application.h"
void appStart(void) {
        ST_cardData_t cardData;
        ST_terminalData_t termData;
        EN_transStat_t transStat;
        // Get card data from user
        while (getCardHolderName(&cardData) != CARD_OK) {
            printf("Wrong card holder name, please try again\n");
        }
        while (getCardExpiryDate(&cardData) != CARD_OK) {
            printf("Wrong card expiry date, please try again\n");
        }
        while (getCardPAN(&cardData) != CARD_OK && isValidCardPAN(&cardData) != TERMINAL_OK) {
            printf("Wrong card PAN, please try again\n");
        }

        while (getTransactionDate(&termData) != TERMINAL_OK) {
            printf("Wrong transaction date, Trying again\n");
        }
        if (isCardExpired(&cardData, &termData) == EXPIRED_CARD) {
            printf("Card is expired\n");
            return;
        }

        while (setMaxAmount(&termData, 5000) != TERMINAL_OK) {
            printf("Wrong max amount, Trying again\n");
        }

        while (getTransactionAmount(&termData) != TERMINAL_OK) {
            printf("Wrong transaction amount, Trying again\n");
        }

        if (isBelowMaxAmount(&termData) != TERMINAL_OK) {
            printf("Transaction amount is above the maximum amount, Transaction declined\n");
            return;
        }

        ST_transaction transData;
        transData.cardHolderData = cardData;
        transData.terminalData = termData;

        transStat = recieveTransactionData(&transData);

        /*if (transStat != APPROVED) {
            if (transStat == FRAUD_CARD) {
                printf("PAN is not found\n");
                continue;
            } else if (transStat == DECLINED_INSUFFECIENT_FUND) {
                printf("Insuffecient funds\n");
                continue;
            } else if (transStat == DECLINED_STOLEN_CARD) {
                printf("Card is stolen\n");
                continue;
            } else {
                printf("Transaction is declined due to internal server error\n");
                continue;
            }
        }
        printf("Transaction is approved, Thanks\n");
        break;*/
}
