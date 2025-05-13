#ifndef EXERCISES_H
#define EXERCISES_H

#include <stdbool.h>
#include <stddef.h>  // For NULL definition

typedef struct {
    const char* main_file;
    const char* output;
    const char* hint;
    bool check_stdout;
    const char* test_suite;  // Path to test suite file
    const char* test_name;   // Name of the test suite
} Exercise;

static Exercise exercises[] = {
    // 1. Hello World
    {
        "exercises/1_hello/1_001_hello.c",
        "Hello, World!",
        "Use printf() to print a string",
        true,
        "tests/1_hello/1_001_hello_test.c",
        "Hello World Test Suite"
    },
    {
        "exercises/1_hello/1_002_welcome_message.c",
        "Welcome to C programming!\nLet the adventure begin!",
        "Use multiple printf statements with newlines",
        true,
        "tests/1_hello/1_002_welcome_message_test.c",
        "Welcome Message Test Suite"
    },
    {
        "exercises/1_hello/1_003_welcome_message2.c",
        "Welcome to C programming!\nLet the adventure begin!",
        "Use a single printf statement with the newline character",
        true,
        "tests/1_hello/1_003_welcome_message2_test.c",
        "Welcome Message 2 Test Suite"
    }
};

#endif // EXERCISES_H 