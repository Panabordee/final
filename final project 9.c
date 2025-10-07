#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>

#define C_BORDER 9
#define C_HEADER 11
#define C_TEXT_DEFAULT 15
#define C_TEXT_ALT 7
#define C_SUCCESS 10
#define C_WARNING 14
#define C_PROMPT 11

typedef struct {
    char code[20], name[100], teacher[100];
    int hours;
} Course;

void getCSVPath(char *path, size_t size);
int load(Course cs[]);
void save(Course cs[], int n);
void showAllPage(Course cs[], int n);
void addPage(Course cs[], int *n);
void searchPage(Course cs[], int n);
void updatePage(Course cs[], int n);
void deletePage(Course cs[], int *n);
void displayCourseTable(Course cs[], int n, const char* filter);
void setConsoleColor(WORD color);
void showMenu();
void printHeader(const char* title);
void pauseForUser(const char* message);
char* strlwr(char* s);

int main() {
    Course cs[100];
    int n = load(cs);
    char input[20];

    do {
        system("cls");
        showMenu();
        scanf("%s", input);

        if (isdigit(input[0]) && strlen(input) == 1) {
            int choice = atoi(input);
            switch (choice) {
                case 1: showAllPage(cs, n); break;
                case 2: addPage(cs, &n); break;
                case 3: searchPage(cs, n); break;
                case 4: updatePage(cs, n); break;
                case 5: deletePage(cs, &n); break;
                default:
                    setConsoleColor(C_WARNING);
                    printf("\n   Invalid choice. Please enter a number from 1-5.");
                    pauseForUser("\n   Press any key to try again...");
                    break;
            }
        } else if (stricmp(input, "exit") == 0) {
            break;
        } else {
            setConsoleColor(C_WARNING);
            printf("\n   Invalid command. Please enter a number or 'exit'.");
            pauseForUser("\n   Press any key to try again...");
        }
    } while (1);

    printf("\n");
    setConsoleColor(C_SUCCESS);
    printf("   Program exited. Goodbye!\n");
    setConsoleColor(C_TEXT_DEFAULT);
    return 0;
}

void showAllPage(Course cs[], int n) {
    char filter[100] = "";
    while (1) {
        system("cls");
        printHeader("All Courses (Live Search)");
        if (stricmp(filter, "") != 0) {
            setConsoleColor(C_TEXT_ALT);
            printf("   Current filter: '%s'\n", filter);
        }
        
        displayCourseTable(cs, n, filter);

        setConsoleColor(C_PROMPT);
        printf("\n   >> Type to filter list, or type 'back' to return to menu: ");
        setConsoleColor(C_TEXT_DEFAULT);
        scanf(" %[^\n]", filter);

        if (stricmp(filter, "back") == 0) break;
    }
}

void addPage(Course cs[], int *n) {
    char name[100], code[20], teacher[100];
    int hours, duplicate;

    while(1) {
        system("cls");
        printHeader("Add New Course");
        displayCourseTable(cs, *n, "");
        
        setConsoleColor(C_PROMPT);
        printf("\n   Enter New Course Name (or type 'back' to return to menu)\n   >> ");
        setConsoleColor(C_TEXT_DEFAULT);
        scanf(" %[^\n]", name);
        if (stricmp(name, "back") == 0) break;

        duplicate = 0;
        for (int i = 0; i < *n; i++) {
            if (!stricmp(cs[i].name, name)) {
                setConsoleColor(C_WARNING);
                printf("\n   Warning: A course with this name already exists.");
                pauseForUser("\n   Press any key to try again...");
                duplicate = 1;
                break;
            }
        }
        if (duplicate) continue;

        setConsoleColor(C_PROMPT);
        printf("   Enter Course Code for '%s' (or 'back' to cancel)\n   >> ", name);
        setConsoleColor(C_TEXT_DEFAULT);
        scanf("%s", code);
        if (stricmp(code, "back") == 0) continue;

        for (int i = 0; i < *n; i++) {
            if (!stricmp(cs[i].code, code)) {
                setConsoleColor(C_WARNING);
                printf("\n   Warning: A course with this code already exists.");
                pauseForUser("\n   Press any key to try again...");
                duplicate = 1;
                break;
            }
        }
        if (duplicate) continue;

        setConsoleColor(C_PROMPT);
        printf("   Enter Credit Hours >> ");
        setConsoleColor(C_TEXT_DEFAULT);
        scanf("%d", &hours);

        setConsoleColor(C_PROMPT);
        printf("   Enter Instructor Name >> ");
        setConsoleColor(C_TEXT_DEFAULT);
        scanf(" %[^\n]", teacher);

        strcpy(cs[*n].name, name);
        strcpy(cs[*n].code, code);
        strcpy(cs[*n].teacher, teacher);
        cs[*n].hours = hours;
        (*n)++;
        save(cs, *n);

        setConsoleColor(C_SUCCESS);
        printf("\n   SUCCESS: Course '%s' has been added.", name);
        pauseForUser("\n   Press any key to add another course...");
    }
}

