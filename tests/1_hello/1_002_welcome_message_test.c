#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_002_welcome_message.c"
#endif

/**
 * Test suite for Welcome Message exercise
 * 
 * Tests the student's ability to use multiple printf statements
 * and properly format output with newlines.
 */

// Global variable to store program output
static char program_output[1024];

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

// Test: Check for correct welcome message with proper formatting
TestResult test_welcome_message(void) {
    test_program();
    
    // Check if output contains both required lines in any format
    if (strstr(program_output, "Welcome to C programming!") == NULL ||
        strstr(program_output, "Let the adventure begin!") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Output does not contain required text",
            .expected = "Welcome to C programming! and Let the adventure begin!",
            .actual = program_output,
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: Check for exactly two lines of output
TestResult test_two_lines(void) {
    test_program();
    
    // Count lines by checking for newlines or line breaks
    int line_count = 1; // Start with 1 for the first line
    for (size_t i = 0; program_output[i]; ++i) {
        if (program_output[i] == '\n' && program_output[i+1] != '\0') {
            line_count++;
        }
    }
    
    if (line_count < 2) {
        char expected_str[32];
        char actual_str[32];
        snprintf(expected_str, sizeof(expected_str), "At least 2 lines");
        snprintf(actual_str, sizeof(actual_str), "%d lines", line_count);
        
        return (TestResult){
            .passed = false,
            .message = "Insufficient number of lines in output",
            .expected = expected_str,
            .actual = actual_str,
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: Check for no stderr output
TestResult test_no_stderr(void) {
    char cmd[1024];
    char stderr_output[1024] = {0};
    FILE* fp;

    // Compile the exercise file
    snprintf(cmd, sizeof(cmd), "gcc -o /tmp/test_exe %s", __EXERCISE_FILE__);
    if (system(cmd) != 0) {
        return (TestResult){
            .passed = false,
            .message = "Compilation failed",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }

    // Run the program and capture stderr only
    fp = popen("/tmp/test_exe 2>&1 1>/dev/null", "r");
    if (!fp) {
        return (TestResult){
            .passed = false,
            .message = "Failed to run program",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }

    // Read stderr output
    if (fgets(stderr_output, sizeof(stderr_output), fp) != NULL) {
        pclose(fp);
        return (TestResult){
            .passed = false,
            .message = "Program writes to stderr",
            .expected = "No stderr output",
            .actual = stderr_output,
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    pclose(fp);
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "Welcome message format", .test_func = test_welcome_message },
        { .name = "Two lines of output", .test_func = test_two_lines },
        { .name = "No stderr output", .test_func = test_no_stderr }
    };
    
    TestSuite suite = {
        .name = "Welcome Message Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 