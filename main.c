#include "course.h"
#include "unittests.h"
#include "e2etests.h"

int main() {
    Course cs[100];
    int n = load_courses(NULL, cs); // Use load_courses for consistency
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
                case 6: showTestPage(); break;
                default:
                    setConsoleColor(C_WARNING);
                    printf("\n   Invalid choice.");
                    pauseForUser("\n   Press any key to try again...");
                    break;
            }
        } else if (stricmp(input, "exit") == 0) {
            break;
        } else {
            setConsoleColor(C_WARNING);
            printf("\n   Invalid command.");
            pauseForUser("\n   Press any key to try again...");
        }
    } while (1);

    setConsoleColor(C_SUCCESS);
    printf("\n   Program exited. Goodbye!\n");
    setConsoleColor(C_TEXT_DEFAULT);
    return 0;
}

// --- CORE LOGIC IMPLEMENTATIONS ---
// These functions contain the actual logic and are called by both the UI and the tests.

void getCSVPath(char *path, size_t size) {
    char exePath[250];
    if (GetModuleFileName(NULL, exePath, (DWORD)size)) {
        char *lastBackslash = strrchr(exePath, '\\');
        if (lastBackslash) *lastBackslash = '\0';
        snprintf(path, size, "%s\\courses.csv", exePath);
    } else {
        strcpy(path, "courses.csv");
    }
}

int load_courses(const char* path_override, Course cs[]) {
    char path[250];
    if (path_override) {
        strcpy(path, path_override);
    } else {
        getCSVPath(path, sizeof(path));
    }
    FILE *fp = fopen(path, "r");
    if (!fp) return 0;
    char line[256]; fgets(line, sizeof(line), fp);
    int n = 0;
    while (fgets(line, sizeof(line), fp) && n < 100) {
        if (sscanf(line, "%99[^,],%19[^,],%d,%99[^\n]", cs[n].name, cs[n].code, &cs[n].hours, cs[n].teacher) == 4) n++;
    }
    fclose(fp); return n;
}

void save_courses(const char* path_override, Course cs[], int n) {
    char path[250];
    if (path_override) {
        strcpy(path, path_override);
    } else {
        getCSVPath(path, sizeof(path));
    }
    FILE *fp = fopen(path, "w");
    if (!fp) return;
    fprintf(fp, "CourseName,CourseCode,Hours,Instructor\n");
    for (int i = 0; i < n; i++) fprintf(fp, "%s,%s,%d,%s\n", cs[i].name, cs[i].code, cs[i].hours, cs[i].teacher);
    fclose(fp);
}

int add_course_logic(Course new_course, Course cs[], int n) {
    if (find_course_by_code(new_course.code, cs, n) != -1) return n;
    cs[n] = new_course;
    return n + 1;
}

int find_course_by_code(const char* code, Course cs[], int n) {
    char search_code[20]; strcpy(search_code, code); strlwr(search_code);
    for (int i = 0; i < n; i++) {
        char temp_code[20]; strcpy(temp_code, cs[i].code); strlwr(temp_code);
        if (strcmp(temp_code, search_code) == 0) return i;
    }
    return -1;
}

int delete_course_by_index(int index, Course cs[], int n) {
    if (index < 0 || index >= n) return n;
    for (int j = index; j < n - 1; j++) cs[j] = cs[j + 1];
    return n - 1;
}


// --- UI PAGE IMPLEMENTATIONS ---
// These functions handle user interaction and call the core logic functions.

void showMenu() {
    printHeader("COURSE MANAGEMENT SYSTEM - Main Menu");
    printf("   [1] Show All Courses\n   [2] Add New Course\n   [3] Search for a Course\n   [4] Edit a Course\n   [5] Delete a Course\n   [6] Run Tests\n\n");
    setConsoleColor(C_WARNING); printf("   Type 'exit' to quit the program\n");
    setConsoleColor(C_PROMPT); printf("\n   Please select an option >> ");
    setConsoleColor(C_TEXT_DEFAULT);
}

void showTestPage() {
    char choice[10];
    system("cls");
    printHeader("Testing Menu");
    printf("   [1] Run Unit Tests\n");
    printf("   [2] Run End-to-End Test\n\n");
    setConsoleColor(C_PROMPT);
    printf("   Please select an option (or 'back') >> ");
    setConsoleColor(C_TEXT_DEFAULT);
    scanf("%s", choice);
    if (strcmp(choice, "1") == 0) {
        run_unit_tests();
    } else if (strcmp(choice, "2") == 0) {
        run_e2e_tests();
    }
    pauseForUser("Press any key to return to the main menu...");
}

