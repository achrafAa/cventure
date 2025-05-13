#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Test result structure
typedef struct {
    bool passed;
    const char* message;
    const char* expected;
    const char* actual;
    int line_number;
    const char* file_name;
} TestResult;

// Test case structure
typedef struct {
    const char* name;
    TestResult (*test_func)(void);
    bool should_fail;
} TestCase;

// Test suite structure
typedef struct {
    const char* name;
    TestCase* test_cases;
    size_t test_count;
    size_t passed_count;
    size_t failed_count;
} TestSuite;

// Assertion macros
#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            TestResult result = { \
                .passed = false, \
                .message = "Assertion failed: " #condition, \
                .line_number = __LINE__, \
                .file_name = __FILE__ \
            }; \
            return result; \
        } \
    } while(0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            TestResult result = { \
                .passed = false, \
                .message = "Assertion failed: " #condition, \
                .line_number = __LINE__, \
                .file_name = __FILE__ \
            }; \
            return result; \
        } \
    } while(0)

#define ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            TestResult result = { \
                .passed = false, \
                .message = "Values are not equal", \
                .expected = #expected, \
                .actual = #actual, \
                .line_number = __LINE__, \
                .file_name = __FILE__ \
            }; \
            return result; \
        } \
    } while(0)

#define ASSERT_STRING_EQUAL(expected, actual) \
    do { \
        if (strcmp(expected, actual) != 0) { \
            TestResult result = { \
                .passed = false, \
                .message = "Strings are not equal", \
                .expected = expected, \
                .actual = actual, \
                .line_number = __LINE__, \
                .file_name = __FILE__ \
            }; \
            return result; \
        } \
    } while(0)

#define ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != NULL) { \
            TestResult result = { \
                .passed = false, \
                .message = "Pointer is not NULL", \
                .line_number = __LINE__, \
                .file_name = __FILE__ \
            }; \
            return result; \
        } \
    } while(0)

#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            TestResult result = { \
                .passed = false, \
                .message = "Pointer is NULL", \
                .line_number = __LINE__, \
                .file_name = __FILE__ \
            }; \
            return result; \
        } \
    } while(0)

// Test suite functions
void run_test_suite(TestSuite* suite);
void print_test_results(TestSuite* suite);

// Helper functions
void capture_stdout(void (*func)(void), char* output, size_t max_size);
void capture_stderr(void (*func)(void), char* output, size_t max_size);

#endif // TEST_SUITE_H 