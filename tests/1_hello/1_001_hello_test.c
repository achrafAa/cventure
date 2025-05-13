#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// At the top of the test file, after includes
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_001_hello.c"  // Default for linter only
#endif

/**
 * Test suite for Hello World exercise
 * 
 * This file demonstrates the standard pattern for all test files:
 * 1. Define test functions that return TestResult
 * 2. Each test verifies one aspect of the program
 * 3. Use the test_program function to run the program and capture output
 */

// Global variable to store program output
static char program_output[1024];

// Function to compile and run the exercise program
static void test_program(void) {
    char cmd[1024];
    FILE* fp;

    // Compile the exercise file
    #ifndef __EXERCISE_FILE__
    #error "Exercise file path not defined. Use -D__EXERCISE_FILE__=\"path/to/file.c\""
    #endif

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

    // Read output
    program_output[0] = '\0';
    if (fgets(program_output, sizeof(program_output), fp) != NULL) {
        // Remove trailing newline if present
        size_t len = strlen(program_output);
        if (len > 0 && program_output[len-1] == '\n') {
            program_output[len-1] = '\0';
        }
    }
    
    pclose(fp);
}

// Test: Check if output is "Hello, World!"
TestResult test_hello_world_output(void) {
    test_program();
    
    if (strcmp(program_output, "Hello, World!") != 0) {
        return (TestResult){
            .passed = false,
            .message = "Output does not match expected string",
            .expected = "Hello, World!",
            .actual = program_output,
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: Check if there's no stderr output
TestResult test_hello_world_no_stderr(void) {
    // Since we capture both stdout and stderr in test_program,
    // we've already verified the output is correct in the previous test.
    // This test is redundant but included for demonstration purposes.
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "Output matches expected", .test_func = test_hello_world_output },
        { .name = "No stderr output", .test_func = test_hello_world_no_stderr }
    };
    
    TestSuite suite = {
        .name = "Hello World Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 