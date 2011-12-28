/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * callbacks.c
 * Copyright (C) 2011 Free Software Foundation, Inc.
 * 
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "user_interface.h"
#include "sqlite3_api.h"
#include "callbacks.h"

GtkWidget *new_amount;
GtkWidget *field_sel_cmbo_box;
GtkTextBuffer *buffer;

void destroy (GtkWidget *widget, gpointer data)
{
  gtk_main_quit ();
}

void fill_text_entry_with_query_results(void)
{
  GtkTextIter iter;

  /* No need to clear the text buffer since initializing iterator from zero offset
   * already clears it.
   */	

  /* Iterator is used to traverse line by line. */
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);

  /* Values and row_count are two shared structures included from sqlite
   * which are filled upon execution of a database query
   */
  int i;
  for(i=0; i<col_count; i++) {

    gtk_text_buffer_insert(buffer, &iter, ColNames[i], -1);
    gtk_text_buffer_insert(buffer, &iter, " = ", -1);
    gtk_text_buffer_insert(buffer, &iter, values[i], -1);
    gtk_text_buffer_insert(buffer, &iter, "\n", -1);
  }
}

void create_cmbo_box_with_names(void)
{
  /*--Create a new combo box widget --*/
  field_sel_cmbo_box = gtk_combo_box_new_text();

	gtk_combo_box_set_title(GTK_COMBO_BOX(field_sel_cmbo_box), "Select name");
	
  int i;
  for(i=0; i<col_count; i++) {
    gtk_combo_box_insert_text (GTK_COMBO_BOX(field_sel_cmbo_box), i, ColNames[i]);
  }

  //gtk_combo_box_set_add_tearoffs(GTK_COMBO_BOX(field_sel_cmbo_box), TRUE);	
}

void update_field_proc(GtkWidget * button, gpointer data)
{
  gint col_num = gtk_combo_box_get_active(GTK_COMBO_BOX(field_sel_cmbo_box));
  gchar *value = (gchar *)gtk_entry_get_text (GTK_ENTRY(new_amount) );
	
  g_print("field num selected is %d \n", col_num);
  g_print("And new value is %s \n", value);

  gchar * fld_name = gtk_combo_box_get_active_text(GTK_COMBO_BOX(field_sel_cmbo_box));
  //g_print("BUTTON PRESSED and field selected string is %s \n", fld_name);

  strcpy(values[col_num], value);

  fill_text_entry_with_query_results();	

	g_print("Food DES %s\n", values[1]);

	/* Update the entry in db. Last param is Food_Des i.e. name of food item
	 * which identifies a row.
	 */
    run_update_query(fld_name, value, values[1]);
}

