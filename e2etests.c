#include "e2etests.h"
#include <time.h>

static char E2E_TEST_PATH[250];

void report_step(const char* scenario, int step, const char* name, int passed) {
    if (step == 1) {
        setConsoleColor(C_PROMPT);
        printf("\n--- SCENARIO: %s ---\n", scenario);
        setConsoleColor(C_TEXT_DEFAULT);
    }
    printf("    ");
    if (passed) {
        setConsoleColor(C_SUCCESS);
        printf("[PASS]");
    } else {
        setConsoleColor(C_WARNING);
        printf("[FAIL]");
    }
    setConsoleColor(C_TEXT_DEFAULT);
    printf(" Step %d: %s\n", step, name);

    if (!passed) {
        setConsoleColor(C_WARNING);
        printf("\n    Critical workflow failure. Test aborted.\n");
        pauseForUser("    Press any key to return to menu...");
        exit(1);
    }
}

Course buffer[100];
int n = 0;

void setup_initial_state() {
    n = 0;
    memset(buffer, 0, sizeof(buffer));
    remove(E2E_TEST_PATH);
}

void run_scenario_1_initial_population() {
    setup_initial_state();
    report_step("Initial Population", 1, "Clean up previous test file", 1);

    n = add_course_logic((Course){"CS101", "Algorithms", "Alice", 40}, buffer, n);
    n = add_course_logic((Course){"DB202", "Databases", "Bob", 35}, buffer, n);
    n = add_course_logic((Course){"NT303", "Networks", "Charlie", 45}, buffer, n);
    n = add_course_logic((Course){"OS404", "Operating Systems", "Diana", 50}, buffer, n);
    n = add_course_logic((Course){"SE505", "Software Engineering", "Eve", 60}, buffer, n);
    report_step("Initial Population", 2, "Five initial courses created in memory", n == 5);

    save_courses(E2E_TEST_PATH, buffer, n);
    report_step("Initial Population", 3, "Saved initial 5 courses to disk", 1);
    
    n = 0;
    memset(buffer, 0, sizeof(buffer));

    n = load_courses(E2E_TEST_PATH, buffer);
    report_step("Initial Population", 4, "Loaded 5 courses back from disk", n == 5);
    report_step("Initial Population", 5, "Verify data integrity of OS404 (Teacher: Diana)", strcmp(buffer[3].teacher, "Diana") == 0);
    report_step("Initial Population", 6, "Verify data integrity of CS101 (Name: Algorithms)", strcmp(buffer[0].name, "Algorithms") == 0);
}

void run_scenario_2_search_workflow() {
    report_step("Search Workflow", 1, "Starting with 5 courses from previous scenario", n == 5);

    int idx = find_course_by_code("DB202", buffer, n);
    report_step("Search Workflow", 2, "Search for existing middle course 'DB202'", idx == 1);

    idx = find_course_by_code("se505", buffer, n);
    report_step("Search Workflow", 3, "Search for existing last course with lowercase 'se505'", idx == 4);
    
    idx = find_course_by_code("CS999", buffer, n);
    report_step("Search Workflow", 4, "Search for non-existent course 'CS999'", idx == -1);

    save_courses(E2E_TEST_PATH, buffer, n);
    report_step("Search Workflow", 5, "Saved state to disk (no changes made)", 1);
    
    n = 0; memset(buffer, 0, sizeof(buffer));
    n = load_courses(E2E_TEST_PATH, buffer);
    report_step("Search Workflow", 6, "Reloaded data from disk", n==5);
    
    idx = find_course_by_code("NT303", buffer, n);
    report_step("Search Workflow", 7, "Verify 'NT303' still exists after reload", idx == 2);
}

void run_scenario_3_modification_cycle() {
    report_step("Modification Cycle", 1, "Starting with 5 courses from previous scenario", n == 5);
    
    int idx_update_nt = find_course_by_code("NT303", buffer, n);
    strcpy(buffer[idx_update_nt].name, "Advanced Computer Networks");
    buffer[idx_update_nt].hours = 55;
    report_step("Modification Cycle", 2, "Modified course NT303 in memory", 1);
    
    int idx_update_db = find_course_by_code("DB202", buffer, n);
    strcpy(buffer[idx_update_db].teacher, "Robert");
    report_step("Modification Cycle", 3, "Modified teacher for DB202 in memory", 1);

    int idx_delete = find_course_by_code("CS101", buffer, n);
    n = delete_course_by_index(idx_delete, buffer, n);
    report_step("Modification Cycle", 4, "Deleted course CS101 from memory", n == 4);

    n = add_course_logic((Course){"AI606", "Artificial Intelligence", "Frank", 65}, buffer, n);
    report_step("Modification Cycle", 5, "Added new course AI606 to memory", n == 5);
    
    save_courses(E2E_TEST_PATH, buffer, n);
    report_step("Modification Cycle", 6, "Saved all modifications to disk", 1);
}

