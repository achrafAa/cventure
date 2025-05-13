#include "test_suite.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// ANSI color codes
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define BOLD "\x1b[1m"
#define RESET "\x1b[0m"
#define RED_BOLD "\x1b[31;1m"
#define GREEN_BOLD "\x1b[32;1m"

void run_test_suite(TestSuite* suite) {
    printf("\n%sRunning test suite: %s%s\n", BOLD, suite->name, RESET);
    printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", BLUE, RESET);

    suite->passed_count = 0;
    suite->failed_count = 0;

    for (size_t i = 0; i < suite->test_count; i++) {
        TestCase* test = &suite->test_cases[i];
        printf("\n%sTest: %s%s\n", YELLOW, test->name, RESET);

        TestResult result = test->test_func();

        if (result.passed) {
            printf("%s✅ PASSED%s\n", GREEN, RESET);
            suite->passed_count++;
        } else {
            printf("%s❌ FAILED%s\n", RED, RESET);
            printf("  Message: %s\n", result.message);
            if (result.expected && result.actual) {
                printf("  Expected: %s\n", result.expected);
                printf("  Actual: %s\n", result.actual);
            }
            printf("  Location: %s:%d\n", result.file_name, result.line_number);
            suite->failed_count++;
        }
    }
}

void print_test_results(TestSuite* suite) {
    printf("\n%sTest Results for %s%s\n", BOLD, suite->name, RESET);
    printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", BLUE, RESET);
    printf("Total tests: %zu\n", suite->test_count);
    printf("%sPassed: %zu%s\n", GREEN, suite->passed_count, RESET);
    printf("%sFailed: %zu%s\n", RED, suite->failed_count, RESET);
    
    if (suite->failed_count == 0) {
        printf("\n%s🎉 All tests passed!%s\n", GREEN_BOLD, RESET);
    } else {
        printf("\n%s⚠️ Some tests failed.%s\n", RED_BOLD, RESET);
    }
}

void capture_stdout(void (*func)(void), char* output, size_t max_size) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {  // Child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        func();
        exit(0);
    } else {  // Parent process
        close(pipefd[1]);
        
        ssize_t bytes_read = read(pipefd[0], output, max_size - 1);
        if (bytes_read > 0) {
            output[bytes_read] = '\0';
        } else {
            output[0] = '\0';
        }
        
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
    }
}

void capture_stderr(void (*func)(void), char* output, size_t max_size) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {  // Child process
        close(pipefd[0]);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);
        
        func();
        exit(0);
    } else {  // Parent process
        close(pipefd[1]);
        
        ssize_t bytes_read = read(pipefd[0], output, max_size - 1);
        if (bytes_read > 0) {
            output[bytes_read] = '\0';
        } else {
            output[0] = '\0';
        }
        
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
    }
} 