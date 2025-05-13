#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "src/exercises.h"
#include "src/test_suite.h"

#define MAX_PATH 256
#define MAX_OUTPUT 4096
#define MAX_EXERCISES 100

// ANSI color codes
#define RED "\x1b[31m"
#define RED_BOLD "\x1b[31;1m"
#define RED_DIM "\x1b[31;2m"
#define GREEN "\x1b[32m"
#define GREEN_BOLD "\x1b[32;1m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define BOLD "\x1b[1m"
#define RESET "\x1b[0m"

// Global variables
static int use_color_escapes = 0;
static const char* progress_filename = ".progress.txt";

// Function declarations
void print_logo(void);
int validate_exercises(const Exercise* exercises, size_t count);
int run_exercise(const Exercise* ex, const char* work_path, const char* build_path);
char* capture_output(const char* exe_path, int check_stdout);
int compile_exercise(const Exercise* ex, const char* work_path, const char* build_path, char* exe_path);
void print_help(const Exercise* ex, const char* work_path);
int read_progress(int* current_progress);
int update_progress(int exercise_number);
int run_exercise_test_suite(const Exercise* ex, const char* work_path, const char* build_path);

int main(int argc, char** argv) {
    if (isatty(STDERR_FILENO)) {
        use_color_escapes = 1;
    }

    print_logo();

    if (!validate_exercises(exercises, sizeof(exercises) / sizeof(Exercise))) {
        fprintf(stderr, "Exercise validation failed\n");
        return 1;
    }

    int reset = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--reset") == 0) {
            reset = 1;
        }
    }

    if (reset) {
        unlink(progress_filename);
        printf("Progress reset, %s removed.\n", progress_filename);
        return 0;
    }

    const char* work_path = "exercises";
    const char* build_path = "build";
    int current_progress = 0;
    
    if (read_progress(&current_progress) != 0) {
        fprintf(stderr, "Failed to read progress file\n");
        return 1;
    }

    for (size_t i = current_progress; i < sizeof(exercises) / sizeof(Exercise); i++) {
        if (run_exercise(&exercises[i], work_path, build_path) != 0) {
            return 1;
        }
        if (update_progress(i + 1) != 0) {
            fprintf(stderr, "Failed to update progress\n");
            return 1;
        }
    }

    return 0;
}

void print_logo(void) {
    printf(
        "\n"
        "    %s\"Welcome to Cventure - Let's begin the C adventure!\"%s\n"
        "\n\n",
        BOLD, RESET
    );
}

int validate_exercises(const Exercise* exercises, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (!exercises[i].main_file || !exercises[i].output) {
            fprintf(stderr, "Exercise %zu has missing required fields\n", i + 1);
            return 0;
        }
    }
    return 1;
}

