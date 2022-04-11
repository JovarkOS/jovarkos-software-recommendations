#define BUTTON_PADDING 3
#define CATEGORY_NUMBER 3
GtkWidget *window;
GtkWidget *category_buttons[CATEGORY_NUMBER];
const char* category_names[] = {"Utilities", "Games", "Applications"};
GtkWidget *box;

static void hide_buttons();
static void on_button_click();
static void activate(GtkApplication *app, gpointer user_data);
