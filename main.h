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
    "Communication",
    "Development Software",
    "Utilities",
    "Gaming"
};
//paths for each category (MUST be synchronized)
const char* category_paths[] = {
	"browsers.category",
    "texteditors.category",
    "terminalemulators.category",
    "media.category",
    "communication.category",
    "development.category",
    "utities.category",
    "gaming.category"
};

GtkWidget* category_container;
GtkWidget* main_box;

// Define the number of categories (find the number of elements)
#define CATEGORY_NUMBER sizeof(category_names) / sizeof(category_names[0])

GtkWidget* window;
GtkWidget* scrolled;
GtkWidget *category_buttons[CATEGORY_NUMBER];
static void handle_back_button_click(GtkWidget *widget, gpointer data);
static void on_button_click(GtkWidget *widget, gpointer data);
static void activate(GtkApplication *app, gpointer user_data);
static void create_buttons();
