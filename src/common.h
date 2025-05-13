#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>

// ANSI color codes - only keeping used ones
#define RED "\x1b[31m"
#define RED_BOLD "\x1b[31;1m"
#define GREEN "\x1b[32m"
#define GREEN_BOLD "\x1b[32;1m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define BOLD "\x1b[1m"
#define RESET "\x1b[0m"

// Constants
#define MAX_PATH 256
#define MAX_OUTPUT 4096
#define MAX_EXERCISES 100
#define PROGRESS_FILENAME ".progress.txt"

// Exercise structure
typedef struct {
    const char* main_file;
    const char* output;
    const char* hint;
    int check_stdout;
} Exercise;

// Function declarations
void print_logo(void);
int validate_exercises(const Exercise* exercises, size_t count);
int run_exercise(const Exercise* ex, const char* work_path);
char* capture_output(const char* exe_path, int check_stdout);
int compile_exercise(const Exercise* ex, const char* work_path, char* exe_path);
void print_help(const Exercise* ex, const char* work_path);
int update_progress(int exercise_number);
int read_progress(int* current_progress);

#endif // COMMON_H 