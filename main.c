#include <gtk/gtk.h>
#include "build_ui.h"
#include "main.h"

static void hide_buttons()
{
	gtk_widget_hide(category_container);
}

static void on_button_click(GtkWidget *widget, gpointer data)
{
	// Hide the buttons from the view 
	hide_buttons();
	Category category = read_category_from_file("views/template.category");
	char* xml = build_ui_from_category(category); 
	GtkBuilder* builder = gtk_builder_new_from_string(xml, -1);
	GObject* main_box = gtk_builder_get_object(builder, "box");
	gtk_widget_show(GTK_WIDGET(main_box));
}

static void activate (GtkApplication *app, gpointer user_data)
{
	  category_container  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	  // prepare the window
	  window = gtk_application_window_new (app);
	  gtk_window_set_title (GTK_WINDOW (window), "JovarkOS Software Recommendations");
	  gtk_window_set_icon_from_file(GTK_WINDOW(window), "assets/window-icon.png", NULL);
	  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);

	  // make a new button for each category and add each to the box
	  for(int i = 0; i < CATEGORY_NUMBER; i++)
	  {
		  	category_buttons[i] = gtk_button_new_with_label(category_names[i]);
		  	gtk_box_pack_start(GTK_BOX(category_container), category_buttons[i], 1, 1, BUTTON_PADDING);
		  	g_signal_connect(category_buttons[i], "clicked", G_CALLBACK(on_button_click), NULL);
	  }
	  
	  gtk_box_set_spacing(GTK_BOX(category_container), 2);
	  gtk_container_add (GTK_CONTAINER (window), category_container);
	  gtk_widget_show_all (GTK_WIDGET (window));
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
