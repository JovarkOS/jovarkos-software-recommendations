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
{	Software* software_list;
	Category to_return = { software_list, "template"};
	to_return.software_list =  malloc(sizeof(Software) * 100);
	FILE* file;
	char current_string[100000];
	int current_character;
	int category_count = 0;
	Mode parse_mode = NONE;	
    file = fopen(file_name, "r");
	if(file == NULL)
	{
	 	printf("file is null\n");
		return to_return;
	}
	do
	{	
		
		current_character = fgetc(file);
		switch(current_character)
		{
			case '[': parse_mode = ID; break;
			case ']': strcpy(to_return.software_list[category_count].id, current_string); parse_mode=KEY; category_count++; break;
			default: 
			{
				if(parse_mode != NONE) append_char(current_string, (char)current_character); 
			}
			break; 
		}			
	} while(current_character != EOF);
	fclose(file);
	printf("%s", to_return.software_list[0].id);
	
}

int main()
{
		read_category_from_file("/home/null/jovarkos-software-recommendations/views/template.category");
}