void addPage(Course cs[], int *n) {
    Course new_course;
    system("cls");
    printHeader("Add New Course");
    printf("\n   Enter New Course Name (or 'back')\n   >> ");
    scanf(" %[^\n]", new_course.name);
    if (stricmp(new_course.name, "back") == 0) return;
    printf("   Enter Course Code >> ");
    scanf("%s", new_course.code);
    
    int original_n = *n;
    *n = add_course_logic(new_course, cs, *n);

    if (*n > original_n) {
        printf("   Enter Credit Hours >> ");
        scanf("%d", &cs[*n-1].hours);
        printf("   Enter Instructor Name >> ");
        scanf(" %[^\n]", cs[*n-1].teacher);
        save_courses(NULL, cs, *n);
        setConsoleColor(C_SUCCESS);
        printf("\n   SUCCESS: Course '%s' has been added.", new_course.name);
    } else {
        setConsoleColor(C_WARNING);
        printf("\n   Warning: A course with this code already exists.");
    }
    pauseForUser("\n   Press any key to continue...");
}

void deletePage(Course cs[], int *n) {
    char code[20];
    system("cls");
    printHeader("Delete Course");
    displayCourseTable(cs, *n, "");
    printf("\n   Enter CODE of course to delete (or 'back')\n   >> ");
    scanf("%s", code);
    if (stricmp(code, "back") == 0) return;
    
    int idx = find_course_by_code(code, cs, *n);

    if (idx != -1) {
        char confirm[10];
        printf("   Are you sure you want to delete '%s'? (yes/no): ", cs[idx].name);
        scanf("%s", confirm);
        if (stricmp(confirm, "yes") == 0) {
            *n = delete_course_by_index(idx, cs, *n);
            save_courses(NULL, cs, *n);
            printf("\n   SUCCESS: Course has been deleted.");
        } else {
            printf("\n   Deletion cancelled.");
        }
    } else {
        printf("\n   Warning: Course with code '%s' not found.", code);
    }
    pauseForUser("\n   Press any key to continue...");
}

void showAllPage(Course cs[], int n) {
    system("cls");
    printHeader("All Courses");
    displayCourseTable(cs, n, "");
    pauseForUser("\n   Press any key to return to main menu...");
}

void searchPage(Course cs[], int n) {
    char filter[100];
    system("cls");
    printHeader("Search Courses");
    printf("\n   Enter search keyword (or 'back')\n   >> ");
    scanf("%s", filter);
    if (stricmp(filter, "back") == 0) return;
    displayCourseTable(cs, n, filter);
    pauseForUser("\n   Press any key to return to main menu...");
}

void updatePage(Course cs[], int n) {
    char code[20], input[100];
    system("cls");
    printHeader("Edit Course Information");
    displayCourseTable(cs, n, "");
    printf("\n   Enter CODE of course to edit (or 'back')\n   >> ");
    scanf("%s", code);
    if (stricmp(code, "back") == 0) return;
    int idx = find_course_by_code(code, cs, n);
    if (idx != -1) {
        printf("\n   Editing: %s\n", cs[idx].name);
        printf("   Enter new credit hours (current: %d) or Enter to skip\n   >> ", cs[idx].hours);
        scanf(" %[^\n]", input);
        if (strlen(input) > 0 && isdigit(input[0])) cs[idx].hours = atoi(input);
        printf("   Enter new instructor (current: %s) or Enter to skip\n   >> ", cs[idx].teacher);
        scanf(" %[^\n]", input);
        if (strlen(input) > 0) strcpy(cs[idx].teacher, input);
        save_courses(NULL, cs, n);
        printf("\n   SUCCESS: Course has been updated.");
    } else {
        printf("\n   Warning: Course with code '%s' not found.", code);
    }
    pauseForUser("\n   Press any key to continue...");
}


// --- UI HELPER IMPLEMENTATIONS ---

void displayCourseTable(Course cs[], int n, const char* filter) {
    int count = 0; char temp_name[100], temp_code[20], temp_filter[100];
    strcpy(temp_filter, filter); strlwr(temp_filter);
    printf("\n   %-15s %-40s %-10s %-30s\n", "CODE", "COURSE NAME", "HOURS", "INSTRUCTOR");
    for(int i=0; i<105; i++) printf("-"); printf("\n");
    if (n == 0) { printf("   No courses found.\n"); }
    else {
        for (int i = 0; i < n; i++) {
            strcpy(temp_name, cs[i].name); strcpy(temp_code, cs[i].code);
            if (strlen(filter) == 0 || strstr(strlwr(temp_name), temp_filter) || strstr(strlwr(temp_code), temp_filter)) {
                printf("   %-15s %-40s %-10d %-30s\n", cs[i].code, cs[i].name, cs[i].hours, cs[i].teacher);
                count++;
            }
        }
        if (count == 0) { printf("   No courses match your search criteria.\n"); }
    }
}

void printHeader(const char* title) {
    printf("=========================================================================================================\n");
    printf("                                     %s\n", title);
    printf("=========================================================================================================\n");
}

void pauseForUser(const char* message) {
    printf("%s", message);
    getch();
}

void setConsoleColor(WORD color) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); }
char* strlwr(char* s) { for (char *p = s; *p; p++) *p = (char)tolower((unsigned char)*p); return s; }