#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_011_potion_counter.c"
#endif

/**
 * Test suite for the Potion Counter exercise
 * Tests the student's ability to use increment and decrement operators.
 */

// Global variable to store program output
static char program_output[1024];

// Function to compile and run the exercise program
static void test_program(void) {
    char cmd[1024];
    FILE* fp;

    // Compile the exercise file
    snprintf(cmd, sizeof(cmd), "gcc -o /tmp/test_potion %s", __EXERCISE_FILE__);
    if (system(cmd) != 0) {
        strcpy(program_output, "Compilation failed");
        return;
    }

    // Run the program and capture output
    fp = popen("/tmp/test_potion 2>&1", "r");
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

// Test: initial value
TestResult test_initial_value(void) {
    test_program();
    
    if (strstr(program_output, "Initial potion count: 5") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Initial value not correctly set or displayed",
            .expected = "Output should include 'Initial potion count: 5'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: post-increment
TestResult test_post_increment(void) {
    test_program();
    
    if (strstr(program_output, "Post-increment: 5 (now 6)") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Post-increment not correctly implemented",
            .expected = "Output should include 'Post-increment: 5 (now 6)'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: pre-increment
TestResult test_pre_increment(void) {
    test_program();
    
    if (strstr(program_output, "Pre-increment: 7") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Pre-increment not correctly implemented",
            .expected = "Output should include 'Pre-increment: 7'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: post-decrement
TestResult test_post_decrement(void) {
    test_program();
    
    if (strstr(program_output, "Post-decrement: 7 (now 6)") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Post-decrement not correctly implemented",
            .expected = "Output should include 'Post-decrement: 7 (now 6)'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: pre-decrement
TestResult test_pre_decrement(void) {
    test_program();
    
    if (strstr(program_output, "Pre-decrement: 5") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Pre-decrement not correctly implemented",
            .expected = "Output should include 'Pre-decrement: 5'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "Initial value", .test_func = test_initial_value },
        { .name = "Post-increment (x++)", .test_func = test_post_increment },
        { .name = "Pre-increment (++x)", .test_func = test_pre_increment },
        { .name = "Post-decrement (x--)", .test_func = test_post_decrement },
        { .name = "Pre-decrement (--x)", .test_func = test_pre_decrement }
    };
    
    TestSuite suite = {
        .name = "Potion Counter Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 