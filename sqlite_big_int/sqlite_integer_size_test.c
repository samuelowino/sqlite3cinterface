#include <stdio.h>
#include <sqlite3.h>

int main() {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = 0;
    int rc;

    // Open in-memory SQLite DB
    rc = sqlite3_open(":memory:", &db);
    if (rc) {
        fprintf(stderr, "Can't open DB: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Create table
    sqlite3_exec(db, "CREATE TABLE TestInts (label TEXT, value INTEGER);", NULL, NULL, &errMsg);

    // Just below 53-bit safe integer range
    sqlite3_exec(db, "INSERT INTO TestInts (label, value) VALUES ('safe', -9007199254740991);", NULL, NULL, &errMsg);

    // Beyond 53-bit precision (still valid 64-bit signed int)
    sqlite3_exec(db, "INSERT INTO TestInts (label, value) VALUES ('unsafe', -8417369560160395865);", NULL, NULL, &errMsg);

    // Select and show values
    const char *sql = "SELECT label, value, typeof(value) FROM TestInts;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    printf("label\t\tvalue\t\t\t\ttypeof\n");
    printf("---------------------------------------------------------------\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *label = (const char *)sqlite3_column_text(stmt, 0);
        sqlite3_int64 value = sqlite3_column_int64(stmt, 1);
        const char *type = (const char *)sqlite3_column_text(stmt, 2);

        printf("%-10s\t%20lld\t%s\n", label, value, type);
    }

    /**
     * 
     *  label        value               typeof
     *   ---------------------------------------------------------------
     *   safe           -9007199254740991    integer
     *   unsafe      -8417369560160395865    integer
     *
     **/

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

// Compile and Run w/
//gcc sqlite_integer_size_test.c -o sqlite_test -lsqlite3
//./sqlite_test

