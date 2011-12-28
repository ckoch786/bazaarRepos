
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

#include "callbacks.h"
#include "user_interface.h"
#include "sqlite3_api.h"


void create_recipe_page( GtkWidget * nb )
{
   GtkWidget *tab_label;
   GtkWidget *recipe_name_label;/*RN*/
   GtkWidget *RN_entry;
   GtkWidget *category_label;
   GtkWidget *category_combo; 
   GList     *glist;/*List for the combo*/
   GtkWidget *num_servings_label;/*NS*/
   GtkWiget  *NS_entry;
   GtkWidget *ingredients_label;
   /*Amount |measure |Food*/
   GtkWidget *ingredients_tabular_scroll_pane; /*TODO ???*/
   GtkWidget *recipe_instructions_label;
   GtkWidget *RI_scroll_pane;/*TODO ???*/
   GtkWidget *hbox;/* container for recipe menu*/
   GtkWidget *RN_hsearchbox, category_NS_searchbox;
   GtkWidget *vbox;/* container for entering recipes */
   

   /*TODO add code for horizontal recipe menu*/
   
   /* Create a box to contain recipes */
   vbox = gtk_vbox_new ( FALSE, 0 );

/**************************Recipe Name***********************************/   
   
   /* Create a box to contain the Recipe name */ 
   RN_hsearchbox = gtk_hbox_new (FALSE, 0 );
   
   /* Initialize label and entry search box  */
   recipe_name_label = gtk_label_new_with_mnemonic ("Recipe Name");
   RN_entry = gtk_entry_new();
   /* TODO set signals */ 

   /*pack box*/
   gtk_box_pack_start (GTK_BOX (RN_hbox), 
                       recipe_name_label,
		       RN_entry, 
		       TRUE, TRUE, 0 );

/**********************Category and Num. Servings****************************/

   /*Create a box to contain the Category and Num servings widgets*/
   NS_hsearchbox = gtk_hbox_new (FALSE, 0);

   /*Initialize label and combo box*/
   category_label = gtk_label_new_with_mnemonic ("Category");
   
   category_combo = gtk_combo_new();

   glist = g_list_append (glist, "All");
   glist = g_list_append (glist, "Breakfast");
   glist = g_list_append (glist, "Lunch");
   glist = g_list_append (glist, "Dinner");
   glist = g_list_append (glist, "Snack");
   glist = g_list_append (glist, "Dessert");
   glist = g_list_append (glist, "Appetizer");
   glist = g_list_append (glist, "Bread");
   glist = g_list_append (glist, "Salad");
   glist = g_list_append (glist, "Soup");
   glist = g_list_append (glist, "Drink");
   glist = g_list_append (glist, "Alcohol");

   gtk_combo_set_popdown_strings (GTK_COMBO (category_combo), 
                                             glist);
   /* TODO can free glist now, combo takes a copy */

   /* Initialize Num Servings label and entry box */
   num_servings_label = gtk_label_new_with_mnemonic ("Num. Servings");
   NS_entry = gtk_entry_new();

   /*pack box*/
   gtk_box_pack_start (GTK_BOX(NS_hsearchbox),
                       category_label,
		       category_combo,
		       num_servings_label,
		       NS_entry,
		       TRUE, TRUE, 0);

/********************************Ingredients***********************************************/
}/* end create_recipe_page */



