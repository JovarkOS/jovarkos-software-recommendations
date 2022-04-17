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
typedef enum { ID, KEY, VALUE, NONE } Mode;