void searchPage(Course cs[], int n) {
    char key[100];
    system("cls");
    printHeader("Search Courses");
    displayCourseTable(cs, n, "");
    
    setConsoleColor(C_PROMPT);
    printf("\n   Enter search term (name or code), or 'back' to return\n   >> ");
    setConsoleColor(C_TEXT_DEFAULT);
    scanf(" %[^\n]", key);

    if (stricmp(key, "back") != 0) {
        printf("\n   --- Search Results ---\n");
        displayCourseTable(cs, n, key);
        pauseForUser("\n   Press any key to return to main menu...");
    }
}

void updatePage(Course cs[], int n) {
    char code[20], input[100], confirm[10];
    int original_hours;
    char original_teacher[100];
    
    while(1) {
        system("cls");
        printHeader("Edit Course Information");
        displayCourseTable(cs, n, "");

        setConsoleColor(C_PROMPT);
        printf("\n   Enter the CODE of the course to edit (or 'back' to return)\n   >> ");
        setConsoleColor(C_TEXT_DEFAULT);
        scanf("%s", code);
        if (stricmp(code, "back") == 0) break;

        int found_idx = -1;
        for (int i = 0; i < n; i++) {
            if (!stricmp(cs[i].code, code)) {
                found_idx = i;
                break;
            }
        }

        if (found_idx == -1) {
            setConsoleColor(C_WARNING);
            printf("\n   Warning: Course with code '%s' not found.", code);
            pauseForUser("\n   Press any key to try again...");
            continue;
        }

        setConsoleColor(C_TEXT_ALT);
        printf("\n   Editing Course: ");
        setConsoleColor(C_TEXT_DEFAULT);
        printf("%s (%s)\n", cs[found_idx].name, cs[found_idx].code);
        
        original_hours = cs[found_idx].hours;
        strcpy(original_teacher, cs[found_idx].teacher);

        int new_hours = original_hours;
        char new_teacher[100];
        strcpy(new_teacher, original_teacher);

        setConsoleColor(C_PROMPT);
        printf("   Enter new credit hours (current: %d) or press Enter to skip\n   >> ", original_hours);
        setConsoleColor(C_TEXT_DEFAULT);
        scanf(" %[^\n]", input);
        if (strlen(input) > 0) new_hours = atoi(input);

        setConsoleColor(C_PROMPT);
        printf("   Enter new instructor (current: %s) or press Enter to skip\n   >> ", original_teacher);
        setConsoleColor(C_TEXT_DEFAULT);
        scanf(" %[^\n]", input);
        if (strlen(input) > 0) strcpy(new_teacher, input);

        if (new_hours == original_hours && strcmp(new_teacher, original_teacher) == 0) {
            setConsoleColor(C_TEXT_ALT);
            printf("\n   No changes were made.");
            pauseForUser("\n   Press any key to continue...");
            continue;
        }

        setConsoleColor(C_WARNING);
        printf("\n   Please confirm changes for %s:\n", cs[found_idx].code);
        printf("   Hours: %d -> %d\n", original_hours, new_hours);
        printf("   Instructor: '%s' -> '%s'\n", original_teacher, new_teacher);
        printf("   Save these changes? (yes or no): ");
        setConsoleColor(C_TEXT_DEFAULT);
        scanf("%s", confirm);

        if (stricmp(confirm, "yes") == 0) {
            cs[found_idx].hours = new_hours;
            strcpy(cs[found_idx].teacher, new_teacher);
            save(cs, n);
            setConsoleColor(C_SUCCESS);
            printf("\n   SUCCESS: Course '%s' has been updated.", cs[found_idx].name);
        } else {
            setConsoleColor(C_TEXT_ALT);
            printf("\n   Changes discarded.");
        }
        pauseForUser("\n   Press any key to edit another course...");
    }
}

