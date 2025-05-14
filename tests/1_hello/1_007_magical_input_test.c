#include "../../src/test_suite.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Default for linter only
#ifndef __EXERCISE_FILE__
#define __EXERCISE_FILE__ "exercises/1_hello/1_007_magical_input.c"
#endif

/**
 * Test suite for the Magical Input exercise
 * Tests the student's ability to get user input and print it.
 */

// Global variable to store program output
static char program_output[1024];
static char source_code[4096];

// Function to read the source code
static bool read_source_code(void) {
    FILE* source_file = fopen(__EXERCISE_FILE__, "r");
    if (!source_file) {
        return false;
    }
    
    size_t bytes_read = fread(source_code, 1, sizeof(source_code) - 1, source_file);
    fclose(source_file);
    
    if (bytes_read == 0) {
        return false;
    }
    
    source_code[bytes_read] = '\0';
    return true;
}

// Function to check if a line is commented out
static bool is_line_commented(const char* line) {
    // Skip whitespace
    while (*line && (*line == ' ' || *line == '\t')) {
        line++;
    }
    
    // Check if the line starts with a comment
    return (*line == '/' && *(line + 1) == '/');
}

// Function to compile and run the exercise program with input
static void run_program_with_input(const char* input) {
    char cmd[1024];
    FILE* fp;

    // Compile the exercise file
    snprintf(cmd, sizeof(cmd), "gcc -o /tmp/test_input %s", __EXERCISE_FILE__);
    if (system(cmd) != 0) {
        strcpy(program_output, "Compilation failed");
        return;
    }

    // Create a command that pipes the input to the program
    snprintf(cmd, sizeof(cmd), "printf \"%s\\n\" | /tmp/test_input 2>&1", input);
    
    // Run the program and capture output
    fp = popen(cmd, "r");
    if (!fp) {
        strcpy(program_output, "Failed to run program");
        return;
    }

    // Read output
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

// Test: check source code requirements
TestResult test_source_code_requirements(void) {
    if (!read_source_code()) {
        return (TestResult){
            .passed = false,
            .message = "Failed to read source code",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }

    // Check for variable declaration
    if (strstr(source_code, "int spellNumber") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Missing or incorrect variable declaration",
            .expected = "The program should declare 'int spellNumber'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }

    // Check for scanf
    char* scanf_line = strstr(source_code, "scanf");
    if (scanf_line == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Missing scanf statement",
            .expected = "The program should use scanf to read input",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }

    // Check if scanf is commented out
    if (is_line_commented(scanf_line)) {
        return (TestResult){
            .passed = false,
            .message = "scanf statement is commented out",
            .expected = "The program should have an uncommented scanf statement",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }

    // Check for correct scanf format
    if (strstr(scanf_line, "scanf(\"%d\", &") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Incorrect scanf format",
            .expected = "The program should use scanf(\"%d\", &variableName)",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }

    return (TestResult){ .passed = true };
}

// Test: check runtime behavior
TestResult test_runtime_behavior(void) {
    // Run the program with input "42"
    run_program_with_input("42");
    
    // Check if the program prompts for input
    if (strstr(program_output, "Enter spell number:") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Program does not prompt for input",
            .expected = "Output should include 'Enter spell number:'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    // Check if the program correctly displays the input
    if (strstr(program_output, "You selected spell #42") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Program does not correctly display the input",
            .expected = "Output should include 'You selected spell #42'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    // Run the program with a different input "123"
    run_program_with_input("123");
    
    // Check if the program correctly displays the different input
    if (strstr(program_output, "You selected spell #123") == NULL) {
        return (TestResult){
            .passed = false,
            .message = "Program does not correctly handle different input values",
            .expected = "Output should include 'You selected spell #123'",
            .line_number = __LINE__,
            .file_name = __FILE__
        };
    }
    
    return (TestResult){ .passed = true };
}

// Entry point
int main(void) {
    TestCase cases[] = {
        { .name = "Source code requirements", .test_func = test_source_code_requirements },
        { .name = "Runtime behavior", .test_func = test_runtime_behavior }
    };
    
    TestSuite suite = {
        .name = "Magical Input Test Suite",
        .test_cases = cases,
        .test_count = sizeof(cases) / sizeof(TestCase)
    };
    
    run_test_suite(&suite);
    print_test_results(&suite);
    
    return suite.failed_count > 0 ? 1 : 0;
} 