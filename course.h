#ifndef COURSE_H
#define COURSE_H

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


int add_course_logic(Course new_course, Course cs[], int n);
int find_course_by_code(const char* code, Course cs[], int n);
int delete_course_by_index(int index, Course cs[], int n);
void save_courses(const char* path, Course cs[], int n);
int load_courses(const char* path, Course cs[]);
void getCSVPath(char *path, size_t size);

void showMenu();
void showTestPage();
void showAllPage(Course cs[], int n);
void addPage(Course cs[], int *n);
void searchPage(Course cs[], int n);
void updatePage(Course cs[], int n);
void deletePage(Course cs[], int *n);
void displayCourseTable(Course cs[], int n, const char* filter);
void setConsoleColor(WORD color);
void printHeader(const char* title);
void pauseForUser(const char* message);
char* strlwr(char* s);


void run_unit_tests();
void run_e2e_tests();


#endif 
