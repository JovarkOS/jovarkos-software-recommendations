#ifndef GTK
#include "gtk/gtk.h"
#define GTK
#endif
#define BUTTON_PADDING 3

// Define list of category names found in SOFTWARE.ini
const char* category_names[] = {
    "Browsers",
    "Text Editors",
    "Terminal Emulators",
    "Media",
    "Internet Messaging",
    "Development Software"
};
//paths for each category (MUST be synchronized)
const char* category_paths[] = {
	"browsers.category",
    "texteditors.category",
    "terminalemulators.category",
    "media.category",
    "messaging.category",
    "development.category"
};

GtkWidget *category_container;
GtkWidget* main_box;

// Define the number of categories (find the number of elements)
#define CATEGORY_NUMBER sizeof(category_names) / sizeof(category_names[0])

GtkWidget *window;
GtkWidget *category_buttons[CATEGORY_NUMBER];

static void hide_buttons();
static void on_button_click();
static void activate(GtkApplication *app, gpointer user_data);
