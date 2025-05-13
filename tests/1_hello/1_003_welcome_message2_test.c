#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_003_welcome_message2.c"
#endif

/**
 * Test suite for Welcome Message 2 exercise
 * 
 * Tests the student's ability to use a single printf statement
 * with newline characters for multi-line output.
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
    
    const char* expected = "Welcome to C programming!\nLet the adventure begin!";
    
    if (strcmp(program_output, expected) != 0) {
        return (TestResult){
            .passed = false,
            .message = "Output does not match expected message",
            .expected = expected,
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
    
    int newline_count = 0;
    for (size_t i = 0; program_output[i]; ++i) {
        if (program_output[i] == '\n') {
            newline_count++;
        }
    }
    
    if (newline_count != 1) {
        char expected[32];
        char actual[32];
        snprintf(expected, sizeof(expected), "2 lines (1 newline)");
        snprintf(actual, sizeof(actual), "%d lines (%d newlines)", newline_count, newline_count);
        
        return (TestResult){
            .passed = false,
            .message = "Incorrect number of lines in output",
            .expected = expected,
            .actual = actual,
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: Check that only one printf is used
TestResult test_single_printf(void) {
    char cmd[1024];
    char grep_output[1024] = {0};
    FILE* fp;

    // Count printf occurrences in the source file, excluding comments
    snprintf(cmd, sizeof(cmd), "grep -v \"//\" %s | grep -v \"\\*\" | grep -c \"printf\"", __EXERCISE_FILE__);
    fp = popen(cmd, "r");
    if (!fp) {
        return (TestResult){
            .passed = false,
            .message = "Failed to analyze source code",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }

    if (fgets(grep_output, sizeof(grep_output), fp) == NULL) {
        pclose(fp);
        return (TestResult){
            .passed = false,
            .message = "Failed to count printf calls",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    pclose(fp);
    
    // Convert output to integer
    int printf_count = atoi(grep_output);
    
    // Check if there's exactly one printf (excluding comments and examples)
    if (printf_count > 1) {
        char expected[32];
        char actual[32];
        snprintf(expected, sizeof(expected), "1 printf call");
        snprintf(actual, sizeof(actual), "%d printf calls", printf_count);
        
        return (TestResult){
            .passed = false,
            .message = "Too many printf statements used",
            .expected = expected,
            .actual = actual,
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "Welcome message format", .test_func = test_welcome_message },
        { .name = "Two lines of output", .test_func = test_two_lines },
        { .name = "Single printf used", .test_func = test_single_printf }
    };
    
    TestSuite suite = {
        .name = "Welcome Message 2 Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 