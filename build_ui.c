#include "build_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
void append_char(char str[], char c)
{
	char arr[2] = {c, '\0'};
	strncat(str, arr, 2);
}

Category read_category_from_file(const char *file_name)
{
	// this code is a mess, maybe somebody with time and more c knowledge than I do can refactor it somewhat.
	// idk if that's possible tho.
	// please change variable names. if you can reduce loc or make it less complex do that as well
	Software *software_list;
	Category to_return = {software_list, "template"};
	to_return.software_list = malloc(sizeof(Software) * 100); // Idk why I'm mallocing here. call it future proofing.
	FILE *file;
	char current_string[100000];
	char key_string[500];
	int current_character;
	int software_count = 0;
	Mode parse_mode = NONE;

	file = fopen(file_name, "r");
	if (file == NULL)
	{
		printf("file is null\n");
		return to_return;
	}
	do
	{
		// essentially right here we grab a character from the file, and either write it to a buffer or take it as a control char.
		current_character = fgetc(file);
		switch (current_character)
		{ // this defined a header/id section
		case '[':
			parse_mode = ID;
			software_count++;
			break;
		case ']':
		{
			// this gets the id of the element or smth, idk, I'm sleep deprived
			if (parse_mode == ID)
			{

				strncpy(to_return.software_list[software_count - 1].id, current_string, sizeof(to_return.software_list[0].id));
				parse_mode = KEY;
				current_string[0] = '\0';
			}
		}
		break;
		// switches between key and value mode while erasing the buffer
		case '=':
		{
			if (parse_mode == KEY)
			{
				strncpy(key_string, current_string, sizeof(key_string));
				parse_mode = VALUE;
				current_string[0] = '\0'; // yes this is how you 'erase' a string in c
			}
		}
		break;

		case '\n':
		{	// extracts the value for each key and puts it in its appropriate slot in a Software struct
			// long if else chain because c doesn't support char* in switch statements, and hash mapping is too hard
			if (strcmp(key_string, "name") == 0 && parse_mode == VALUE)
			{
				strncpy(to_return.software_list[software_count - 1].name, current_string, sizeof(to_return.software_list[0].name));
			}

			else if (strcmp(key_string, "image_source") == 0 && parse_mode == VALUE)
			{
				strncpy(to_return.software_list[software_count - 1].image, current_string, sizeof(to_return.software_list[0].image));
			}
			else if (strcmp(key_string, "description") == 0 && parse_mode == VALUE)
			{
				strncpy(to_return.software_list[software_count - 1].description, current_string, sizeof(to_return.software_list[0].description));
			}
			else if (strcmp(key_string, "package") == 0 && parse_mode == VALUE)
			{
				strncpy(to_return.software_list[software_count - 1].package, current_string, sizeof(to_return.software_list[0].package));
			}

			current_string[0] = '\0';
			parse_mode = KEY;
		}
		break;
		default:
		{
			if (parse_mode != NONE)
				append_char(current_string, (char)current_character); // feeds chars to the buffer
		}
		break;
		}
	} while (current_character != EOF);
	to_return.software_count = software_count;
	fclose(file); 
	return to_return;
}

void callback(GtkWidget *widget, gpointer data)
{	
	char install_command[500];
	sprintf(install_command, "yes | pkexec pacman -S  %s\n", ((Software *) data)->package);
	int status = system(install_command);
}

GtkWidget* build_ui_from_category(Category category)
{
	GtkWidget* category_main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	
	for(int i = 0; i < category.software_count; i++)
	{	
		
		GtkWidget* main_software_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
		
		//setup install button
		char button_title[600];
		sprintf(button_title, "install %s", category.software_list[i].name);
		GtkWidget* install_button = gtk_button_new_with_label(button_title);
		gtk_widget_set_size_request(GTK_WIDGET(install_button), 400, 30);
		g_signal_connect(install_button, "clicked", G_CALLBACK(callback), (gpointer) &category.software_list[i]);
		

		//setup title
		PangoFontDescription* title_font_desc = pango_font_description_from_string ("Helvetica 15");
		GtkWidget* title =  gtk_text_view_new();
		GtkTextBuffer* title_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (title));
		gtk_text_buffer_set_text(title_buffer, category.software_list[i].name, -1);
		gtk_widget_override_font (title, title_font_desc);
		pango_font_description_free (title_font_desc);
		gtk_text_view_set_editable(GTK_TEXT_VIEW (title), FALSE);
		gtk_text_view_set_justification(GTK_TEXT_VIEW(title), GTK_JUSTIFY_CENTER);
		gtk_text_view_set_left_margin(GTK_TEXT_VIEW(title), 20);
		gtk_text_view_set_right_margin(GTK_TEXT_VIEW(title), 20);
		gtk_text_view_set_top_margin(GTK_TEXT_VIEW(title), 4);
		gtk_widget_set_size_request(GTK_WIDGET(title), 400, 20);

		//setup description
		PangoFontDescription* description_font_desc = pango_font_description_from_string ("Helvetica 10");
		GtkWidget* description = gtk_text_view_new();
		GtkTextBuffer* description_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (description));
		gtk_text_buffer_set_text(description_buffer, category.software_list[i].description, -1);
		gtk_widget_override_font (description, description_font_desc);
		pango_font_description_free (description_font_desc);
		gtk_text_view_set_editable(GTK_TEXT_VIEW (description), FALSE);
		gtk_text_view_set_justification(GTK_TEXT_VIEW(description), GTK_JUSTIFY_CENTER);
		gtk_text_view_set_left_margin(GTK_TEXT_VIEW(description), 20);
		gtk_text_view_set_right_margin(GTK_TEXT_VIEW(description), 20);
		gtk_text_view_set_top_margin(GTK_TEXT_VIEW(description), 8 );
		gtk_widget_set_size_request(GTK_WIDGET(description), 400, 90);

		//setup image
		GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(category.software_list[i].image, NULL);
		pixbuf = gdk_pixbuf_scale_simple(pixbuf, 560, 315, GDK_INTERP_BILINEAR);
		GtkWidget* image = gtk_image_new_from_pixbuf(pixbuf);
		
		//pac stuff into a box
		gtk_box_pack_start(GTK_BOX(main_software_box), GTK_WIDGET(title), 0, 0, 1);
		gtk_box_pack_start(GTK_BOX(main_software_box), GTK_WIDGET(image), 0, 0, 1);
		gtk_box_pack_start(GTK_BOX(main_software_box), GTK_WIDGET(description), 0, 0, 1);
		gtk_box_pack_start(GTK_BOX(main_software_box), GTK_WIDGET(install_button), 0, 0, 1);
		gtk_box_pack_start(GTK_BOX(category_main_box), GTK_WIDGET(main_software_box), 0, 0, 1);
	}
	return category_main_box;
}
