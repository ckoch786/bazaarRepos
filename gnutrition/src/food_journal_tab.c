/***************************************************************************
 *            food_journal_tab.c
 *
 *  Sat March  12 18:53:19 2011
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

#include <gtk/gtk.h>
#include <glib.h>
 
#include "callbacks.h"
#include "sqlite3_api.h"
#include "user_interface.h"

void	create_food_journal(GtkWidget * nb)
{

  GtkWidget *vbox = gtk_vbox_new(FALSE, 0);	
	GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
	GtkWidget *calendar = gtk_calendar_new();
  GtkWidget * tab_label = gtk_label_new(_("Food Journal"));
 
	//gtk_box_pack_start (GTK_BOX (vbox), calendar, FALSE, FALSE, 0);

  //GtkListStore *store = gtk_list_store_new(1, G_TYPE_BOXED);
  GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
  GtkTreeIter iter;
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, "Hello", -1);
	//gtk_list_store_set(store, &iter, 0, calendar, -1);
  GtkWidget *combo_box = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));

	GtkCellRenderer *cell = gtk_cell_renderer_text_new();
	//GtkCellRenderer *cell = gtk_cell_renderer_text_set_fixed_height_from_font();
	//GtkCellRenderer

	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), cell, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), cell, "text", 0, NULL);
	
	gtk_box_pack_start (GTK_BOX (hbox), combo_box, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  gtk_notebook_append_page(GTK_NOTEBOOK(nb), vbox, tab_label);

	g_object_unref(G_OBJECT(store));
}

