/***************************************************************************
 *            food_db_tab.c
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

#include <string.h>


#include "callbacks.h"
#include "user_interface.h"
#include "sqlite3_api.h"

/* First is text entry for search and second is text view for displaying results. */
GtkWidget *findtext1, *text;

int flag;

const gchar* get_search_text_for_query(char * entry_name)
{
	const gchar *text;
	
	if(strcmp(entry_name, "findtext1") == 0)
    text = gtk_entry_get_text (GTK_ENTRY (findtext1));
	
	else
		text = NULL;

	return text;
}

void create_food_database_page(GtkWidget * nb)
{
  GtkWidget *tab_label = gtk_label_new(_("Food Database"));
  GtkWidget *label_main, *vbox, *h_searchbox;

  /* Create box to contain widgets. */
  vbox = gtk_vbox_new (FALSE, 0);

	/* Add box containing label and search text entry field */
	h_searchbox = gtk_hbox_new(FALSE, 0);

  /* Create main label. */
  label_main = gtk_label_new(_("Find Items where the name contains:"));
  //gtk_box_pack_start( GTK_BOX(h_searchbox), label_main, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(h_searchbox), label_main);

	findtext1 = gtk_entry_new();

	g_signal_connect(findtext1, "key_press_event", G_CALLBACK (search_contains_proc), 
	  (gpointer) 	"findtext1"); // contains entry
	
  //gtk_box_pack_start( GTK_BOX(h_searchbox), findtext1, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(h_searchbox), findtext1);

	gtk_box_pack_start( GTK_BOX(vbox), h_searchbox, FALSE, FALSE, 5);
	
  /* Create a multiline text widget. */
  text = gtk_text_view_new ();
  //gtk_box_pack_start (GTK_BOX (vbox), text, 1, 1, 0);

	gtk_text_view_set_editable(GTK_TEXT_VIEW(text), FALSE);

  /* Register handler for double clicking an entry from mouse. */ 	
  g_signal_connect (G_OBJECT (text), "button_press_event", G_CALLBACK (mousevent), NULL); 	
	
	/* Obtaining the buffer associated with the widget. */
  food_db_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text));

  /* Try running database query on default file path, if not found look for
	 * path to file in location file in the same default.
	 */
	int status = default_db_file_path();

	if (status >= 0) { // success run query now
		status = run_basic_query(); 

	  if (status == SQL_DB_FILE_NOT_FOUND) { // if failed
      //g_print("Failed opening DB\n");

			status = get_location_file(); 
			
      if(status >= 0) {		// found file
		    status = run_basic_query(); // execute query
				
			  if(status < 0) // if failed then create dialog to locate file
		      locate_db_dialog();
			  else {
          //g_print("Successlly executed query of database provided by location file\n");

	        // Call fill query result
	        fill_query_results();
  			}
		  }
			
			else { // else if failed to get location file then create db dialog to locate file
				g_print("Failed getting path from location file\n");
	      locate_db_dialog();
			}
    } // end if failed to file db file on default path

	  else { // query executed successfully on default path db file
	    //g_print("Now call fill query\n");

	    // Call fill query result
	    fill_query_results();
	  }
	} // end if found default db path.

  /* Run query on version table too. */
  status = create_exec_version_table_query();

	if(status >= 0) { // if success
		//g_print("Version query successfully executed\n");
	}
	
  GtkWidget * scrld_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrld_window),
    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	//gtk_container_add(GTK_CONTAINER(scrld_window), text);
  gtk_scrolled_window_add_with_viewport (
    GTK_SCROLLED_WINDOW (scrld_window), text);

	// Used Container add so that vbox occupies the whole tab rather than a small window.
  gtk_box_pack_start (GTK_BOX (vbox), scrld_window, TRUE, TRUE, 30);
	//gtk_container_add(GTK_CONTAINER(vbox), scrld_window);
  gtk_notebook_append_page(GTK_NOTEBOOK(nb), vbox, tab_label);
}

void fill_query_results(void)
{
  GtkTextIter iter, start, end; 

	/* Clear the buffer first */
  if (food_db_buffer != NULL)
  {
    gtk_text_buffer_get_start_iter (food_db_buffer,&start);
    gtk_text_buffer_get_end_iter (food_db_buffer,&end);
    //gtk_text_buffer_remove_all_tags(food_db_buffer, &start, &end);
	  gtk_text_buffer_delete (food_db_buffer,&start,&end);
  } 	

	/* Iterator is used to traverse line by line. */
  gtk_text_buffer_get_iter_at_offset(food_db_buffer, &iter, 0);

	if(flag == FALSE) {
    hlink = gtk_text_buffer_create_tag (food_db_buffer, "hlink", "foreground",
	  "#0000ff", NULL);
		flag = TRUE;
	}
	
	/* Values and row_count are two shared structures included from sqlite
	 * which are filled upon execution of a database query
	 */
	int i;
	for(i=0; i<row_count; i++) {
		//g_print("%s\n", values[i]);
	  //gtk_text_buffer_insert(food_db_buffer, &iter, values[i], -1);
    gtk_text_buffer_insert_with_tags(food_db_buffer,&iter,(const gchar*)values[i],-1,hlink,NULL); 		
	  gtk_text_buffer_insert(food_db_buffer, &iter, "\n", -1);
	}
}
