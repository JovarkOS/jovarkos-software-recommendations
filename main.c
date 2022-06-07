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
	  gtk_container_add (GTK_CONTAINER (window), category_container);
	  gtk_widget_show_all (GTK_WIDGET (window));
}

static void handle_back_button_click(GtkWidget *widget, gpointer data)
{
	gtk_container_remove(GTK_CONTAINER (window), main_box);
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
	
	Category category = read_category_from_file(path);
	GtkWidget* body = build_ui_from_category(category); 
	
	gtk_box_pack_start(GTK_BOX(main_box), header, 0, 0, 10);
	gtk_box_pack_start(GTK_BOX(main_box), body, 0, 0, 10);
	
	gtk_container_remove(GTK_CONTAINER (window), category_container);
	gtk_container_add(GTK_CONTAINER (window), main_box);
	gtk_widget_show_all(GTK_WIDGET (window));
}

static void activate (GtkApplication *app, gpointer user_data)
{
	  // prepare the window
	  window = gtk_application_window_new (app);
	  gtk_window_set_title (GTK_WINDOW (window), "JovarkOS Software Recommendations");
	  gtk_window_set_icon_from_file(GTK_WINDOW(window), "assets/window-icon.png", NULL);
	  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);

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
