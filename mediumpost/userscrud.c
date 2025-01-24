# include <stdio.h>
# include <sqlite3.h>

int main(){
  // Create the sqlite3 object
  sqlite3 *db;

  // Open a database connection
  const char *db_file_name = "pets_db";
  int rc = sqlite3_open_v2(db_file_name,
                            &db,
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                            NULL); // no custom VFS module

  // If successful, response code should be SQLITE_OK ==> 0
  if (rc == SQLITE_OK) {
    printf("DB file created successfully: rc %d\n", rc);
  } else {
    printf("Failed to open db file: rc %d msg %s", rc, sqlite3_errmsg(db));
    return rc;
  }
  //Create USERS Table
  const char *sql_create = "CREATE TABLE IF NOT EXISTS USERS("
                                "id INT PRIMARY KEY,"
                                "name TEXT,"
                                "created TEXT,"
                                "archived INT"
                                ");";
  char *create_table_err = 0;
  int create_exec_rc = sqlite3_exec(db,sql_create,0,0,&create_table_err);
  if (create_exec_rc == SQLITE_OK) {
        printf("\nTable created successfully rc:%d\n", create_exec_rc);
  } else {
        printf("\nFailed to create table USERS (rc %d, msg %s)\n", create_exec_rc, create_table_err);
        return create_exec_rc;
  }
  // Create SELECT sqlite3_stmt 
  sqlite3_stmt *stmt;
  const char *sql_query = "SELECT * FROM USERS"
                           " WHERE id = ?";
  int prep_rc = sqlite3_prepare_v2(db,sql_query,-1,&stmt,0);
  if (prep_rc == SQLITE_OK) {
    printf("Finished preparing sql stmt INSERT rc: %d\n", prep_rc);
  } else {
    printf("Failed to prep stmt rc:%d errMsg %s\n", prep_rc, sqlite3_errmsg(db));
    return prep_rc;
  }

  // Bind Parameters
  int user_id = 5;
  int parameter_index = 1;
  int bind_rc = sqlite3_bind_int(stmt, parameter_index, user_id);

  if (bind_rc == SQLITE_OK) {
      printf("Finished binding parameters for SELECT query rc %d\n",bind_rc);
  } else {
      printf("Failed to bind parameters rc %d, errmsg %s\n", bind_rc, sqlite3_errmsg(db));
      return bind_rc;
  }

  // Execute query with sqlite3_step
  int select_rc;
  while( (select_rc = sqlite3_step(stmt)) == SQLITE_ROW) {
      printf("Found a row of data for processing with sqlite3_column\n");
      // Process each row of data in result set with sqlite3_column_*
      // Assuming table has:
      // user.id | user.name | user.created_date | user.archived |
      int id_col = 0;
      int name_col = 1;
      int created_col = 2;
      int archived_col = 3;

      int user_id = sqlite3_column_int(stmt, id_col);
      const unsigned char *user_name = sqlite3_column_text(stmt, name_col);
      const unsigned char *created = sqlite3_column_text(stmt, created_col);
      const int archived = sqlite3_column_int(stmt, archived_col);
      
      printf("User: (\nid %d\n"
              "name %s\n"
              "created Date %s\n"
              "archived status %s\n)",
              user_id, user_name, created, (archived == 1 ? "TRUE" : "FALSE")
            );
  }
  printf("Finished processing rows...");
  
  // Reset sqlite3_stmt | Unbind parameters
  int reset_rc = sqlite3_reset(stmt);
  if (reset_rc == SQLITE_OK) {
      printf("\nSQLite3 Stmt reset was successful rc: %d\n", reset_rc);
  } else {
      printf("\nFailed to reset stmt rc:%d MSG:%s\n", 
                                      reset_rc, sqlite3_errmsg(db));
      return reset_rc;
  }

  int finalize_rc = sqlite3_finalize(stmt);
  if (finalize_rc == SQLITE_OK) {
	printf("\nFinalized sqlite3 statement rc %d\n", finalize_rc);
  } else {
	printf("\nFailed to finalize sqlite3 statement (rc %d, msg %s)",finalize_rc,sqlite3_errmsg(db));
	return finalize_rc;
  }
  // Always close database connection to free resources
  int cls_rc = sqlite3_close(db);
  if (cls_rc == SQLITE_OK) {
    printf("DB file is closed ✅ : rc %d\n", cls_rc);
  } else {
    printf("Failed to close DB : rc %d errMsg %s", cls_rc, sqlite3_errmsg(db));
  }
  return cls_rc;
}
