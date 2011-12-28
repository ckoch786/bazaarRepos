/***************************************************************************
 *            sqlite3_api.h
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

#ifndef SQLITE3_API_H
#define SQLITE3_API_H

#define MAX_ROWS           10000
#define MAX_COLUMNS          200
#define MAX_COL_NAME_LEN     100
#define MAX_QUERY_STRING_LEN 400

#define SQL_DB_FILE_NOT_FOUND      -1
#define SQL_USER_HOME_NOT_FOUND    -2

/* Following structures will always contain query results, are filled by
 * api's in sqlite and used by user interface.
 */
char ColNames[MAX_COLUMNS][MAX_COL_NAME_LEN];
char values[MAX_ROWS][MAX_COLUMNS];
int col_count;
int row_count;

int sqlite3_run_query(int argc, char argv[][MAX_QUERY_STRING_LEN]);
int sqlite3_version_table_query(int argc, char argv[][MAX_QUERY_STRING_LEN]);
int run_basic_query(void);
int run_scalar_query(char * name);
int create_exec_version_table_query(void);
int run_update_query(char *col_name, char *value, char *food_name);

#endif
