#include <gtk/gtk.h>
static void activate (GtkApplication *app, gpointer user_data)
{
	  GtkWidget *window;
	  GtkWidget *utilities_category_button;
	  GtkWidget *games_category_button;
	  GtkWidget *box;

	  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	  
	  //prepare the window
	  window = gtk_application_window_new (app);
	  gtk_window_set_title (GTK_WINDOW (window), "Window");
	  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);

	  //set buttons
	  utilities_category_button = gtk_button_new_with_label ("Utilities");
	  games_category_button = gtk_button_new_with_label("Games");

	  //pack them into a box
	  gtk_box_pack_start(GTK_BOX(box),utilities_category_button, 1, 0, 0);
	  gtk_box_pack_start(GTK_BOX(box),games_category_button, 1, 0, 0);
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
