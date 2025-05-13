#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_004_escape_runes.c"
#endif

/**
 * Test suite for Fix the Escape Runes exercise
 * 
 * Tests the student's ability to properly use escape sequences in printf statements.
 */

// Global variable to store program output
static char program_output[2048];

// Function to compile and run the exercise program
static void test_program(void) {
    char cmd[1024];
    FILE* fp;

    // Compile the exercise file
    snprintf(cmd, sizeof(cmd), "gcc -o /tmp/test_exe %s", __EXERCISE_FILE__);
    if (system(cmd) != 0) {
        strcpy(program_output, "Compilation failed");
        return;
    }

    // Run the program and capture output
    fp = popen("/tmp/test_exe 2>&1", "r");
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

// Test: Check for correct output for each printf statement
TestResult test_all_outputs(void) {
    test_program();
    
    if (strstr(program_output, "Compilation failed") != NULL) {
        return (TestResult){
            .passed = false,
            .message = "Your code doesn't compile. Fix the syntax errors in the printf statements.",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    // Check for quoted text
    if (strstr(program_output, "This is a \"quoted\" text") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "First printf statement doesn't output the quoted text correctly",
            .expected = "This is a \"quoted\" text",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    // Check for backslashes in file path
    if (strstr(program_output, "C:\\Program Files\\App") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Second printf statement doesn't output the file path with backslashes correctly",
            .expected = "The file is located at C:\\Program Files\\App",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    // Check for percent sign
    if (strstr(program_output, "10% of the work is done") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Fourth printf statement doesn't output the percentage correctly",
            .expected = "10% of the work is done",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "All escape sequences are fixed", .test_func = test_all_outputs }
    };
    
    TestSuite suite = {
        .name = "Fix Escape Runes Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 