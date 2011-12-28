/***************************************************************************
 *            user_interface.h
 *
 *  Sun Jan  2 21:38:19 2011
 *  Copyright (C) 2011 Free Software Foundation, Inc.
 *  <root@fayyazlinux>
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

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

#include <gtk/gtk.h>


#define DB_FILE_PATH_LENGTH         100
#define LOC_FILE_PATH_LENGTH        100
#define DB_FILE_CONTAINED_FOLDER    ".gnutrition"
#define DB_FILE_NAME                "database"
#define LOCATION_FILE_NAME          "location"
#define VERSION_STRING_LEN          10


GtkTextBuffer *food_db_buffer;

/* The tag used to pick which entry was clicked in search area. */
GtkTextTag *hlink;

char db_file_path[DB_FILE_PATH_LENGTH];
char gnutrition_version[VERSION_STRING_LEN];
char database_version[VERSION_STRING_LEN];

void	UI_initialize(int *, char *[]);
void create_location_file(void);
void update_file_path_pref(void);
const gchar* get_search_text_for_query(char * entry_name);

/* Functions included from Food Database Tab */
void create_food_database_page(GtkWidget * nb);
void fill_query_results(void);

/*Functions included from Recipe Tab */
void create_recipe_page(GtkWidget * nb);

/* Functions included from File handling */
int default_db_file_path(void);
int get_location_file(void);
int move_db_file(char * dst, char *src);

/* Functions included from Preferences Tab */
void create_preferences_page(GtkWidget *nb);

/* Functions included from Food Journal Tab */
void	create_food_journal(GtkWidget * nb);

#endif
