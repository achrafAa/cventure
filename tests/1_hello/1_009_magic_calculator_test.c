#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_009_magic_calculator.c"
#endif

/**
 * Test suite for the Magic Calculator exercise
 * Tests the student's ability to perform basic arithmetic operations.
 */

// Global variable to store program output
static char program_output[1024];

// Function to compile and run the exercise program
static void test_program(void) {
    char cmd[1024];
    FILE* fp;

    // Compile the exercise file
    snprintf(cmd, sizeof(cmd), "gcc -o /tmp/test_calculator %s", __EXERCISE_FILE__);
    if (system(cmd) != 0) {
        strcpy(program_output, "Compilation failed");
        return;
    }

    // Make sure the executable has the right permissions
    chmod("/tmp/test_calculator", 0755);

    // Run the program and capture output
    fp = popen("/tmp/test_calculator 2>&1", "r");
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

    // Debug: Print the captured output
    printf("Debug - Captured output:\n%s\n", program_output);
}

// Test: check complete output
TestResult test_complete_output(void) {
    test_program();
    
    const char* expected_output = 
        "Magic Calculator Results:\n"
        "7 + 3 = 10\n"
        "7 - 3 = 4\n"
        "7 * 3 = 21\n"
        "7 / 3 = 2\n"
        "7 % 3 = 1\n";
    
    // Debug: Print lengths for comparison
    printf("Debug - Expected length: %zu, Actual length: %zu\n", 
           strlen(expected_output), strlen(program_output));
    
    if (strcmp(program_output, expected_output) != 0) {
        return (TestResult){
            .passed = false,
            .message = "Output does not match expected format",
            .expected = expected_output,
            .actual = program_output,
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "Complete output check", .test_func = test_complete_output }
    };
    
    TestSuite suite = {
        .name = "Magic Calculator Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 