void open_food_item(char * item_name)
{
  GtkWidget * food_window, * vbox;
	
  /* Create and set window. */
  food_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_position(GTK_WINDOW(food_window), GTK_WIN_POS_CENTER);
  gtk_window_set_title (GTK_WINDOW (food_window), "Food Item Detail");
  gtk_window_set_default_size(GTK_WINDOW(food_window), 600, 400);
  gtk_window_set_modal(GTK_WINDOW(food_window), TRUE);
  gtk_window_set_transient_for(GTK_WINDOW(food_window), NULL);	
	
  /* Create box to contain widgets. */
  vbox = gtk_vbox_new (FALSE, 0);

  GtkWidget *name = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(name), item_name);
  gtk_entry_set_editable(GTK_ENTRY(name), FALSE);	
  gtk_box_pack_start(GTK_BOX(vbox), name, FALSE, FALSE, 10);
  //gtk_container_add(GTK_CONTAINER(vbox), label);

  run_scalar_query(item_name);

  /* Create a multiline text widget. */
  GtkWidget *text = gtk_text_view_new ();

  gtk_text_view_set_editable(GTK_TEXT_VIEW(text), FALSE);

  /* Obtaining the buffer associated with the widget. */
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text));

  GtkWidget * scrld_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrld_window),
    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_scrolled_window_add_with_viewport (
    GTK_SCROLLED_WINDOW (scrld_window), text);

  // Used Container add so that vbox occupies the whole tab rather than a small window.
  gtk_box_pack_start (GTK_BOX (vbox), scrld_window, TRUE, TRUE, 30);
  //gtk_container_add(GTK_CONTAINER(vbox), scrld_window);

  fill_text_entry_with_query_results();	
	
  GtkWidget *editbox = gtk_hbox_new(FALSE, 0);

  create_cmbo_box_with_names();
	
  new_amount = gtk_entry_new();
  GtkWidget *update_button = gtk_button_new_with_mnemonic("Update");

  gtk_box_pack_start(GTK_BOX(editbox), field_sel_cmbo_box, FALSE, FALSE, 0);	
  gtk_box_pack_start(GTK_BOX(editbox), new_amount, FALSE, FALSE, 10);	
  gtk_box_pack_start(GTK_BOX(editbox), update_button, FALSE, FALSE, 10);	

  /* When the button receives the "clicked" signal, it will call the
   * function (browse_proc) passing argument.
   */
  g_signal_connect (update_button, "clicked",
		      G_CALLBACK (update_field_proc), NULL);
              //(gint *)gtk_combo_box_get_active(GTK_COMBO_BOX (field_sel_cmbo_box)));
	
  gtk_box_pack_start(GTK_BOX(vbox), editbox, FALSE, FALSE, 10);	
	
  gtk_container_add (GTK_CONTAINER(food_window), vbox);
  gtk_widget_show_all(food_window);
}

gboolean mousevent (GtkWidget *widget, GdkEventButton *MoBt)
{   
  GtkTextIter there, tend;
  gchar *word;
   
  if (MoBt->button != 1) 
    return FALSE;
	
  if (MoBt->type != GDK_2BUTTON_PRESS)
    return FALSE;

  gtk_text_buffer_get_iter_at_mark (food_db_buffer, &there,
    gtk_text_buffer_get_insert (food_db_buffer));
	
  if ((gtk_text_iter_has_tag(&there,hlink)) != TRUE)
    return FALSE;
	
  if ((gtk_text_iter_begins_tag(&there,hlink)) != TRUE)
    gtk_text_iter_backward_to_tag_toggle(&there,hlink);

  tend=there;
  gtk_text_iter_forward_to_tag_toggle(&tend,hlink);
  word=gtk_text_buffer_get_text(food_db_buffer, &there,&tend,FALSE);

  //g_print("Event captured\n");
  //g_print("%s\n", word);

  open_food_item(word);
	
  return TRUE;
} 

