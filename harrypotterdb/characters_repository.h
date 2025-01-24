# ifndef character_repository.h
# define character_repository.h

# define CHARACTERS_TABLE  = "All_Characters";

/*
	* Create the sqlite databas table for characters
	* Only creates the table if does not already exist
	* 
	* Parameters:
	* A pointer to an open sqlite3 database 
	*
	* SCHEMA:
	* 	CREATE TABLE Characters (
    	*		id UUID PRIMARY KEY,
    	*		name VARCHAR(255) NOT NULL,
    	*		species VARCHAR(50),
    	*		gender VARCHAR(10),
    	*		house VARCHAR(50),
    	*		date_of_birth DATE,
    	*		year_of_birth INT,
    	*		wizard BOOLEAN,
    	*		ancestry VARCHAR(50),
    	*		eye_colour VARCHAR(50),
   	*		hair_colour VARCHAR(50),
   	*		patronus VARCHAR(100),
    	*		hogwarts_student BOOLEAN,
    	*		hogwarts_staff BOOLEAN,
    	*		actor VARCHAR(255),
    	*		alive BOOLEAN,
    	*		image_url TEXT
	*	);
	*
	* RETURNS:
	* int result_code (rc)	SQLITE_OK, SQLITE_ERROR ....
	*
*/
int create_table(sqlite3 *db);

#endif 
