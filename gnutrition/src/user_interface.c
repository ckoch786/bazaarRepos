/***************************************************************************
 *            user_interface.c
 *
 *  Sun Jan  2 21:38:19 2011
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <config.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h> // includes GDK_q
#include <glib.h>

#include "callbacks.h"
#include "sqlite3_api.h"
#include "user_interface.h"

GtkWidget* create_window (void)
{
	GtkWidget *window;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "GNUtrition 0.4");
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect (GTK_OBJECT (window), "destroy",
      (GtkSignalFunc)gtk_main_quit, NULL);

	return window;
}

void create_menus(GtkWidget *main_vbox, GtkWidget *window)
{
  GtkWidget *vbox, *file_root, *help_root, *menu_bar;
  GtkWidget *file_menu, *quit_item, *help_menu, *about_gnutrition;

  // Create an accelerator group.
  GtkAccelGroup *accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group (GTK_WINDOW(window), accel_group);

  /* Create new menu, menu item and append the item to it. */
  file_menu = gtk_menu_new();
  quit_item = gtk_menu_item_new_with_label("Quit");
  gtk_menu_append(GTK_MENU(file_menu), quit_item);

  /* We can attach the Quit menu item to our exit function */
  g_signal_connect (GTK_OBJECT (quit_item), "activate",
   GTK_SIGNAL_FUNC (destroy), (gpointer) "file.quit");
  gtk_widget_show(quit_item);	  

  gtk_widget_add_accelerator (quit_item, // widget
                              "activate", // signal to emit
                              accel_group, // accelerator group
                              GDK_q, // key
                              GDK_CONTROL_MASK, // modifier keys
                              GTK_ACCEL_VISIBLE); // flags

  help_menu = gtk_menu_new();
  about_gnutrition = gtk_menu_item_new_with_label("About GNUtrition");
  gtk_menu_append(GTK_MENU(help_menu), about_gnutrition);

  g_signal_connect (GTK_OBJECT (about_gnutrition), "activate",
  G_CALLBACK (about_gnutrition_proc), NULL);
  gtk_widget_show(about_gnutrition);

  /* This is the root menu and will be the label displayed
   * on the menu bar. It will carry the whole menu.
   */
  file_root = gtk_menu_item_new_with_label("File");
  gtk_widget_show(file_root);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_root), file_menu);

  help_root = gtk_menu_item_new_with_label("Help");
  gtk_widget_show(help_root);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_root), help_menu);

  // Inserting menu's vbox into the window's main vbox.	
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(main_vbox), vbox, FALSE, FALSE, 0);
  //gtk_container_add(GTK_CONTAINER(main_vbox), vbox);
  gtk_widget_show(vbox);
	  
  menu_bar = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);
	
  gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), file_root);	
  gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), help_root);	
  gtk_widget_show(menu_bar);
}

void create_notebook(GtkWidget * main_vbox)
{
  // This box keeps note book widget occupy area of it's own size and not the
	// entire page.
  //GtkWidget * h_box = gtk_hbox_new(FALSE, 0);
  GtkWidget * nb = gtk_notebook_new();
  GtkWidget * label, *tab_label;

  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(nb), GTK_POS_LEFT);
	create_food_journal(nb);
	
/*  label = gtk_label_new(_("recipes"));
  tab_label = gtk_label_new(_("Recipes"));
  gtk_notebook_append_page(GTK_NOTEBOOK(nb), label, tab_label);*/
	
	create_recipe_page(nb);
	create_food_database_page(nb);
	
	label = gtk_label_new(_("Reporting"));
  tab_label = gtk_label_new(_("Reporting"));
  gtk_notebook_append_page(GTK_NOTEBOOK(nb), label, tab_label);

	create_preferences_page(nb);

	gtk_container_add(GTK_CONTAINER(main_vbox), nb);
	//gtk_container_add(GTK_CONTAINER(h_box),nb);
  //gtk_box_pack_start(GTK_BOX(main_vbox), h_box, FALSE, FALSE, 0);
}

void	UI_initialize(int * argc, char *argv[])
{
  GtkWidget * window, * main_vbox;
  gtk_set_locale ();
  gtk_init (argc, &argv);
 
  window = create_window ();
  main_vbox = gtk_vbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(window), main_vbox);
    
  create_menus (main_vbox, window);
  create_notebook(main_vbox);

	gtk_widget_show_all(window); 
  gtk_main ();
}
