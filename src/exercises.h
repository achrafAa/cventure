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
    }
};

#endif // EXERCISES_H 