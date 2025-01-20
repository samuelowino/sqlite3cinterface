# include <stdio.h>
# include <sqlite3.h>
/*
	* Include SQLite 3 library
	* I might include a make file for linking or
	* just a .sh file with the compile-link-run command
**/

// FUNCTION PROTOTYPES
sqlite3* init_db();
int create_tvshows_table_if_not_exist(sqlite3 *db);
int insert_tvshows(sqlite3 *db);
int create_table_characters_if_not_exist(sqlite3 *db);
int main(){
	sqlite3 *db = init_db();
	int create_table_rc = create_tvshows_table_if_not_exist(db);
	printf("Create table result code:%d MSG: %s\n", create_table_rc, sqlite3_errmsg(db));
	int create_chars_table_rc = create_table_characters_if_not_exist(db);
	printf(
		"\n===============================================================\n"
		"Transactions Summary\tResponse Code\n"
		"===============================================================\n"
		"Create Table TV Shows\t rc = %d\n"
		"*****************************************************************\n"
		"Create Table Characters\t rc = %d\n"
		"*****************************************************************\n",
		create_table_rc, 
		create_chars_table_rc
		);
	//Done with Ops close db
	int close_rc = sqlite3_close(db);
	if (close_rc == SQLITE_OK){
		printf("Close database successful: rc %d\n", close_rc);
	} else {
		printf("Close database failed with error: rc %d, MSG: %s\n", close_rc, sqlite3_errmsg(db));
	}
	return 0;
}
sqlite3* init_db(){
	// Initialize sqlite3 database 
	// The SQLite3 database object is responsible for creating a connection
	// to the database file and for executing SQL statements on the db
	sqlite3 *db;
	// Initialize a variable for storing the return codes of the sqlite3 function calls
	// Typical return codes include the following
	/*
		* SQLite Return Codes:
 		* 
 		* Return Code   Macro Name        Description
 		* ------------------------------------------------------------
 		* 0             SQLITE_OK         Operation completed successfully.
 		* 1             SQLITE_ERROR      SQL error or missing database.
 		* 2             SQLITE_INTERNAL   Internal logic error in SQLite.
 		* 3             SQLITE_PERM       Access permission denied.
 		* 5             SQLITE_BUSY       The database file is locked.
 		* 6             SQLITE_LOCKED     A table in the database is locked.
 		* 7             SQLITE_NOMEM      A malloc() failed.
 		* 14            SQLITE_CANTOPEN   Unable to open the database file.
 		* 21            SQLITE_MISUSE     Library used incorrectly.
 		* 100           SQLITE_ROW        A result row is available (used with sqlite3_step()).
 		* 101           SQLITE_DONE       SQL statement execution is complete (used with sqlite3_step()).
 	*/
	int rc;
	// Open the database connection with sqlite3_open()
	// The function requres two arguments; 
	// 	1. The name of the database file
	//	2. The sqlite3 database object
	/*
		* The sqlite3_open function is used to open a connection to an SQLite database file.
 		* Purpose, syntax, behavior, and common use cases:
		*
 		* Purpose:
 		* - To establish a connection to an SQLite database file.
 		* - If the specified database file does not exist, SQLite will attempt to create it.
 		* - Returns a pointer to a database connection object (sqlite3*) that can be used
 		*   for subsequent database operations.
 		*
 		* Function Signature:
 		* int sqlite3_open(const char *filename, sqlite3 **ppDb);
 		*
 		* Parameters:
 		* 1. filename:
 		*    - The name of the database file to open or create.
 		*    - Special values:
 		*      - ":memory:": Creates a temporary in-memory database.
 		*      - "file::memory:?cache=shared": Creates a shared in-memory database.
 		*    - Can include URI parameters if URI support is enabled in SQLite.
 		* 
 		* 2. ppDb:
 		*    - A pointer to a pointer to an sqlite3 object.
 		*    - On success, this pointer is set to the database connection object.
 		*    - On failure, it is set to NULL.
 		*
 		* Return Value:
 		* - An integer that represents the success or failure of the operation.
 		* - Common return codes:
 		*   - SQLITE_OK (0): Success.
 		*   - SQLITE_CANTOPEN (14): The database file could not be opened.
 		*   - SQLITE_NOMEM (7): Memory allocation failed.
 		*   - SQLITE_ERROR (1): A general error occurred.
 	*/
	rc = sqlite3_open("startrektime.db",&db);
	printf("Attempted to open sqlite database: response code %d\n", rc);
	if (rc != SQLITE_OK){
		/*
 			* The sqlite3_errmsg() function is a part of the SQLite library and is used to retrieve 
 			* a human-readable string describing the most recent error that occurred in a database connection.
 			*
 			* Function Signature:
 			* const char *sqlite3_errmsg(sqlite3 *db);
 			*
 			* Purpose:
 			* - Provides detailed error messages to help developers diagnose issues with database operations.
 			* - Useful for debugging or logging when an SQLite function returns an error code.
 			*
 			* Parameters:
 			* 1. sqlite3 *db:
 			*    - A pointer to the database connection object for which the error message is retrieved.
 			*    - This must be the same connection object that experienced the error.
 			*
 			* Return Value:
 			* - A const char* pointer to a null-terminated UTF-8 string containing the error message.
 			* - The error message is generated by SQLite and is specific to the database connection provided.
 			*
 			* Behavior:
 			* - Scope:
 			*   - The error message pertains to the last API call on the given database connection (db) that failed.
 			*   - If no error has occurred, sqlite3_errmsg() returns "not an error".
 			* 
 			* - Memory:
 			*   - The string returned is managed by SQLite and remains valid until the next API call on the 
 			*     same database connection.
 			*   - Do not free or modify the returned string.
 		*/
		printf("Failed to open database file: Error %s\n", sqlite3_errmsg(db));
		// ALWAYS CLOSE THE DATABASE CONNECTION WITH: sqlite3_close(db);
		/*
 			* The sqlite3_close() function is used to close a connection to an SQLite database 
 			* and free all associated resources. It is a crucial part of managing the lifecycle 
 			* of a database connection to ensure proper cleanup and prevent resource leaks.
 			*
 			* Function Signature:
 			* int sqlite3_close(sqlite3 *db);
 			*
 			* Purpose:
 			* - To close a database connection that was previously opened using sqlite3_open() 
 			*   or sqlite3_open_v2().
 			* - Ensures that all resources, including memory allocations and file handles, are released.
 			* - Prevents database file corruption by ensuring that any pending changes are properly finalized.
 			*
 			* Parameters:
 			* 1. sqlite3 *db:
 			*    - A pointer to the database connection object to close.
 			*    - Must be a valid database connection that was opened earlier.
 			*    - If db is NULL, the function does nothing and returns SQLITE_OK.
 			*
 			* Return Value:
 			* - An integer indicating the result of the operation:
 			*   - SQLITE_OK (0): The database connection was successfully closed.
 			*   - SQLITE_BUSY (5): The connection could not be closed because there are 
 			*     unfinalized prepared statements, active transactions, or other resources still in use.
	 	*/

		int close_rc = sqlite3_close(db);
		printf("Closing db with result code: %d\n", close_rc);
		return NULL;
	} else {
		printf("Open Database Successfully, can start reading/writing to db file");
		return db;
	}
}
int create_tvshows_table_if_not_exist(sqlite3 *db){
	/*
 		* SQLite provides a C API for interacting with SQLite databases, which includes 
 		* preparing and executing SQL statements using the sqlite3_stmt interface. This 
 		* allows you to execute SQL statements and retrieve results programmatically in a 
 		* more efficient manner compared to running ad-hoc SQL queries directly.
 		*
 		* Key Concepts:
 		* 1. Database Connection (sqlite3):
 		*    You need to open a connection to the SQLite database.
 		* 
 		* 2. Prepared Statement (sqlite3_stmt):
 		*    A prepared statement is a SQL statement that is compiled and ready to be 
 		*    executed multiple times efficiently.
 		*
 		* 3. Executing Prepared Statements:
 		*    Use placeholders (?) instead of actual values, and bind parameters to these 
 		*    placeholders.
 		* 
 		* 4. Result Handling:
 		*    After execution, use functions to retrieve results from the statement (e.g., 
 		*    retrieving rows, numbers, text).
 	*/
	sqlite3_stmt *create_stmt;
	const char *create_sql = "CREATE TABLE IF NOT EXISTS tvshows ("
                         "id INTEGER PRIMARY KEY, "
                         "title TEXT, "
                         "release_date TEXT, "
                         "star_date TEXT, "
                         "end_star_date TEXT);";
	/*
 		* The function sqlite3_prepare_v2 is a crucial part of SQLite's C API, specifically 
 		* for preparing SQL statements for execution. It compiles a given SQL statement into 
 		* a bytecode representation, which is then executed by the SQLite engine. This allows 
 		* you to execute the same SQL statement multiple times with different parameters, 
 		* improving efficiency and security.
 		*
 		* Function Prototype:
 		* int sqlite3_prepare_v2(
 		*   sqlite3 *db,            // The database handle
 		*   const char *sql,        // The SQL statement
 		*   int nByte,              // The length of the SQL statement (or -1)
 		*   sqlite3_stmt **ppStmt,  // The prepared statement handle (output)
 		*   const char **pzTail     // Pointer to unused portion of the SQL (output)
 		* );
 		*
 		* Parameters:
 		* sqlite3 *db:
 		*   This is a pointer to the SQLite database object (sqlite3 *), which represents 
 		*   an open database connection. This connection is required to prepare SQL 
 		*   statements.
 		*
 		* const char *sql:
 		*   This is the SQL statement you want to prepare. It must be a string containing 
 		*   a valid SQL query. The query can contain placeholders (?) for parameters that 
 		*   you can bind later using functions like sqlite3_bind_*.
 		*
 		* int nByte:
 		*   This parameter specifies the number of bytes in the SQL query. If the SQL 
 		*   statement is a null-terminated string, you can pass -1, and SQLite will 
 		*   automatically calculate the length of the string using strlen().
 		*   If nByte is positive, it indicates the exact length of the SQL string, 
 		*   allowing for partial strings (e.g., if you're passing a portion of a larger query).
 		*
 		* sqlite3_stmt **ppStmt:
 		*   This is an output parameter that will receive the prepared statement object 
 		*   (sqlite3_stmt *). This object represents the compiled SQL statement that can 
 		*   be executed multiple times.
 		*   Once the statement is prepared, you can execute it using sqlite3_step() and 
 		*   retrieve the results.
 		*
 		* const char **pzTail:
 		*   This is an optional output parameter. If there are extra unused SQL statements 
 		*   after the first SQL statement (for example, multiple SQL statements in one 
 		*   string), this will point to the tail of the SQL string after the first statement.
 		*   If there is only one statement in the string, pzTail will be set to NULL.
 		*   If multiple SQL statements are provided, this can be used to identify and 
 		*   extract the remaining SQL commands for subsequent preparation.
 		*
 		* Return Value:
 		* - SQLITE_OK: Success (0).
 		* - SQLITE_ERROR: A generic error occurred, and sqlite3_errmsg() can be used to 
 		*   get the specific error message.
 		* - SQLITE_NOMEM: There was not enough memory to prepare the statement.
 		* - SQLITE_BUSY: The database is locked and is busy.
 		* - SQLITE_MISUSE: A programming error (e.g., calling this function with invalid arguments).
 		* - SQLITE_INTERNAL: An internal SQLite error.
 		*
 		* How sqlite3_prepare_v2 Works:
 		* - Parsing: SQLite parses the provided SQL string and checks it for syntax errors. 
 		*   If any errors are found, the function will return SQLITE_ERROR and set the 
 		*   appropriate error message.
 		* - Compilation: If no syntax errors are found, SQLite compiles the SQL query into 
 		*   an internal bytecode representation. This bytecode is an intermediate form that 
 		*   the SQLite engine can execute efficiently. The bytecode is stored in the 
 		*   sqlite3_stmt object returned in ppStmt.
 		* - Parameterization: If the SQL contains placeholders (e.g., ? for parameters), 
 		*   the prepared statement can be executed multiple times with different parameter 
 		*   values. You can later bind values to the placeholders using sqlite3_bind_* functions.
 		* - Memory Management: After preparation, the bytecode is stored in the memory for 
 		*   execution, and the original SQL string is no longer required. The prepared 
 		*   statement (sqlite3_stmt) must be finalized using sqlite3_finalize() when you 
 		*   are done with it.
 	*/
	int prep_create_rc = sqlite3_prepare_v2(db,create_sql,-1,&create_stmt,0);
	if (prep_create_rc != SQLITE_OK) {
		if (prep_create_rc == 1) {
			printf("\nAlready created table: rc %d MSG %s\n",prep_create_rc, sqlite3_errmsg(db));
		} else {
			printf("\nPrepare sql statement failed with error %s\nRC:%d\n", sqlite3_errmsg(db),prep_create_rc);
			return prep_create_rc;
		}
	} else {
		printf("\nPrepare sql statement successful, you can proceed to bind and exec the statement: RC%d\n",prep_create_rc);
	}
	/*
 		* The sqlite3_step function is a core part of the SQLite C API, responsible for 
 		* executing a prepared statement (sqlite3_stmt). It processes one "step" of the 
 		* SQL statement at a time. Depending on the statement type (e.g., SELECT, INSERT, 
 		* UPDATE, or DELETE), it may return results, affect rows in the database, or complete 
 		* execution.
 		*
 		* Function Prototype:
 		* int sqlite3_step(sqlite3_stmt *pStmt);
 		*
 		* Parameters:
 		* sqlite3_stmt *pStmt:
 		*   A pointer to a prepared statement object (sqlite3_stmt) that was previously 
 		*   created using sqlite3_prepare_v2.
 		*
 		* Return Values:
 		* The return values of sqlite3_step indicate the current status of the execution process. 
 		* Key return codes include:
 		*
 		* - SQLITE_ROW:
 		*   Indicates that a row of data is available (for SELECT statements).
 		*   You can retrieve column values from the current row using sqlite3_column_* functions 
 		*   (e.g., sqlite3_column_text, sqlite3_column_int). Call sqlite3_step again to move 
 		*   to the next row.
 		*
 		* - SQLITE_DONE:
 		*   Indicates that the SQL statement has completed execution.
 		*   For SELECT statements, this means there are no more rows to return.
 		*   For INSERT, UPDATE, and DELETE statements, this means the changes have been 
 		*   applied successfully.
 		*
 		* - SQLITE_BUSY:
 		*   Indicates that the database is locked, and the statement cannot proceed. This could 
 		*   happen if another process is accessing the database in a way that conflicts with 
 		*   the current statement. You may retry calling sqlite3_step after a short delay or 
 		*   resolve the contention.
 		*
 		* - SQLITE_ERROR:
 		*   A generic error occurred. Use sqlite3_errmsg to get the detailed error message.
 		*
 		* - SQLITE_MISUSE:
 		*   Indicates that the function was called incorrectly (e.g., using a statement after 
 		*   it has been finalized).
 		*
 		* - SQLITE_SCHEMA:
 		*   The database schema has changed. You must re-prepare the statement using 
 		*   sqlite3_prepare_v2.
 		*
 		* - SQLITE_NOMEM:
 		*   Insufficient memory to execute the statement.
 	*/
	int exec_rc = sqlite3_step(create_stmt);
	if (exec_rc == SQLITE_DONE) {
		printf("\nTable created successfuly:RC%d\n",exec_rc);
	} else {
		printf("\nFailed to create table:RC%d, %s\n",exec_rc, sqlite3_errmsg(db));
	}
	/*
 		* The sqlite3_reset() function in SQLite is used to reset a prepared SQL
 		* statement back to its initial state, ready for reuse with different input
 		* values. It is an essential part of the SQLite statement lifecycle, especially
 		* when re-executing a statement in a loop or with different parameter bindings.
 		*
 		* Prototype:
 		*     int sqlite3_reset(sqlite3_stmt *stmt);
 		*
 		* Parameters:
 		*     stmt: A pointer to the prepared statement object (sqlite3_stmt) that you
 		*           want to reset.
 		*
 		* Return Value:
 		*     SQLITE_OK: The reset was successful.
 		*     Any other value indicates an error, typically related to issues
 		*     encountered during the last execution of the statement.
 		*
 		* Function Purpose:
 		*     When you prepare a statement using sqlite3_prepare_v2(), execute it with
 		*     sqlite3_step(), and want to execute it again, you need to:
 		*     
 		*     1. Call sqlite3_reset() to reset the internal state of the statement.
 		*     2. Optionally rebind new values to the parameters using sqlite3_bind_*.
 		*
 		*     sqlite3_reset() is necessary because it clears any result state left over
 		*     from the last execution. Without resetting, reusing the statement could
 		*     cause undefined behavior or errors.
 		*
	*/
	int reset_rc = sqlite3_reset(create_stmt);
	if (reset_rc != SQLITE_OK) {
		printf("\nFailed to reset stmt:rc %d MSG %s\n", reset_rc, sqlite3_errmsg(db));
	} else {
		printf("\nSQL Stmt reset was successful: rc %d\n", reset_rc);
	}
	/*
 		* The sqlite3_finalize() function in SQLite is used to destroy a prepared SQL
 		* statement object and release all associated resources. It marks the end of
 		* the lifecycle of a prepared statement.
 		*
 		* Prototype:
 		*     int sqlite3_finalize(sqlite3_stmt *stmt);
 		*
 		* Parameters:
 		*     stmt: A pointer to the prepared statement object (sqlite3_stmt) that you
 		*           want to finalize. If stmt is NULL, sqlite3_finalize() is a no-op
 		*           and returns SQLITE_OK.
 		*
 		* Return Value:
 		*     SQLITE_OK: The statement was successfully finalized.
 		*     Any other value indicates an error that was encountered during the last
 		*     execution of the statement.
 		*
 		* Purpose:
 		*     When you prepare a SQL statement using sqlite3_prepare_v2(), SQLite
 		*     allocates resources such as memory and execution context for the
 		*     statement. These resources remain allocated until the statement is
 		*     finalized with sqlite3_finalize().
 		*
 		*     The function:
 		*     - Frees all resources associated with the statement.
 		*     - Invalidates the sqlite3_stmt object, making it unusable.
 		*     - Ensures memory is properly managed, avoiding resource leaks.
 	*/
	int fin_rc = sqlite3_finalize(create_stmt);
	if (fin_rc == SQLITE_OK) {
		printf("\nSQL statement clean up done...finalized with rc %d\n", fin_rc);
	} else {
		printf("\nFailed to finalized prepared statement: rc %d MSG %s\n", fin_rc, sqlite3_errmsg(db));
		return fin_rc;
	}
	return 0;
}
int insert_tvshows(sqlite3 *db){
	return 0;
}
int create_table_characters_if_not_exist(sqlite3 *db){
	int rc = 0;
	char *errmsg = 0;
	const char *create_sql = "CREATE TABLE IF NOT EXISTS Show_Characters("
					"id INT PRIMARY KEY," 
					"name TEXT," 
					"traits TEXT);";
	/* 
 		* The sqlite3_exec function is a high-level convenience function provided 
 		* by the SQLite C library. It allows you to execute SQL statements directly 
 		* on an SQLite database connection. This function is particularly useful 
 		* for simple operations like creating tables, inserting data, or running 
 		* queries without needing to prepare and manage SQL statements explicitly.
 		* 
 		* Here’s a breakdown of how sqlite3_exec works:
 		* 
 		* Function Signature:
 		* int sqlite3_exec(
 		*   sqlite3 *db,                 // Database connection
 		*   const char *sql,             // SQL statement(s)
 		*   int (*callback)(void*,int,char**,char**), // Callback function
 		*   void *arg,                   // 1st argument to callback
 		*   char **errmsg                // Error message string
 		* );
 		* 
 		* Parameters:
 		* 
 		* sqlite3 *db
 		*   A pointer to the SQLite database connection object, created using sqlite3_open.
 		* 
 		* const char *sql
 		*   A string containing one or more SQL statements to be executed. 
 		*   Statements are separated by semicolons (;).
 		* 
 		* int (*callback)(void*, int, char**, char**)
 		*   A callback function that is called for each row in the result set 
 		*   (optional; NULL if not used).
 		*   - Arguments to the callback:
 		*     - void*: Pointer to the user-defined data (passed via arg).
 		*     - int: Number of columns in the result set.
 		*     - char**: Array of strings representing column values.
 		*     - char**: Array of strings representing column names.
 		* 
 		* void *arg
 		*   A pointer to user data passed to the callback. If no data is needed, pass NULL.
 		* 
 		* char **errmsg
 		*   A pointer to a string that will be set to an error message if an error occurs. 
 		*   If you don't need the error message, you can pass NULL. Otherwise, remember to 
 		*   free the error message using sqlite3_free when done.
 	*/
	rc = sqlite3_exec(db,create_sql,0,0,&errmsg);
	if (rc != SQLITE_OK) {
		printf("Failed to create table Show_Characters: rc %d : MSG %s\n",rc,errmsg);
		sqlite3_free(errmsg);
		return rc;
	}
	printf("Created Charcacters table successful: rc %d\n", rc);
	sqlite3_free(errmsg);
	return 0;
}
