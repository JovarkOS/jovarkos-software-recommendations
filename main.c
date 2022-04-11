#include <gtk/gtk.h>
#include "main.h"
static void hide_buttons()
{
	for(int i=0; i< CATEGORY_NUMBER; i++)
	{
		gtk_widget_hide(category_buttons[i]);
	}
}

static void on_button_click()
{
	hide_buttons();
}

static void activate (GtkApplication *app, gpointer user_data)
{
	 

	  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	 
	  //prepare the window
	  window = gtk_application_window_new (app);
	  gtk_window_set_title (GTK_WINDOW (window), "Window");
	  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
	  for(int i = 0; i < CATEGORY_NUMBER; i++)
	  {
	  	category_buttons[i] = gtk_button_new_with_label(category_names[i]);
	  	gtk_box_pack_start(GTK_BOX(box), category_buttons[i], 1, 1, BUTTON_PADDING);
	  	g_signal_connect(category_buttons[i], "clicked", G_CALLBACK(on_button_click), NULL);
	  }
	  
	  gtk_box_set_spacing(GTK_BOX(box), 2);
	  //put the box into the window
	  gtk_container_add (GTK_CONTAINER (window), box);

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
