#ifndef GTK
#include "gtk/gtk.h"
#define GTK
#endif
#define IMAGE_RESOLUTION_X 480
#define IMAGE_RESOLUTION_Y 270
typedef struct {
	char id[50];
	char package[200];
	char name[100];
	char image[400];
	char description[1000];
} Software;

typedef struct {
	 Software* software_list;
	 char* name;
	 int software_count;
} Category;

Category read_category_from_file(const char* fileName);
GtkWidget*  build_ui_from_category(Category c);
typedef enum { ID, KEY, VALUE, NONE } Mode;
void callback(GtkWidget *widget, gpointer data );

