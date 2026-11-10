#ifndef DB_H
#define DB_H

#include "sqlite3.h"

sqlite3 *db_open(const char *filename);
void db_close(sqlite3 *bank_db);
int db_init_tables(sqlite3 *bank_db);
int db_add_account(sqlite3 *bank_db, int id, const char *name, double balance);
int db_update_balance(sqlite3 *bank_db, int id, double balance);
int db_delete_account(sqlite3 *bank_db, int id);
void db_print_accounts(sqlite3 *bank_db);

#endif