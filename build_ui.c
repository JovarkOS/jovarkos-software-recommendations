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
		// switches between key and value mode while erasin the buffer
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

char *build_ui_from_category(Category category)
{
	char* xml_ui_definition;
	xml_ui_definition = malloc(sizeof(char) * 10000);
	xml_ui_definition[0] = '\0';
	//this isn't spacing properly and I have no idea why.
	sprintf(xml_ui_definition, "\
<interface>\n\
 <object class=\"GtkBox\" id=\"main\">\n\
  <property name=\"orientation\">vertical</property>\n\
  <property name=\"visible\">True</property>\n\
  <property name=\"spacing\">200</property>\n\
  <child>\n");
	
	for(int i=0; i < category.software_count; i++)
	{
		char temp_buffer[1000];
		sprintf(temp_buffer, "\
   <object class=\"GtkBox\" id=\"%s\">\n\
    <property name=\"orientation\">vertical</property>\n\
    <child>\n\
     <object class=\"GtkTextBuffer\" id=\"title-%i\">\n\
      <property name=\"text\"> hello world</property>\n\
      </object>\n\
       <object class=\"GtkTextView\">\n\
        <property name=\"visible\">True</property>\n\
        <property name=\"buffer\">title-%i</property>\n\
       </object>\n\
        <object class=\"GtkButton\" id=\"%s-button\">\n\
        <property  name=\"label\">install %s</property>\n\
        <property name=\"visible\">True</property>\n\
       </object>\n\
     </child>\n\
    </object>\n\
   <packing>\n\
    <property name=\"expand\">False</property>\n\
    <property name=\"fill\">True</property>\n\
    <property name=\"position\">0</property>\n\
   </packing>", category.software_list[i].id, i, i,category.software_list[i].id,  category.software_list[i].name);
		strcat(xml_ui_definition, temp_buffer);	
	
	}
	strcat(xml_ui_definition, "\
  </child>\n\
 </object>\n\
</interface>");
	printf("xml ui def:\n%s\n", xml_ui_definition);
	return xml_ui_definition;
}