void deletePage(Course cs[], int *n) {
    char key[100], confirm[10];

    while(1) {
        system("cls");
        printHeader("Delete Course");
        displayCourseTable(cs, *n, "");
        if (*n == 0) {
            pauseForUser("\n   There are no courses to delete. Press any key to return...");
            break;
        }

        setConsoleColor(C_PROMPT);
        printf("\n   Enter exact Name or Code of the course to delete (or 'back')\n   >> ");
        setConsoleColor(C_TEXT_DEFAULT);
        scanf(" %[^\n]", key);
        if (stricmp(key, "back") == 0) break;

        int found_idx = -1;
        for (int i = 0; i < *n; i++) {
            if (!stricmp(cs[i].name, key) || !stricmp(cs[i].code, key)) {
                found_idx = i;
                break;
            }
        }

        if (found_idx == -1) {
            setConsoleColor(C_WARNING);
            printf("\n   Warning: Course '%s' not found.", key);
            pauseForUser("\n   Press any key to try again...");
            continue;
        }
        
        setConsoleColor(C_WARNING);
        printf("   Are you sure you want to delete '%s' (%s)? (yes or no): ", cs[found_idx].name, cs[found_idx].code);
        setConsoleColor(C_TEXT_DEFAULT);
        scanf("%s", confirm);

        if (stricmp(confirm, "yes") == 0) {
            char deletedName[100];
            strcpy(deletedName, cs[found_idx].name);

            for (int j = found_idx; j < *n - 1; j++) {
                cs[j] = cs[j + 1];
            }
            (*n)--;
            save(cs, *n);

            setConsoleColor(C_SUCCESS);
            printf("\n   SUCCESS: Course '%s' has been deleted.", deletedName);
            pauseForUser("\n   Press any key to delete another course...");
        } else {
            setConsoleColor(C_TEXT_ALT);
            printf("\n   Deletion cancelled.");
            pauseForUser("\n   Press any key to continue...");
        }
    }
}

void displayCourseTable(Course cs[], int n, const char* filter) {
    setConsoleColor(C_BORDER);
    printf("\n   %c", 201); for(int i=0; i<86; i++) printf("%c", 205); printf("%c\n", 187);
    setConsoleColor(C_HEADER);
    printf("   %c %-30s %c %-8s %c %-5s %c %-20s %c\n", 186, "Course Name", 186, "Code", 186, "Hours", 186, "Instructor", 186);
    setConsoleColor(C_BORDER);
    printf("   %c", 204); for(int i=0; i<32; i++) printf("%c", 205); printf("%c", 206); for(int i=0; i<10; i++) printf("%c", 205); printf("%c", 206); for(int i=0; i<7; i++) printf("%c", 205); printf("%c", 206); for(int i=0; i<22; i++) printf("%c", 205); printf("%c\n", 185);
    
    int found = 0;
    char temp_name[100], temp_code[20], temp_filter[100];
    strcpy(temp_filter, filter);
    strlwr(temp_filter);

    if (n == 0) {
         setConsoleColor(C_TEXT_DEFAULT);
         printf("   %c %-84s %c\n", 186, "No courses in the system yet.", 186);
         found = 1;
    } else {
        for (int i = 0; i < n; i++) {
            strcpy(temp_name, cs[i].name);
            strcpy(temp_code, cs[i].code);
            strlwr(temp_name);
            strlwr(temp_code);

            if (strlen(filter) == 0 || strstr(temp_name, temp_filter) || strstr(temp_code, temp_filter)) {
                setConsoleColor(C_TEXT_DEFAULT);
                printf("   %c %-30s ", 186, cs[i].name);
                setConsoleColor(C_BORDER); printf("%c", 186);
                setConsoleColor(C_TEXT_DEFAULT); printf(" %-8s ", cs[i].code);
                setConsoleColor(C_BORDER); printf("%c", 186);
                setConsoleColor(C_TEXT_DEFAULT); printf(" %-5d ", cs[i].hours);
                setConsoleColor(C_BORDER); printf("%c", 186);
                setConsoleColor(C_TEXT_DEFAULT); printf(" %-20s ", cs[i].teacher);
                setConsoleColor(C_BORDER); printf("%c\n", 186);
                found = 1;
            }
        }
    }

    if (!found) {
        setConsoleColor(C_TEXT_DEFAULT);
        printf("   %c %-84s %c\n", 186, "No courses found matching criteria.", 186);
    }
    setConsoleColor(C_BORDER);
    printf("   %c", 200); for(int i=0; i<86; i++) printf("%c", 205); printf("%c\n", 188);
    setConsoleColor(C_TEXT_DEFAULT);
}

