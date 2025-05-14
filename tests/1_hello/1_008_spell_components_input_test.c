#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_008_spell_components_input.c"
#endif

/**
 * Test suite for the Spell Components Input exercise
 * Tests the student's ability to handle multiple input types.
 */

// Global variable to store program output
static char program_output[1024];

// Function to compile and run the exercise program
static void test_program_with_input(const char* input) {
    char cmd[1024];
    FILE* fp;

    // Compile the exercise file
    snprintf(cmd, sizeof(cmd), "gcc -o /tmp/test_components %s", __EXERCISE_FILE__);
    if (system(cmd) != 0) {
        strcpy(program_output, "Compilation failed");
        return;
    }

    // Create a command that pipes the input to the program using printf
    snprintf(cmd, sizeof(cmd), "printf \"%s\" | /tmp/test_components 2>&1", input);
    
    // Run the program and capture output
    fp = popen(cmd, "r");
    if (!fp) {
        strcpy(program_output, "Failed to run program");
        return;
    }

    // Read output (multiple lines)
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

// Test: standard input
TestResult test_standard_input(void) {
    test_program_with_input("75\n2.5\nA\n");
    
    if (strstr(program_output, "Enter spell power:") == NULL ||
        strstr(program_output, "Enter ingredient weight:") == NULL ||
        strstr(program_output, "Enter magical grade:") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Program does not prompt for all the required inputs",
            .expected = "Prompts for spell power, ingredient weight, and magical grade",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    if (strstr(program_output, "- Power: 75") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Program does not correctly handle the integer input",
            .expected = "Output should include '- Power: 75'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    if (strstr(program_output, "- Weight: 2.50 oz") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Program does not correctly handle the float input",
            .expected = "Output should include '- Weight: 2.50 oz'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    if (strstr(program_output, "- Grade: A") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Program does not correctly handle the character input",
            .expected = "Output should include '- Grade: A'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: different values
TestResult test_different_values(void) {
    test_program_with_input("100\n3.75\nB\n");
    
    if (strstr(program_output, "- Power: 100") == NULL ||
        strstr(program_output, "- Weight: 3.75 oz") == NULL ||
        strstr(program_output, "- Grade: B") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Program does not correctly handle different input values",
            .expected = "Output should adapt to different input values",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "Standard input values", .test_func = test_standard_input },
        { .name = "Different input values", .test_func = test_different_values }
    };
    
    TestSuite suite = {
        .name = "Spell Components Input Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 