#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_010_spell_shortcuts.c"
#endif

/**
 * Test suite for the Spell Shortcuts exercise
 * Tests the student's ability to use compound assignment operators.
 */

// Global variable to store program output
static char program_output[1024];

// Function to compile and run the exercise program
static void test_program(void) {
    char cmd[1024];
    FILE* fp;

    // Compile the exercise file
    snprintf(cmd, sizeof(cmd), "gcc -o /tmp/test_shortcuts %s", __EXERCISE_FILE__);
    if (system(cmd) != 0) {
        strcpy(program_output, "Compilation failed");
        return;
    }

    // Run the program and capture output
    fp = popen("/tmp/test_shortcuts 2>&1", "r");
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

// Test: check initial value
TestResult test_initial_value(void) {
    test_program();
    
    if (strstr(program_output, "Initial mana pool: 100") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Initial value not correctly set or displayed",
            .expected = "Output should include 'Initial mana pool: 100'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: addition with compound operator
TestResult test_addition(void) {
    test_program();
    
    if (strstr(program_output, "After adding 50: 150") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Addition with compound operator not correctly implemented",
            .expected = "Output should include 'After adding 50: 150'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: subtraction with compound operator
TestResult test_subtraction(void) {
    test_program();
    
    if (strstr(program_output, "After subtracting 25: 125") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Subtraction with compound operator not correctly implemented",
            .expected = "Output should include 'After subtracting 25: 125'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: multiplication with compound operator
TestResult test_multiplication(void) {
    test_program();
    
    if (strstr(program_output, "After multiplying by 2: 250") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Multiplication with compound operator not correctly implemented",
            .expected = "Output should include 'After multiplying by 2: 250'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Test: division with compound operator
TestResult test_division(void) {
    test_program();
    
    if (strstr(program_output, "After dividing by 5: 50") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Division with compound operator not correctly implemented",
            .expected = "Output should include 'After dividing by 5: 50'",
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
        { .name = "Addition (+=)", .test_func = test_addition },
        { .name = "Subtraction (-=)", .test_func = test_subtraction },
        { .name = "Multiplication (*=)", .test_func = test_multiplication },
        { .name = "Division (/=)", .test_func = test_division }
    };
    
    TestSuite suite = {
        .name = "Spell Shortcuts Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 