#include <stdio.h>
#include "db.h"

sqlite3 *db_open(const char *filename)
{
    sqlite3 *bank_db;
    int rc = sqlite3_open(filename, &bank_db);
    if (rc != SQLITE_OK)
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(bank_db));
        sqlite3_close(bank_db);
        return NULL;
    }
    return bank_db;
}

void db_close(sqlite3 *bank_db)
{
    sqlite3_close(bank_db);
}

int db_init_tables(sqlite3 *bank_db)
{
    const char *sql =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "id INTEGER PRIMARY KEY, name TEXT, balance REAL);";
    char *errMsg = NULL;
    if (sqlite3_exec(bank_db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
    {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return -1;
    }
    return 0;
}

int db_add_account(sqlite3 *bank_db, int id, const char *name, double balance)
{
    const char *sql = "INSERT INTO accounts (id, name, balance) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    int result = 0;
    int rc = sqlite3_prepare_v2(bank_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Preparation failed while adding account: %s\n", sqlite3_errmsg(bank_db));
        return -1;
    }
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, balance);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Insert failed: %s\n", sqlite3_errmsg(bank_db));
        result = -1;
    }
    sqlite3_finalize(stmt);
    return result;
}

int db_update_balance(sqlite3 *bank_db, int id, double balance)
{
    const char *sql = "UPDATE accounts SET balance = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    int result;

    if (sqlite3_prepare_v2(bank_db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Preparation failed while updating: %s\n", sqlite3_errmsg(bank_db));
        return -1;
    }
    sqlite3_bind_double(stmt, 1, balance);
    sqlite3_bind_int(stmt, 2, id);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Update failed: %s\n", sqlite3_errmsg(bank_db));
        result = -1;
    }
    else
    {
        result = sqlite3_changes(bank_db);
    }
    sqlite3_finalize(stmt);
    return result;
}

int db_delete_account(sqlite3 *bank_db, int id)
{
    const char *sql = "DELETE FROM accounts WHERE id = ?;";
    sqlite3_stmt *stmt;
    int result;

    if (sqlite3_prepare_v2(bank_db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Preparation failed while deleting: %s\n", sqlite3_errmsg(bank_db));
        return -1;
    }
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Delete failed: %s\n", sqlite3_errmsg(bank_db));
        result = -1;
    }
    else
    {
        result = sqlite3_changes(bank_db);
    }

    sqlite3_finalize(stmt);
    return result;
}

void db_print_accounts(sqlite3 *bank_db)
{
    const char *sql = "SELECT * FROM accounts";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(bank_db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Preparation failed when printing: %s\n", sqlite3_errmsg(bank_db));
        return;
    }
    printf("\n--- Registered Accounts ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *)sqlite3_column_text(stmt, 1);
        double balance = sqlite3_column_double(stmt, 2);

        printf("Account ID %d: %s, Balance: %.2f\n", id, name, balance);
    }

    sqlite3_finalize(stmt);
}