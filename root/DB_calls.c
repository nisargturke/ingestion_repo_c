#include <stdio.h>
#include <sqlite3.h>

int main() {
    sqlite3 *db;
    char *err_msg = 0;
    int rc;

    // Open database (creates file if it doesn't exist)
    rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Create table
    const char *sql_create = "CREATE TABLE IF NOT EXISTS Users(Id INT, Name TEXT);";
    rc = sqlite3_exec(db, sql_create, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Insert data
    const char *sql_insert = "INSERT INTO Users VALUES(1, 'Alice');";
    rc = sqlite3_exec(db, sql_insert, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Query data
    const char *sql_select = "SELECT * FROM Users;";
    sqlite3_stmt *res;

    rc = sqlite3_prepare_v2(db, sql_select, -1, &res, 0);

    if (rc == SQLITE_OK) {
        printf("Id\tName\n");
        while (sqlite3_step(res) == SQLITE_ROW) {
            printf("%d\t%s\n", sqlite3_column_int(res, 0), sqlite3_column_text(res, 1));
        }
    } else {
        printf("Failed to fetch data: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return 0;
}