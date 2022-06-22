/*
    This file is part of Jovarkos Software Recommendations.

    Jovarkos Software Recommendations is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

    Jovarkos Software Recommendations is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>. 
*/

#include "build_ui.h"
#include "main.h"

static void create_buttons()
{	  category_container  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	  // make a new button for each category and add each to the box
	  for(int i = 0; i < CATEGORY_NUMBER; i++)
	  {
		  	category_buttons[i] = gtk_button_new_with_label(category_names[i]);
		  	gtk_box_pack_start(GTK_BOX(category_container), category_buttons[i], 1, 1, BUTTON_PADDING);
		  	g_signal_connect(category_buttons[i], "clicked", G_CALLBACK(on_button_click), (gpointer) category_names[i]);
	  }
	  
	  
	  gtk_box_set_spacing(GTK_BOX(category_container), 2);
	  gtk_container_add (GTK_CONTAINER (scrolled), category_container);
	  gtk_widget_show_all (GTK_WIDGET (window));
}

static void handle_back_button_click(GtkWidget *widget, gpointer data)
{
	gtk_container_remove(GTK_CONTAINER (scrolled), main_box);
	create_buttons();

}

static void on_button_click(GtkWidget *widget, gpointer data)
{
	// Hide the buttons from the view 
	char path[100];
	char category_name[100];
	for(int i = 0; i < CATEGORY_NUMBER; i++)
	{
		if(strcmp(category_names[i],(char*) data) == 0)
		{
			strncpy(category_name, category_names[i], sizeof(category_name));
			sprintf(path, "categories/%s", category_paths[i]);
			break;
		}
	}
	//create the main box that is then pushed to the screen
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	//the header that contains stuff like the the header name and a back button
	GtkWidget* header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	//setup back button
	GtkWidget* back_button = gtk_button_new_with_label("Back");
	g_signal_connect(back_button, "clicked",  G_CALLBACK(handle_back_button_click), NULL);
	gtk_box_pack_start(GTK_BOX(header), GTK_WIDGET(back_button), 0, 0, 20);

	//setup category title
	PangoFontDescription* font_desc = pango_font_description_from_string ("Helvetica 12");
	GtkWidget* title_view =  gtk_text_view_new();
	GtkTextBuffer* title_view_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (title_view));
	gtk_text_buffer_set_text(title_view_buffer, category_name, -1);
	gtk_widget_override_font (title_view, font_desc);
	pango_font_description_free (font_desc);
	gtk_text_view_set_editable(GTK_TEXT_VIEW (title_view), FALSE);
	gtk_text_view_set_justification(GTK_TEXT_VIEW(title_view), GTK_JUSTIFY_CENTER);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(title_view), 20);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(title_view), 20);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(title_view), 8);
	gtk_widget_set_size_request(GTK_WIDGET(title_view), 400, 20);
	gtk_box_pack_start(GTK_BOX (header), GTK_WIDGET (title_view), 0, 0, 20);

	 	
	Category category = read_category_from_file(path);
	GtkWidget* body = build_ui_from_category(category); 
	
	gtk_box_pack_start(GTK_BOX(main_box), header, 0, 0, 10);
	gtk_box_pack_start(GTK_BOX(main_box), body, 0, 0, 10);

	gtk_container_remove(GTK_CONTAINER (scrolled), category_container);
	gtk_container_add(GTK_CONTAINER (scrolled), main_box);
	gtk_widget_show_all(GTK_WIDGET (window));
}

static void activate (GtkApplication *app, gpointer user_data)
{
	  // prepare the window
	  window = gtk_application_window_new( GTK_APPLICATION ( app ) );
	  gtk_window_set_title (GTK_WINDOW (window), "JovarkOS Software Recommendations");
	  gtk_window_set_icon_from_file(GTK_WINDOW(window), "assets/window-icon.png", NULL);
	  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);

	  scrolled = gtk_scrolled_window_new(NULL, NULL);
	  gtk_container_add(GTK_CONTAINER (window), scrolled);

	  //make buttons
	  create_buttons();
}

int main (int argc,char **argv)
{
	  GtkApplication *app;
	  int status;
	  app = gtk_application_new ("org.jovarkos.software", G_APPLICATION_FLAGS_NONE);
	  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	  status = g_application_run (G_APPLICATION (app), argc, argv);
	  g_object_unref (app);

	  return status;
}
