#include "build_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
void append_char(char str[] , char c){
     char arr[2] = {c , '\0'};
     strncat(str , arr, 2);
}

Category read_category_from_file(const char* file_name)
{	
	//this code is a mess, maybe somebody with time and more c knowledge than I do can refactor it somewhat.
	//idk if that's possible tho.
	//please change variable names. if you can reduce loc or make it less complex do that as well 
	Software* software_list;
	Category to_return = { software_list, "template"};
	to_return.software_list =  malloc(sizeof(Software) * 100); //Idk why I'm mallocing here. call it future proofing.
	FILE* file;
	char current_string[100000];
	char key_string[500];
	int current_character;
	int software_count = 0;
	Mode parse_mode = NONE;	
	
    file = fopen(file_name, "r");
	if(file == NULL)
	{
	 	printf("file is null\n");
		return to_return;
	}
	do
	{	
		//essentially right here we grab a character from the file, and either write it to a buffer or take it as a control char.
		current_character = fgetc(file);
		switch(current_character)
		{	//this defined a header/id section
			case '[': parse_mode = ID; software_count++;  break;
			case ']': 
			{	
				//this gets the id of the element or smth, idk, I'm sleep deprived
				if(parse_mode == ID)
				{	
					
					strncpy(to_return.software_list[software_count - 1].id, current_string, sizeof(to_return.software_list[0].id)); 
					parse_mode=KEY;
					current_string[0] = '\0'; 
					
				}
			} break;
			//switches between key and value mode while erasin the buffer
			case '=': 
			{
				if(parse_mode == KEY)
				{
					strncpy(key_string, current_string, sizeof(key_string));
					parse_mode=VALUE;
					current_string[0] = '\0'; //yes this is how you 'erase' a string in c
				}	
				
			} break;
			
			case '\n':
			{	// extracts the value for each key and puts it in its appropriate slot in Software struct  but I have not yet implemented all keys
			 	// long if else chain because c doesn't support char* in switch statements, and hash mapping is too hard
				if(strcmp(key_string, "name") == 0 && parse_mode == VALUE )
				{ 	
					strncpy(to_return.software_list[software_count - 1].name, current_string, sizeof(to_return.software_list[0].name));						
				}
				
				else if(strcmp(key_string, "image_source") == 0 && parse_mode == VALUE)
				{
					strncpy(to_return.software_list[software_count - 1].image, current_string, sizeof(to_return.software_list[0].image));
				}
				else if(strcmp(key_string, "description") == 0 && parse_mode == VALUE)
				{
					strncpy(to_return.software_list[software_count -1].description, current_string, sizeof(to_return.software_list[0].description));
				}
				else if(strcmp(key_string, "package") == 0 && parse_mode == VALUE)
				{
					strncpy(to_return.software_list[software_count -1].package, current_string, sizeof(to_return.software_list[0].package));
				}
				
				current_string[0] = '\0';
				parse_mode = KEY;
				
			} break;
			default: 
			{
				if(parse_mode != NONE) append_char(current_string, (char)current_character); //feeds chars to the buffer
				 
			} break; 
		}			
	} while(current_character != EOF);
	to_return.software_count = software_count;
	fclose(file);
	return to_return;
}

char* build_ui_from_category(Category category)
{
	char* xml_ui_definition;
	xml_ui_definition = malloc(sizeof(char) * 10000);
	strcat(xml_ui_definition, "<interface>");
	for(int i=0; i < category.software_count; i++)
	{
		strcat(xml_ui_definition, "<box id=");
		strcat(xml_ui_definition, category.software_list[i].id);
		strcat(xml_ui_definition, "> <property name='orientation'>vertical<property>");
	}
	strcat(xml_ui_definition, "</interface>");
	return xml_ui_definition;
}

int main()
{
	Category c = read_category_from_file("/home/null/jovarkos-software-recommendations/views/template.category");
	char* xml = build_ui_from_category(c);
	printf("%s", xml);
}