int run_exercise(const Exercise* ex, const char* work_path, const char* build_path) {
    printf("\n%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", BLUE, RESET);
    printf("%s📝 Exercise: %s%s\n", BOLD, ex->main_file, RESET);
    printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n\n", BLUE, RESET);

    char exe_path[MAX_PATH];
    if (compile_exercise(ex, work_path, build_path, exe_path) != 0) {
        print_help(ex, work_path);
        return 1;
    }

    // Run basic output test
    char* output = capture_output(exe_path, ex->check_stdout);
    if (!output) {
        print_help(ex, work_path);
        return 1;
    }

    // Normalize line endings and trim whitespace
    char* normalized_output = malloc(MAX_OUTPUT);
    char* normalized_expected = malloc(MAX_OUTPUT);
    if (!normalized_output || !normalized_expected) {
        free(output);
        free(normalized_output);
        free(normalized_expected);
        return 1;
    }

    // Copy and normalize output
    const char* src = output;
    char* dst = normalized_output;
    while (*src) {
        if (*src == '\r' && *(src + 1) == '\n') {
            *dst++ = '\n';
            src += 2;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';

    // Copy and normalize expected output
    src = ex->output;
    dst = normalized_expected;
    while (*src) {
        if (*src == '\r' && *(src + 1) == '\n') {
            *dst++ = '\n';
            src += 2;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';

    // Trim trailing whitespace
    char* end = normalized_output + strlen(normalized_output) - 1;
    while (end > normalized_output && (*end == ' ' || *end == '\n' || *end == '\r' || *end == '\t')) {
        *end = '\0';
        end--;
    }

    end = normalized_expected + strlen(normalized_expected) - 1;
    while (end > normalized_expected && (*end == ' ' || *end == '\n' || *end == '\r' || *end == '\t')) {
        *end = '\0';
        end--;
    }

    if (strcmp(normalized_output, normalized_expected) != 0) {
        printf("\n%s❌ Test Failed%s\n", RED_BOLD, RESET);
        printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", RED, RESET);
        printf("%sExpected:%s\n%s\n", YELLOW, RESET, normalized_expected);
        printf("%sGot:%s\n%s\n", YELLOW, RESET, normalized_output);
        printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n\n", RED, RESET);
        print_help(ex, work_path);
        free(output);
        free(normalized_output);
        free(normalized_expected);
        return 1;
    }

    printf("%s✅ Basic Test Passed!%s\n", GREEN_BOLD, RESET);
    printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", GREEN, RESET);
    printf("%s%s%s\n", GREEN, normalized_output, RESET);
    printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n\n", GREEN, RESET);
    
    free(output);
    free(normalized_output);
    free(normalized_expected);

    // Run test suite if available
    if (ex->test_suite) {
        if (run_exercise_test_suite(ex, work_path, build_path) != 0) {
            return 1;
        }
    }

    return 0;
}

char* capture_output(const char* exe_path, int check_stdout) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return NULL;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return NULL;
    }

    if (pid == 0) {
        // Child process
        close(pipefd[0]);
        if (check_stdout) {
            dup2(pipefd[1], STDOUT_FILENO);
        } else {
            dup2(pipefd[1], STDERR_FILENO);
        }
        close(pipefd[1]);
        execl(exe_path, exe_path, NULL);
        exit(1);
    }

    // Parent process
    close(pipefd[1]);
    char* output = malloc(MAX_OUTPUT);
    if (!output) {
        close(pipefd[0]);
        return NULL;
    }

    ssize_t bytes_read = read(pipefd[0], output, MAX_OUTPUT - 1);
    close(pipefd[0]);
    
    if (bytes_read == -1) {
        perror("read");
        free(output);
        return NULL;
    }
    
    output[bytes_read] = '\0';
    return output;
}

int compile_exercise(const Exercise* ex, const char* work_path, const char* build_path, char* exe_path) {
    (void)work_path;  // Silence unused parameter warning
    
    // Create build directory if it doesn't exist
    mkdir(build_path, 0755);
    
    // Create a unique executable name in the build directory
    const char* base_name = strrchr(ex->main_file, '/');
    if (base_name) {
        base_name++; // Skip the '/'
    } else {
        base_name = ex->main_file;
    }
    
    // Create the executable name without modifying the original string
    char exe_name[MAX_PATH];
    strncpy(exe_name, base_name, MAX_PATH - 1);
    exe_name[MAX_PATH - 1] = '\0';
    
    // Replace .c with .exe
    char* dot = strrchr(exe_name, '.');
    if (dot) {
        *dot = '\0';
    }
    
    snprintf(exe_path, MAX_PATH, "%s/%s.exe", build_path, exe_name);
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child process
        execlp("gcc", "gcc", "-Wall", "-Wextra", "-Werror", ex->main_file, "-o", exe_path, NULL);
        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
}

void print_help(const Exercise* ex, const char* work_path) {
    (void)work_path;
    printf("\n%s💡 Hint: %s%s\n", YELLOW, ex->hint, RESET);
    printf("%s📁 File: %s%s\n", BLUE, ex->main_file, RESET);
}

int read_progress(int* current_progress) {
    FILE* progress_file = fopen(progress_filename, "r");
    if (!progress_file) {
        *current_progress = 0;
        return 0;
    }

    if (flock(fileno(progress_file), LOCK_EX) == -1) {
        fclose(progress_file);
        return 1;
    }

    if (fscanf(progress_file, "%d", current_progress) != 1) {
        *current_progress = 0;
    }

    flock(fileno(progress_file), LOCK_UN);
    fclose(progress_file);
    return 0;
}

int update_progress(int exercise_number) {
    FILE* progress_file = fopen(progress_filename, "w");
    if (!progress_file) {
        return 1;
    }

    if (flock(fileno(progress_file), LOCK_EX) == -1) {
        fclose(progress_file);
        return 1;
    }

    fprintf(progress_file, "%d", exercise_number);
    
    flock(fileno(progress_file), LOCK_UN);
    fclose(progress_file);
    return 0;
}

int run_exercise_test_suite(const Exercise* ex, const char* work_path, const char* build_path) {
    (void)work_path;  // Silence unused parameter warning
    if (!ex->test_suite) {
        return 0;  // No test suite for this exercise
    }

    printf("\n%sRunning test suite: %s%s\n", BOLD, ex->test_name, RESET);
    printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", BLUE, RESET);

    // Compile and run the test suite as a standalone executable
    char test_exe_path[MAX_PATH];
    snprintf(test_exe_path, MAX_PATH, "%s/test_%zu", build_path, ex - exercises);
    
    char cmd[MAX_PATH * 4];
    // Use -D__EXERCISE_FILE__ to pass the exercise file path to the test suite
    snprintf(cmd, sizeof(cmd), "gcc -Wall -o %s %s -D__EXERCISE_FILE__=\\\"%s\\\" src/test_suite.c",
             test_exe_path, ex->test_suite, ex->main_file);
    
    if (system(cmd) != 0) {
        printf("%s❌ Failed to compile test suite%s\n", RED_BOLD, RESET);
        return 1;
    }

    // Run the test suite
    if (system(test_exe_path) != 0) {
        printf("%s❌ Test suite failed%s\n", RED_BOLD, RESET);
        return 1;
    }

    printf("%s✅ Test suite passed%s\n", GREEN_BOLD, RESET);
    return 0;
} 