void run_scenario_4_final_verification() {
    n = 0;
    memset(buffer, 0, sizeof(buffer));
    report_step("Final Verification", 1, "Cleared memory buffer for clean load", 1);

    n = load_courses(E2E_TEST_PATH, buffer);
    report_step("Final Verification", 2, "Loaded final state from disk", n == 5);

    int final_idx_nt = find_course_by_code("NT303", buffer, n);
    int check1 = strcmp(buffer[final_idx_nt].name, "Advanced Computer Networks") == 0;
    int check2 = buffer[final_idx_nt].hours == 55;
    report_step("Final Verification", 3, "Verify NT303 modifications were saved", check1 && check2);
    
    int final_idx_db = find_course_by_code("DB202", buffer, n);
    report_step("Final Verification", 4, "Verify DB202 teacher update was saved", strcmp(buffer[final_idx_db].teacher, "Robert") == 0);

    int final_idx_delete = find_course_by_code("CS101", buffer, n);
    report_step("Final Verification", 5, "Verify CS101 is permanently deleted", final_idx_delete == -1);

    int final_idx_added = find_course_by_code("AI606", buffer, n);
    report_step("Final Verification", 6, "Verify new course AI606 exists", final_idx_added != -1);
}

void run_scenario_5_stress_test() {
    report_step("Stress Test", 1, "Starting with 5 courses from previous scenario", n == 5);
    
    for(int i = 0; i < 50; i++) {
        char code[20];
        char name[50];
        sprintf(code, "STR%03d", i);
        sprintf(name, "Stress Course %d", i);
        Course temp_course;
        strcpy(temp_course.code, code);
        strcpy(temp_course.name, name);
        strcpy(temp_course.teacher, "Bot");
        temp_course.hours = 10;
        n = add_course_logic(temp_course, buffer, n);
    }
    report_step("Stress Test", 2, "Added 50 new courses to memory buffer", n == 55);

    save_courses(E2E_TEST_PATH, buffer, n);
    report_step("Stress Test", 3, "Saved 55 courses to disk", 1);

    n = 0;
    memset(buffer, 0, sizeof(buffer));
    n = load_courses(E2E_TEST_PATH, buffer);
    report_step("Stress Test", 4, "Loaded 55 courses from disk", n == 55);

    report_step("Stress Test", 5, "Verify last added course 'STR049' exists", find_course_by_code("STR049", buffer, n) != -1);
    
    for(int i = 0; i < 25; i++) {
        n = delete_course_by_index(0, buffer, n);
    }
    report_step("Stress Test", 6, "Deleted first 25 courses from memory", n == 30);
    
    save_courses(E2E_TEST_PATH, buffer, n);
    report_step("Stress Test", 7, "Saved state after deletion to disk", 1);
    
    n = 0;
    memset(buffer, 0, sizeof(buffer));
    n = load_courses(E2E_TEST_PATH, buffer);
    report_step("Stress Test", 8, "Loaded final state from disk", n == 30);
    report_step("Stress Test", 9, "Verify an old deleted course 'OS404' is gone", find_course_by_code("OS404", buffer, n) == -1);
}

void run_scenario_6_edge_cases() {
    report_step("Edge Cases", 1, "Starting with 30 courses from stress test", n == 30);
    
    n = 0;
    save_courses(E2E_TEST_PATH, buffer, n);
    report_step("Edge Cases", 2, "Saved an empty list to the file", 1);
    
    n = load_courses(E2E_TEST_PATH, buffer);
    report_step("Edge Cases", 3, "Loaded the empty file correctly, count is 0", n == 0);
    
    n = add_course_logic((Course){"LAST", "Final Course", "End", 99}, buffer, n);
    save_courses(E2E_TEST_PATH, buffer, n);
    report_step("Edge Cases", 4, "Added single course to empty file and saved", 1);
    
    n = 0;
    memset(buffer, 0, sizeof(buffer));
    n = load_courses(E2E_TEST_PATH, buffer);
    report_step("Edge Cases", 5, "Loaded the single-item file correctly", n == 1);
    report_step("Edge Cases", 6, "Verify single item's data integrity", strcmp(buffer[0].code, "LAST") == 0);
    
    n = delete_course_by_index(0, buffer, n);
    save_courses(E2E_TEST_PATH, buffer, n);
    report_step("Edge Cases", 7, "Deleted the single item and saved", 1);
    
    n = 0;
    memset(buffer, 0, sizeof(buffer));
    n = load_courses(E2E_TEST_PATH, buffer);
    report_step("Edge Cases", 8, "Verified file is empty again after final deletion", n == 0);
}

void run_e2e_tests() {
    getCSVPath(E2E_TEST_PATH, sizeof(E2E_TEST_PATH));
    strcpy(strrchr(E2E_TEST_PATH, '\\') + 1, "e2e_hyper_density_workflow.csv");

    printHeader("HYPER-DENSITY END-TO-END WORKFLOW SIMULATION");
    
    run_scenario_1_initial_population();
    run_scenario_2_search_workflow();
    run_scenario_3_modification_cycle();
    run_scenario_4_final_verification();
    run_scenario_5_stress_test();
    run_scenario_6_edge_cases();

    printf("\n--- Teardown ---\n");
    remove(E2E_TEST_PATH);
    printf("    [OK] Step 1: Cleaned up temporary test file\n");
    
    setConsoleColor(C_SUCCESS);
    printf("\n--- All E2E Scenarios Completed Successfully ---\n\n");
}