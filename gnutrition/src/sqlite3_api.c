/***************************************************************************
 *            sqlite3_api.c
 *
 *  Mon Jan  3 00:48:28 2011
 *  Copyright (C) 2011 Free Software Foundation, Inc.
 *
 ****************************************************************************/

/*
 * This file is part of GNUtrition.
 * 
 * GNUtrition is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * GNUtrition is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <sqlite3_api.h>

#include "user_interface.h"

/* Call back is dummy since it is for update query. It can at max check status */
static int update_scalar_callback(void *NotUsed, int num_col, char **argv, char **azColName)
{
  printf("update query executed\n");
	return 0;
}

/* This call back expects a single row as response. */
static int scalar_callback(void *NotUsed, int num_col, char **argv, char **azColName)
{

  int i;
	col_count = num_col;

  for(i=0; i<num_col; i++){
    //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		strcpy(ColNames[i], azColName[i]);

		if(argv[i] == NULL)
		  strcpy(values[i], "NULL");
		else
		  strcpy(values[i],argv[i]);
  }

  //printf("\n");
  return 0;
}

/* This needs to be a high performance callback since it queries Food table with
 * up to 10k rows.
 */
static int callback(void *NotUsed, int num_col, char **argv, char **azColName)
{
  strcpy(values[row_count],argv[0]);

	// Increment the global row counter
  row_count++;
  //printf("\n");
  return 0;
}

static int version_callback(void *NotUsed, int num_col, char **argv, char **azColName)
{

  int i;

  for(i=0; i<num_col; i++){
    //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if(strcmp(azColName[i], "GNUtrition") == 0) {
			//printf("GNU version %s", gnutrition_version);
			strcpy(gnutrition_version, argv[i]);
		}
		
		else if(strcmp(azColName[i], "SR") == 0)
			strcpy(database_version, argv[i]);
  }

  return 0;
}

int sqlite3_run_query(int argc, char argv[][MAX_QUERY_STRING_LEN])
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

	// Initiaze the global row and column counters
  row_count=0;
	col_count=0;
	
  if( argc!=2 ){
    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
    exit(1);
  }

  rc = sqlite3_open_v2( argv[0], /* Database filename (UTF-8) */
                       &db,      /* OUT: SQLite db handle */
                        SQLITE_OPEN_READONLY,/* Flags */
                        NULL);  /* Name of VFS module to use */
	
  if( rc ){
    //fprintf(stderr, "Failed: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);

		return SQL_DB_FILE_NOT_FOUND;
    //exit(1);
  }

	if(strstr(argv[1], "select * from food where Food_Des=") != NULL) {
	  rc = sqlite3_exec(db, argv[1], scalar_callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
	}

	/* If the query contains food table then register it's callback */
  else if( strstr(argv[1], "food") != NULL) {
	  rc = sqlite3_exec(db, argv[1], callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
	}

	/* If it's an update query the register respective callback */
	else if(strstr(argv[1], "update") != NULL) {
	  rc = sqlite3_exec(db, argv[1], update_scalar_callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
	}

	else { /* It must be version table so register respective call back. */
	  rc = sqlite3_exec(db, argv[1], version_callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
	}
	
  sqlite3_close(db);
  return 0;
}

int run_basic_query(void)
{
  /* Begin Data base query creation and execution code */

	char prm[2][MAX_QUERY_STRING_LEN];

  strcpy(prm[0], db_file_path);
  //g_print("prm[0] %s\n", prm[0]);
  strcpy(prm[1],"select Food_Des from food");
		
	//char * prm[] = {"test.db", "select * from t"};
	int num_prm = 2;
	return sqlite3_run_query(num_prm, prm);

	/* End Data base query creation and execution code */
}

int run_scalar_query(char * name)
{
  /* Begin Data base query creation and execution code */

	char prm[2][MAX_QUERY_STRING_LEN];

  strcpy(prm[0], db_file_path);
  //g_print("prm[0] %s\n", prm[0]);
  sprintf(prm[1],"select * from food where Food_Des=\'%s\'",name);
		
	//char * prm[] = {"test.db", "select * from t"};
	int num_prm = 2;
	return sqlite3_run_query(num_prm, prm);

	/* End Data base query creation and execution code */
}

int create_exec_version_table_query(void)
{
  char prm[2][MAX_QUERY_STRING_LEN];

	/* For now we are just extracting and displaying the DB and program version */
  char query[MAX_QUERY_STRING_LEN] = {"select * from version"};
	
  strcpy(prm[0], db_file_path);
  //g_print("prm[0] %s\n", prm[0]);
  strcpy(prm[1],query);
  //g_print("prm[1] %s\n", prm[1]);

  int num_prm = 2;
	
  return  sqlite3_run_query(num_prm, prm);
}

int run_update_query(char *col_name, char *value, char *food_name)
{
  /* Begin Data base query creation and execution code */

	char prm[2][MAX_QUERY_STRING_LEN];

  strcpy(prm[0], db_file_path);
  //g_print("prm[0] %s\n", prm[0]);
  sprintf(prm[1],"update food SET %s=\'%s\' where Food_Des=\'%s\'",
	  col_name, value, food_name);
		
	//char * prm[] = {"test.db", "select * from t"};
	int num_prm = 2;
	return sqlite3_run_query(num_prm, prm);

	/* End Data base query creation and execution code */
}

