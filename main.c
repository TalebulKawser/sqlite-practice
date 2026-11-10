#include <stdio.h>
#include "db.h"

int main(void)
{

    sqlite3 *db = db_open("bank.db");
    if (db == NULL)
    {
        return 1;
    }

    db_init_tables(db);

    db_add_account(db, 102, "Talebul Kawser", 6899.00);
    db_add_account(db, 103, "K M Rayhan", 3860.00);
    db_add_account(db, 104, "Rakibul Hasan", 4800.00);
    db_add_account(db, 105, "Tanvir Reza", 10000.00);
    db_print_accounts(db);

    int changed = db_update_balance(db, 105, 5620.69);
    if (changed == 0)
    {
        printf("No such account exists in this db.\n");
    }
    db_print_accounts(db);

    db_delete_account(db, 102);
    db_print_accounts(db);

    db_close(db);
    return 0;
}