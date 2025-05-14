#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/2_variables/2_001_foggy_labels.c"
#endif

// Global variable to store program output
static char program_output[1024];

// Function to compile and run the exercise program
static void test_program(void) {
    char cmd[1024];
    FILE* fp;

    // Compile the exercise file
    snprintf(cmd, sizeof(cmd), "gcc -o /tmp/test_foggy %s", __EXERCISE_FILE__);
    if (system(cmd) != 0) {
        strcpy(program_output, "Compilation failed");
        return;
    }

    // Run the program and capture output
    fp = popen("/tmp/test_foggy 2>&1", "r");
    if (!fp) {
        strcpy(program_output, "Failed to run program");
        return;
    }

    // Read output
    program_output[0] = '\0';
    char line[256];
    size_t total_len = 0;
    
    while (fgets(line, sizeof(line), fp) != NULL && total_len < sizeof(program_output) - 1) {
        size_t line_len = strlen(line);
        if (total_len + line_len < sizeof(program_output) - 1) {
            strcat(program_output, line);
            total_len += line_len;
        }
    }
    
    pclose(fp);
}

// Test: check mana capacity output
TestResult test_mana_capacity(void) {
    test_program();
    
    if (strstr(program_output, "Mana Capacity: 50") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Mana capacity not correctly displayed",
            .expected = "Output should include 'Mana Capacity: 50'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: check mana regeneration output
TestResult test_mana_regeneration(void) {
    test_program();
    
    if (strstr(program_output, "Mana Regeneration: 73.2") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Mana regeneration not correctly displayed",
            .expected = "Output should include 'Mana Regeneration: 73.2'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: check rank symbol output
TestResult test_rank_symbol(void) {
    test_program();
    
    if (strstr(program_output, "Rank Symbol: R") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Rank symbol not correctly displayed",
            .expected = "Output should include 'Rank Symbol: R'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "Mana Capacity", .test_func = test_mana_capacity },
        { .name = "Mana Regeneration", .test_func = test_mana_regeneration },
        { .name = "Rank Symbol", .test_func = test_rank_symbol }
    };
    
    TestSuite suite = {
        .name = "Foggy Labels Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 