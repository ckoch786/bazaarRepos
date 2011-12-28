/***************************************************************************
 *            file_handling.c
 *
 *  Sun Feb  6 21:38:19 2011
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
#include<string.h>

#include "user_interface.h"
#include "sqlite3_api.h"


void create_location_file(void)
{
  FILE * file_ptr;
	char * home_pth;
	char loc_file_path[LOC_FILE_PATH_LENGTH];

	home_pth = getenv("HOME");

	if(home_pth == NULL)
	{
		g_print ("Cannot locate user home\n");
		return;
	}	
	
	sprintf(loc_file_path, "%s/%s/%s", home_pth, DB_FILE_CONTAINED_FOLDER,
	  LOCATION_FILE_NAME);

	file_ptr = fopen(loc_file_path, "w");

	if (file_ptr == NULL)
	{
		g_print("Failed to create file\n");
		return;
	}

	fputs(db_file_path, file_ptr);
	fclose(file_ptr);
}

int default_db_file_path(void)
{
	char * home_pth;
	home_pth = getenv("HOME");

	if(home_pth == NULL)
	{
		printf("Cannot locate user home\n");
		return SQL_USER_HOME_NOT_FOUND;
	}	
	
	sprintf(db_file_path, "%s/%s/%s", home_pth, DB_FILE_CONTAINED_FOLDER,
	  DB_FILE_NAME);
	return 0;
}

int get_location_file(void)
{
	char loc_file[LOC_FILE_PATH_LENGTH];
	int status = -1;

	char * home_pth;
	home_pth = getenv("HOME");

	if(home_pth == NULL)
	{
		printf("Cannot locate user home\n");
		status = SQL_USER_HOME_NOT_FOUND;
	}	
	sprintf(loc_file, "%s/%s/%s", home_pth, DB_FILE_CONTAINED_FOLDER, LOCATION_FILE_NAME);

	//g_print("Path to loc file %s\n", loc_file);
	
	FILE * fptr = fopen(loc_file, "r");

	if(fptr) { // if file exists
	  fgets(db_file_path, DB_FILE_PATH_LENGTH, fptr);
  	//g_print("Database file location found in loc file%s\n", db_file_path);

    fclose(fptr);
		// Mark success
		status = TRUE;
	}

	return status;
}

int move_db_file(char * dst, char *src)
{
  char command[100];
  char dest_path[100];
  int x;

	if(strcmp(dst, src) == 0) {
	  g_print("Source and Destination are same\n");
    return EXIT_SUCCESS; 
	}
		  
	int len_of_db_file_name = strlen(DB_FILE_NAME);

	/* Make a local copy of path since we would trim it to contain folder location only. */
	strcpy(dest_path, dst);

	// TODO
  /* Ensure that source contains file name and destination is only folder location. */
 	int len_of_dst = strlen(dst);

	/* If dst includes file name appended at the end, remove it since we only need
	 * path to new folder.
	 */
	if(strcmp(& dest_path[len_of_dst - len_of_db_file_name], DB_FILE_NAME) == 0)
	  dest_path[len_of_dst - len_of_db_file_name] = '\0';
  
  sprintf(command, "mv \'%s\' \'%s\'", src,dest_path);
  //g_print("calling system(%s)\n", command);
  x = system(command);
  //printf("system() returned %d\n", x);
  if (x != EXIT_SUCCESS) {
    g_print("Failed moving\n");
    return EXIT_FAILURE;
  }

	return EXIT_SUCCESS;
}
