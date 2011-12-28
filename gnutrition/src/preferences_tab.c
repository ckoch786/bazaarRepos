/***************************************************************************
 *            preferences_tab.c
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

#include "user_interface.h"
#include "callbacks.h"

/* This entry contains the file path besides browse button in preferences tab. */
GtkWidget *text_entry;

/* This function is made merely to avoid sharing preferences file path text entry. */
void update_file_path_pref(void)
{
  gtk_entry_set_text(GTK_ENTRY(text_entry), db_file_path);
}

void create_preferences_page(GtkWidget *nb)
{ 
  GtkWidget *tab_label = gtk_label_new(_("Preferences"));
  GtkWidget *button = gtk_button_new_with_label (_("Browse"));
  GtkWidget *day_label = gtk_label_new(_("Week starts on: "));

  /* When the button receives the "clicked" signal, it will call the
   * function (browse_proc) passing argument.
   */
  g_signal_connect (button, "clicked",
		      G_CALLBACK (browse_proc), NULL);
	
  GtkWidget *button_box = gtk_vbutton_box_new ();
	gtk_container_add(GTK_CONTAINER(button_box), button);

  GtkWidget *frame_vert = gtk_frame_new (_("Location of food database"));
	GtkWidget *vbox = gtk_vbox_new(FALSE, FALSE);
	GtkWidget *hbox = gtk_hbox_new(FALSE, FALSE);

	//gtk_container_add(GTK_CONTAINER(hbox), button_box);
	gtk_box_pack_start(GTK_BOX(hbox), button_box, FALSE, FALSE, 0);

	/* Create a text entry and show the current path to db. This needs to be updated
	 * when ever path to db file is modified.
	 */
	text_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(text_entry), db_file_path);

	gtk_container_add(GTK_CONTAINER(hbox), text_entry);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 30);
  //gtk_container_add(GTK_CONTAINER(vbox), hbox);
	
	GtkWidget *day_sel_box = gtk_hbox_new(FALSE, FALSE);
	gtk_box_pack_start(GTK_BOX(day_sel_box), day_label, FALSE, FALSE, 0);

  GtkWidget *day_combo_box = gtk_combo_box_new_text();
	gtk_combo_box_set_title(GTK_COMBO_BOX(day_combo_box), "Select day");
	
  int i=0;

	gtk_combo_box_insert_text (GTK_COMBO_BOX(day_combo_box), i, "Saturday");
	gtk_combo_box_insert_text (GTK_COMBO_BOX(day_combo_box), i, "Friday");
	gtk_combo_box_insert_text (GTK_COMBO_BOX(day_combo_box), i, "Thursday");
	gtk_combo_box_insert_text (GTK_COMBO_BOX(day_combo_box), i, "Wednesday");
	gtk_combo_box_insert_text (GTK_COMBO_BOX(day_combo_box), i, "Tuesday");
	gtk_combo_box_insert_text (GTK_COMBO_BOX(day_combo_box), i, "Monday");
	gtk_combo_box_insert_text (GTK_COMBO_BOX(day_combo_box), i, "Sunday");

	gtk_combo_box_set_active(GTK_COMBO_BOX(day_combo_box), 0);

	gtk_box_pack_start(GTK_BOX(day_sel_box), day_combo_box, FALSE, FALSE, 0);
  //gtk_container_add(GTK_CONTAINER(day_sel_box), day_combo_box);
	
	gtk_box_pack_start(GTK_BOX(vbox), day_sel_box, FALSE, FALSE, 30);
  //gtk_container_add(GTK_CONTAINER(vbox), day_sel_box);

	gtk_container_add(GTK_CONTAINER(frame_vert), vbox);
	
  gtk_notebook_append_page(GTK_NOTEBOOK(nb), frame_vert, tab_label);
}