void about_gnutrition_proc (void)
{
  GtkWidget *abt_window, *label_main, *copy_right, *text, *vbox;
  GtkWidget *frame;

  /* Create and set window. */
  abt_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_window_set_position(GTK_WINDOW(abt_window), GTK_WIN_POS_CENTER);
  gtk_window_set_title (GTK_WINDOW (abt_window), "About GNUtrition");
  gtk_window_set_default_size(GTK_WINDOW(abt_window), 450, 250);
  gtk_window_set_modal(GTK_WINDOW(abt_window), TRUE);
  gtk_window_set_transient_for(GTK_WINDOW(abt_window), NULL);	
	
  /* Create box to contain widgets. */
  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER(abt_window), vbox);

  /* Create main label. */
  char version[40];

  sprintf(version, "GNUtrition %s\nDatabase version %s",gnutrition_version,
    database_version);
	
  label_main = gtk_label_new(version);
  //gtk_label_set_text(,"GNUtrition %s", gnutrition_version);
	
  gtk_box_pack_start (GTK_BOX (vbox), label_main, FALSE, FALSE, 15);

  /* Create copy right label inside a frame to show under line. */
  frame      = gtk_frame_new("");
  copy_right = gtk_label_new(_("Copyright 2010, 2011 Free Software Foundation, Inc."));
  //gtk_label_set_pattern (GTK_LABEL (copy_right),
  //  "_____________________________________________________");
  gtk_container_add(GTK_CONTAINER(frame), copy_right);
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 10);


  /* Create the GtkText label */
  text = gtk_label_new(
    _("This program is free software: you can redistribute it and/or \n"
	  "modify it under the terms of GNU General Public License as \n"
      "published by the Free Software Foundation, either version 3 of \n"
	  "the License, or (at your option) any later version. \n"
      "This program is distributed in the hope that it will be useful \n"
      "but WITHOUT ANY WARRANTY; without even the implied \n"
	  "warranty MERCHANTABILITY or FITNESS FOR A PARTICULAR \n"
	  "PURPOSE. See the GNU General Public License for more details.\n" 
      "You should have received a copy of the GNU General Public \n"
	  "License along with this program. If not, see \n"
	  "<http://www.gnu.org/licenses/>"));
	
  gtk_label_set_justify (GTK_LABEL (text), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap_mode(GTK_LABEL (text), TRUE);
 
  GtkWidget * scrld_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrld_window),
    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_scrolled_window_add_with_viewport (
    GTK_SCROLLED_WINDOW (scrld_window), text);

  // Use container add so that help text scroll window occupies the entire box
  gtk_container_add(GTK_CONTAINER(vbox), scrld_window);
  //gtk_box_pack_start (GTK_BOX (vbox), scrld_window, FALSE, FALSE, 0);

  gtk_widget_show_all(abt_window);  
}

/* This function puts up a confirmation dialog after choosing a new folder for
 * database file, whether we like to move the database file too or not.
 */
int confirm_move_file_dialog(char *new_path)
{
  GtkWidget *dialog, *label, *content_area;
  char old_path[DB_FILE_PATH_LENGTH];
  int status = FALSE;
	
  /* Back up the old path for moving the actual file and update new path. */	
  strcpy(old_path,db_file_path);
  strcpy(db_file_path, new_path);
	
  // Prompt whether to move db  or not.
  /* Adding dialogue to ask whether to move the database file or not. */
  dialog = gtk_dialog_new_with_buttons(_("Move Database"), NULL, 
	    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_STOCK_YES,
                                            GTK_RESPONSE_YES,
                                            GTK_STOCK_NO,
                                            GTK_RESPONSE_NO,
                                            NULL); 

  content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  label = gtk_label_new (_("Do you wish to move/create the database file to new location?\n"));
    //"Selecting No means you will do it manually and restart the program\n"));

  /* Add the label, and show everything we've added to the dialog. */
  gtk_container_add (GTK_CONTAINER (content_area), label);
  gtk_widget_show_all(dialog);

  /* Run the dialog in a loop until response received. */
  gint result = gtk_dialog_run (GTK_DIALOG (dialog));
	
  switch (result)
  {
    case GTK_RESPONSE_YES:
      //g_print("YES\n");
	  // Move the database file
      move_db_file(db_file_path, old_path);
	  status = TRUE;
      break;
    default:
      //g_print("NO\n");// Distroy dialog since it was cancelled.
      break;
  }

  gtk_widget_destroy (dialog);	
  return status;
}

/* This function creates the Folder chooser dialog when moving database file to
 * another location.
 */
