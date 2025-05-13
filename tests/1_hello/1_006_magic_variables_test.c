#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_006_magic_variables.c"
#endif

/**
 * Test suite for Magic Variables exercise
 * Tests the student's ability to declare and initialize variables of different types.
 */

// Global variable to store program output
static char program_output[1024];

// Function to compile and run the exercise program
static void test_program(void) {
    char cmd[1024];
    FILE* fp;

    // Compile the exercise file
    snprintf(cmd, sizeof(cmd), "gcc -o /tmp/test_magic_vars %s", __EXERCISE_FILE__);
    if (system(cmd) != 0) {
        strcpy(program_output, "Compilation failed");
        return;
    }

    // Run the program and capture output
    fp = popen("/tmp/test_magic_vars 2>&1", "r");
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

// Test: integer variable declaration and initialization
TestResult test_wizard_level(void) {
    test_program();
    
    if (strstr(program_output, "Wizard level: 42") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "wizard_level variable not correctly declared or initialized",
            .expected = "Output containing 'Wizard level: 42'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: float variable declaration and initialization
TestResult test_spell_power(void) {
    test_program();
    
    if (strstr(program_output, "Spell power: 9.75") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "spell_power variable not correctly declared or initialized",
            .expected = "Output containing 'Spell power: 9.75'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Test: char variable declaration and initialization
TestResult test_magic_symbol(void) {
    test_program();
    
    if (strstr(program_output, "Magic symbol: *") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "magic_symbol variable not correctly declared or initialized",
            .expected = "Output containing 'Magic symbol: *'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "Integer variable", .test_func = test_wizard_level },
        { .name = "Float variable", .test_func = test_spell_power },
        { .name = "Character variable", .test_func = test_magic_symbol }
    };
    
    TestSuite suite = {
        .name = "Magic Variables Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 