//
// Created by mohamed on 22/12/22.
//
#include "application/application.h"

int main() {
    uint32_t seq = 0;
    ST_accountsDB_t account;
    account.balance = 1000;
    account.state = BLOCKED;
    strcpy(account.primaryAccountNumber, "11652943083297258");
    FILE *fptr = fopen("../Sequence_number.txt", "r+");
    fwrite(&seq, sizeof(uint32_t), 1, fptr);
    fclose(fptr);
    fptr = fopen("../Accounts_DB.txt", "a");
    fwrite(&account, sizeof(ST_accountsDB_t), 1, fptr);
    fclose(fptr);

    appStart();
    return 0;
}
