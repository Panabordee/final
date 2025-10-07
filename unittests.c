#include "unittests.h"
int simulate_add(Course new_course, Course courses[], int current_count) {
    for (int i = 0; i < current_count; i++) {
        if (stricmp(courses[i].code, new_course.code) == 0) {
            return current_count;
        }
    }
    courses[current_count] = new_course;
    return current_count + 1;

int simulate_find(const char* code, Course courses[], int current_count) {
    char search_code[20];
    strcpy(search_code, code);
    strlwr(search_code);
    for (int i = 0; i < current_count; i++) {
        char temp_code[20];
        strcpy(temp_code, courses[i].code);
        strlwr(temp_code);
        if (strcmp(temp_code, search_code) == 0) {
            return i;
        }
    }
    return -1;
}


void run_unit_tests() {
    printHeader("UNIT TEST SUITE");
    printf("\n   Running in-memory logic tests...\n\n");

    Course test_courses[100];
    int n = 0;
    int all_tests_passed = 1;


    printf("   [TEST 1] ADD: Adding a single valid course...");
    Sleep(500); 
    Course course1 = {"CS101", "Algorithms", "Alice", 40};
    n = simulate_add(course1, test_courses, n);
    if (n == 1 && strcmp(test_courses[0].code, "CS101") == 0) {
        setConsoleColor(C_SUCCESS); printf(" PASS\n");
    } else {
        setConsoleColor(C_WARNING); printf(" FAIL\n"); all_tests_passed = 0;
    }
    setConsoleColor(C_TEXT_DEFAULT);

    printf("   [TEST 2] ADD: Adding a second course...");
    Sleep(500);
    Course course2 = {"CS102", "Databases", "Bob", 35};
    n = simulate_add(course2, test_courses, n);
    if (n == 2 && strcmp(test_courses[1].code, "CS102") == 0) {
        setConsoleColor(C_SUCCESS); printf(" PASS\n");
    } else {
        setConsoleColor(C_WARNING); printf(" FAIL\n"); all_tests_passed = 0;
    }
    setConsoleColor(C_TEXT_DEFAULT);

    
    printf("   [TEST 3] ADD: Attempting to add a duplicate (CS101)...");
    Sleep(500); 
    Course course_dup = {"CS101", "Duplicate Test", "Charlie", 30};
    int original_n = n;
    n = simulate_add(course_dup, test_courses, n);
    if (n == original_n) {
        setConsoleColor(C_SUCCESS); printf(" PASS\n");
    } else {
        setConsoleColor(C_WARNING); printf(" FAIL\n"); all_tests_passed = 0;
    }
    setConsoleColor(C_TEXT_DEFAULT);


    printf("   [TEST 4] FIND: Searching for an existing course (CS102)...");
    Sleep(500); 
    int found_index = simulate_find("CS102", test_courses, n);
    if (found_index == 1) {
        setConsoleColor(C_SUCCESS); printf(" PASS\n");
    } else {
        setConsoleColor(C_WARNING); printf(" FAIL\n"); all_tests_passed = 0;
    }
    setConsoleColor(C_TEXT_DEFAULT);

    printf("   [TEST 5] FIND: Searching with different casing (cs101)...");
    Sleep(500); 
    found_index = simulate_find("cs101", test_courses, n);
    if (found_index == 0) {
        setConsoleColor(C_SUCCESS); printf(" PASS\n");
    } else {
        setConsoleColor(C_WARNING); printf(" FAIL\n"); all_tests_passed = 0;
    }
    setConsoleColor(C_TEXT_DEFAULT);
    

    printf("   [TEST 6] FIND: Searching for a non-existent course (CS999)...");
    Sleep(500); 
    found_index = simulate_find("CS999", test_courses, n);
    if (found_index == -1) {
        setConsoleColor(C_SUCCESS); printf(" PASS\n");
    } else {
        setConsoleColor(C_WARNING); printf(" FAIL\n"); all_tests_passed = 0;
    }
    setConsoleColor(C_TEXT_DEFAULT);

    
    printf("\n----------------------------------------\n");
    if (all_tests_passed) {
        setConsoleColor(C_SUCCESS);
        printf("   SUMMARY: All Unit Tests Passed!\n");
    } else {
        setConsoleColor(C_WARNING);
        printf("   SUMMARY: Some Unit Tests Failed!\n");
    }
    setConsoleColor(C_TEXT_DEFAULT);
    printf("----------------------------------------\n\n");
    printf("   Unit tests complete.\n");

}
