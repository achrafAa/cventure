#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_005_potion_recipe.c"
#endif

/**
 * Test suite for Potion Recipe exercise
 * 
 * Tests the student's ability to use format specifiers correctly.
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

// Test: Check for correct string format specifier
TestResult test_string_format(void) {
    test_program();
    
    if (strstr(program_output, "Name: Elixir of Wisdom") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "String format specifier not used correctly",
            .expected = "Name: Elixir of Wisdom",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: Check for correct character format specifier
TestResult test_char_format(void) {
    test_program();
    
    if (strstr(program_output, "Grade: A") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Character format specifier not used correctly",
            .expected = "Grade: A",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: Check for correct integer format specifier
TestResult test_integer_format(void) {
    test_program();
    
    if (strstr(program_output, "Dragon Scales: 3 pieces") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Integer format specifier not used correctly",
            .expected = "Dragon Scales: 3 pieces",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: Check for correct float format specifier with precision
TestResult test_float_format(void) {
    test_program();
    
    if (strstr(program_output, "Fairy Dust: 1.50 ounces") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Float format specifier with precision not used correctly",
            .expected = "Fairy Dust: 1.50 ounces",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: Check for correct float format specifier with precision for magic power
TestResult test_magic_power_format(void) {
    test_program();
    
    if (strstr(program_output, "Power Level: 42.42") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Float format specifier with precision not used correctly for magic power",
            .expected = "Power Level: 42.42",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "String format specifier", .test_func = test_string_format },
        { .name = "Character format specifier", .test_func = test_char_format },
        { .name = "Integer format specifier", .test_func = test_integer_format },
        { .name = "Float format specifier with precision", .test_func = test_float_format },
        { .name = "Magic power format specifier", .test_func = test_magic_power_format }
    };
    
    TestSuite suite = {
        .name = "Potion Recipe Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 