void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void showMenu() {
    char header_title[60];
    sprintf(header_title, "COURSE MANAGEMENT SYSTEM");
    
    setConsoleColor(C_BORDER);
    printf("\n   %c", 201); for(int i=0; i<52; i++) printf("%c", 205); printf("%c\n", 187);
    printf("   %c", 186);
    setConsoleColor(C_HEADER);
    printf(" %-50s ", header_title);
    setConsoleColor(C_BORDER);
    printf("%c\n", 186);
    printf("   %c", 186);
    printf(" %-50s ", "Main Menu");
    printf("%c\n", 186);
    printf("   %c", 204); for(int i=0; i<52; i++) printf("%c", 205); printf("%c\n", 185);

    setConsoleColor(C_TEXT_DEFAULT);
    printf("   %c %-50s %c\n", 186, "[1] Show All Courses (with live search)", 186);
    printf("   %c %-50s %c\n", 186, "[2] Add New Course", 186);
    printf("   %c %-50s %c\n", 186, "[3] Search for a Course", 186);
    printf("   %c %-50s %c\n", 186, "[4] Edit a Course", 186);
    printf("   %c %-50s %c\n", 186, "[5] Delete a Course", 186);
    printf("   %c %-50s %c\n", 186, "", 186);

    setConsoleColor(C_WARNING);
    printf("   %c %-50s %c\n", 186, "Type 'exit' to quit the program", 186);
    setConsoleColor(C_BORDER);
    printf("   %c", 204); for(int i=0; i<52; i++) printf("%c", 205); printf("%c\n", 185);

    setConsoleColor(C_PROMPT);
    printf("   %c Please select an option >> ", 186);
    setConsoleColor(C_TEXT_DEFAULT);
}


void printHeader(const char* title) {
    char header_title[60];
    sprintf(header_title, "COURSE MANAGEMENT SYSTEM");
    
    setConsoleColor(C_BORDER);
    printf("\n   %c", 201); for(int i=0; i<52; i++) printf("%c", 205); printf("%c\n", 187);
    printf("   %c", 186);
    setConsoleColor(C_HEADER);
    printf(" %-50s ", header_title);
    setConsoleColor(C_BORDER);
    printf("%c\n", 186);
    printf("   %c", 204); for(int i=0; i<52; i++) printf("%c", 205); printf("%c\n", 185);
    printf("   %c", 186);
    setConsoleColor(C_TEXT_DEFAULT);
    printf(" %-50s ", title);
    setConsoleColor(C_BORDER);
    printf("%c\n", 186);
    printf("   %c", 200); for(int i=0; i<52; i++) printf("%c", 205); printf("%c\n\n", 188);
    setConsoleColor(C_TEXT_DEFAULT);
}

void pauseForUser(const char* message) {
    setConsoleColor(C_TEXT_ALT);
    printf("%s", message);
    _getch();
    setConsoleColor(C_TEXT_DEFAULT);
}

char* strlwr(char* s) {
    for (char *p = s; *p; p++) {
        *p = (char)tolower((unsigned char)*p);
    }
    return s;
}

void getCSVPath(char *path, size_t size) {
    char exePath[250];
    if (GetModuleFileName(NULL, exePath, (DWORD)size)) {
        char *lastBackslash = strrchr(exePath, '\\');
        if (lastBackslash) *lastBackslash = '\0';
        snprintf(path, size, "%s\\courses.csv", exePath);
    } else {
        printf("Error getting executable path\n");
        path[0] = '\0';
    }
}

int load(Course cs[]) {
    char path[250];
    getCSVPath(path, sizeof(path));
    FILE *fp = fopen(path, "r");
    if (!fp) return 0;

    char line[256];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return 0;
    }

    int n = 0;
    while (fgets(line, sizeof(line), fp) && n < 100) {
        if (sscanf(line, "%99[^,],%19[^,],%d,%99[^\n]",
                   cs[n].name, cs[n].code, &cs[n].hours, cs[n].teacher) == 4) {
            n++;
        }
    }
    fclose(fp);
    return n;
}

void save(Course cs[], int n) {
    char path[250];
    getCSVPath(path, sizeof(path));
    FILE *fp = fopen(path, "w");
    if (!fp) {
        printf("Error saving file\n");
        return;
    }
    fprintf(fp, "CourseName,CourseCode,Hours,Instructor\n");
    for (int i = 0; i < n; i++)
        fprintf(fp, "%s,%s,%d,%s\n", cs[i].name, cs[i].code, cs[i].hours, cs[i].teacher);
    fclose(fp);
}