void move_db_dialog(void)
{
  GtkWidget *filedialog;
  char *folder_chosen;
  char curr_path[DB_FILE_PATH_LENGTH];
  int status = -1;

  filedialog = gtk_file_chooser_dialog_new ("Move Database File", NULL,
     				      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
     				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
     				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
     				      NULL);

  gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(filedialog), TRUE);

  if(gtk_dialog_run (GTK_DIALOG (filedialog)) == GTK_RESPONSE_ACCEPT) {

    folder_chosen = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (filedialog));
    //g_print("Current Chosen Folder %s\n", folder_chosen);

    /* Copy folder location and append the file name to the end. */
    sprintf(curr_path, "%s/%s", folder_chosen, DB_FILE_NAME);

	if(confirm_move_file_dialog(curr_path) == TRUE) {
      /* Update the Preferences text entry with new path. */  
      update_file_path_pref();

      /* Create file containing path to db file. It will be used when application
       * is ran again.
	   */
      create_location_file();
      status = run_basic_query(); 
      fill_query_results();

      /* Run query on version table too. */
      status = create_exec_version_table_query();
	}
  } // end if ok / open clicked

  // nothing to do if cancelled	except destroying the dialog
  gtk_widget_destroy (filedialog);	
}


void locate_db_dialog(void)
{
  GtkWidget *filedialog;
  char *folder_chosen;
  char curr_path[DB_FILE_PATH_LENGTH];
  int status = -1;
	
  filedialog = gtk_file_chooser_dialog_new ("Locate Database Folder", NULL,
     				      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
     				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
     				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
     				      NULL);

  gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(filedialog), FALSE);

  while(status < 0) {

    if(gtk_dialog_run (GTK_DIALOG (filedialog)) == GTK_RESPONSE_ACCEPT) {
      folder_chosen = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (filedialog));
      //g_print("Current Chosen Folder %s\n", folder_chosen);

      /* Copy folder location and append the file name to the end. */
      sprintf(curr_path, "%s/%s", folder_chosen, DB_FILE_NAME);

      FILE *test_ptr = fopen(curr_path, "r");

      if(test_ptr != NULL) { // file exists
	    fclose(test_ptr);

		//g_print("file found and closed\n");
   	    strcpy(db_file_path, curr_path);
		  
        /* Create file containing path to db file. It will be used when application
		 * is ran again.
		 */
        create_location_file();
        status = run_basic_query(); 
        fill_query_results();

        /* Run query on version table too. */
        status = create_exec_version_table_query();
	  
	  } // end if db file found
		
      else
        g_print("File not found - please choose a folder containing file named \'%s\'\n", DB_FILE_NAME);
		
	} // end if dialog ok / open button pressed

	else {
      g_print("Gnutrition cannot continue unless a valid database file is found\n");
	  exit(1);
	}  
  } // end while

  gtk_widget_destroy (filedialog);

} // end locate db file function

	
void browse_proc (GtkWidget * button)
{
  //g_print("BUTTON PRESSED \n");
  move_db_dialog();
}

/* This call back is called when ever a key is pressed in search text entry box. */
gboolean search_contains_proc (GtkWidget *widget, GdkEvent *event, gpointer   data)
{
  //g_print("key pressed\n");
  //g_print ("%s pressed, text entry contains %s\n", (char *)data, search_string);

  const gchar* search_string;
  char query[MAX_QUERY_STRING_LEN];

  strcpy(query, "select Food_Des from food");

  /* If search box raised this event and is non empty, include it's text in
   * query
   */
  if(strcmp((char *)data, "findtext1") == 0) { 

	/* Get the search string */
    search_string = get_search_text_for_query("findtext1");

    if(strcmp (search_string,"") != 0)
	  sprintf(query, "%s where Food_Des like \"%s%s%s\"", query, "%", search_string, "%");
  }

  //g_print(query);
  char prm[2][MAX_QUERY_STRING_LEN];

  strcpy(prm[0], db_file_path);
  //g_print("prm[0] %s\n", prm[0]);
  strcpy(prm[1],query);
  //g_print("prm[1] %s\n", prm[1]);

  int num_prm = 2;
  sqlite3_run_query(num_prm, prm);

  fill_query_results();

  /* FALSE continues the event being propagated. This means that after calling this function,
   * the key pressed should also be displayed normally inside the entry.
   * If TRUE is returned then event is not further propagated.
   */	
  return FALSE;
}
