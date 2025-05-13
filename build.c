#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <errno.h>
#include "src/exercises.h"
#include "src/test_suite.h"

#define MAX_PATH 256
#define MAX_OUTPUT 4096
#define MAX_EXERCISES 100
#define MAX_CHAPTERS 20
#define MAX_CONFIG_SIZE 1024
#define MAX_HINT_SIZE 2048

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
static const char* attempts_filename = ".attempts.txt";
static const char* config_filename = ".config.json";
static const char* hints_dir = "hints";
static int hard_mode = 0;  // No hints in hard mode

// Function declarations
int validate_exercises(const Exercise* exercises, size_t count);
int run_exercise(const Exercise* ex, const char* work_path, const char* build_path);
char* capture_output(const char* exe_path, int check_stdout);
int compile_exercise(const Exercise* ex, const char* work_path, const char* build_path, char* exe_path);
void print_help(const Exercise* ex, const char* work_path, int show_hint);
int read_progress(int chapter_progress[MAX_CHAPTERS]);
int update_progress(int chapter, int exercise_number);
int run_exercise_test_suite(const Exercise* ex, const char* work_path, const char* build_path);
int get_chapter_from_path(const char* path);
int get_exercise_attempts(const char* exercise_path);
void increment_exercise_attempts(const char* exercise_path);
int read_config(void);
int update_config(int new_hard_mode);
char* get_hint_for_exercise(const char* exercise_path);

int main(int argc, char** argv) {
    if (isatty(STDERR_FILENO)) {
        use_color_escapes = 1;
    }

    if (!validate_exercises(exercises, sizeof(exercises) / sizeof(Exercise))) {
        fprintf(stderr, "Exercise validation failed\n");
        return 1;
    }

    int reset = 0;
    int run_specific_chapter = -1;
    int save_hard_mode = 0;

    // Read configuration file
    read_config();

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--reset") == 0) {
            reset = 1;
        } else if (strncmp(argv[i], "--chapter=", 10) == 0) {
            run_specific_chapter = atoi(argv[i] + 10);
        } else if (strcmp(argv[i], "--hard") == 0) {
            hard_mode = 1;
            save_hard_mode = 1;
        } else if (strcmp(argv[i], "--normal") == 0) {
            hard_mode = 0;
            save_hard_mode = 1;
        }
    }

    // Update configuration if requested
    if (save_hard_mode) {
        update_config(hard_mode);
    }

    if (reset) {
        unlink(progress_filename);
        unlink(attempts_filename);
        printf("Progress reset, %s and %s removed.\n", progress_filename, attempts_filename);
        return 0;
    }

    const char* work_path = "exercises";
    const char* build_path = "build";
    int chapter_progress[MAX_CHAPTERS] = {0};
    
    if (read_progress(chapter_progress) != 0) {
        fprintf(stderr, "Failed to read progress file\n");
        return 1;
    }

    int all_completed = 1;
    
    // Process exercises by chapter
    for (size_t i = 0; i < sizeof(exercises) / sizeof(Exercise); i++) {
        int chapter = get_chapter_from_path(exercises[i].main_file);
        if (chapter < 0 || chapter >= MAX_CHAPTERS) {
            fprintf(stderr, "Invalid chapter number in exercise path: %s\n", exercises[i].main_file);
            continue;
        }
        
        // Skip if we're running a specific chapter and this isn't it
        if (run_specific_chapter != -1 && chapter != run_specific_chapter) {
            continue;
        }
        
        // Get the exercise number within its chapter
        int exercise_num = 0;
        const char* last_slash = strrchr(exercises[i].main_file, '/');
        if (last_slash) {
            sscanf(last_slash + 1, "%*d_%d", &exercise_num);
        }
        
        // Skip if already completed
        if (exercise_num <= chapter_progress[chapter] && run_specific_chapter == -1) {
            continue;
        }
        
        all_completed = 0;
        
        if (run_exercise(&exercises[i], work_path, build_path) != 0) {
            return 1;
        }
        
        if (update_progress(chapter, exercise_num) != 0) {
            fprintf(stderr, "Failed to update progress\n");
            return 1;
        }
    }

    // Display completion message when all exercises are finished
    if (all_completed && run_specific_chapter == -1) {
        printf("\n%s🎉 Congratulations, Code Wizard! 🎉%s\n", GREEN_BOLD, RESET);
        printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", GREEN, RESET);
        printf("\nYou've completed all available exercises in this magical journey!\n\n");
        printf("Your mastery of C programming spells is growing stronger. \n");
        printf("New challenges will appear in future updates, so keep your wand at the ready.\n\n");
        printf("To practice again, use %s--reset%s to start from the beginning.\n", YELLOW, RESET);
        printf("To focus on a specific chapter, use %s--chapter=N%s (e.g., --chapter=1).\n", YELLOW, RESET);
        printf("For an extra challenge, try %s--hard%s mode with no hints.\n", YELLOW, RESET);
        printf("\n%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", GREEN, RESET);
    }

    return 0;
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
        // Get current attempt count before incrementing
        int attempts = get_exercise_attempts(ex->main_file);
        
        // Increment attempt counter
        increment_exercise_attempts(ex->main_file);
        
        // Show hint if this is at least the second attempt or we're not in hard mode
        int show_hint = (!hard_mode && attempts >= 1);
        print_help(ex, work_path, show_hint);
        return 1;
    }

    // Run basic output test
    char* output = capture_output(exe_path, ex->check_stdout);
    if (!output) {
        // Get current attempt count before incrementing
        int attempts = get_exercise_attempts(ex->main_file);
        
        // Increment attempt counter
        increment_exercise_attempts(ex->main_file);
        
        // Show hint if this is at least the second attempt or we're not in hard mode
        int show_hint = (!hard_mode && attempts >= 1);
        print_help(ex, work_path, show_hint);
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
        
        // Get current attempt count before incrementing
        int attempts = get_exercise_attempts(ex->main_file);
        
        // Increment attempt counter
        increment_exercise_attempts(ex->main_file);
        
        // Show hint if this is at least the second attempt or we're not in hard mode
        int show_hint = (!hard_mode && attempts >= 1);
        print_help(ex, work_path, show_hint);
        
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
            // Get current attempt count before incrementing
            int attempts = get_exercise_attempts(ex->main_file);
            
            // Increment attempt counter
            increment_exercise_attempts(ex->main_file);
            
            // Show hint if this is at least the second attempt or we're not in hard mode
            int show_hint = (!hard_mode && attempts >= 1);
            print_help(ex, work_path, show_hint);
            return 1;
        }
    }

    // Reset attempt counter for this exercise since it passed
    FILE* attempts_file = fopen(attempts_filename, "r");
    if (attempts_file) {
        // Create a temporary file for writing the updated attempts
        FILE* temp_file = fopen(".temp_attempts.txt", "w");
        if (temp_file) {
            char line[MAX_PATH * 2];
            while (fgets(line, sizeof(line), attempts_file) != NULL) {
                // Skip the line for the current exercise
                if (strstr(line, ex->main_file) == NULL) {
                    fputs(line, temp_file);
                }
            }
            fclose(temp_file);
            fclose(attempts_file);
            rename(".temp_attempts.txt", attempts_filename);
        } else {
            fclose(attempts_file);
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

void print_help(const Exercise* ex, const char* work_path, int show_hint) {
    (void)work_path;
    
    // Always show the file path
    printf("%s📁 File: %s%s\n", BLUE, ex->main_file, RESET);
    
    // Only show hints if we're not in hard mode and this is at least the second attempt
    if (show_hint) {
        char* hint = get_hint_for_exercise(ex->main_file);
        if (hint) {
            printf("\n%s💡 Hint: %s%s\n", YELLOW, hint, RESET);
            free(hint);
        } else {
            printf("\n%s💡 Hint: %s%s\n", YELLOW, "No hint available for this exercise.", RESET);
        }
    } else if (!hard_mode) {
        printf("\n%s💡 Try once more for a hint!%s\n", YELLOW, RESET);
    }
}

int read_progress(int chapter_progress[MAX_CHAPTERS]) {
    // Initialize all chapter progress to 0
    for (int i = 0; i < MAX_CHAPTERS; i++) {
        chapter_progress[i] = 0;
    }
    
    FILE* progress_file = fopen(progress_filename, "r");
    if (!progress_file) {
        // It's okay if the file doesn't exist yet
        return 0;
    }

    if (flock(fileno(progress_file), LOCK_EX) == -1) {
        fclose(progress_file);
        return 1;
    }

    int chapter, exercise;
    while (fscanf(progress_file, "%d %d\n", &chapter, &exercise) == 2) {
        if (chapter >= 0 && chapter < MAX_CHAPTERS) {
            chapter_progress[chapter] = exercise;
        }
    }

    flock(fileno(progress_file), LOCK_UN);
    fclose(progress_file);
    return 0;
}

int update_progress(int chapter, int exercise_number) {
    if (chapter < 0 || chapter >= MAX_CHAPTERS) {
        return 1;
    }
    
    // Read current progress
    int chapter_progress[MAX_CHAPTERS];
    if (read_progress(chapter_progress) != 0) {
        return 1;
    }
    
    // Update only if the new exercise number is higher
    if (exercise_number <= chapter_progress[chapter]) {
        return 0;
    }
    
    chapter_progress[chapter] = exercise_number;
    
    // Write back all progress
    FILE* progress_file = fopen(progress_filename, "w");
    if (!progress_file) {
        return 1;
    }

    if (flock(fileno(progress_file), LOCK_EX) == -1) {
        fclose(progress_file);
        return 1;
    }

    for (int i = 0; i < MAX_CHAPTERS; i++) {
        if (chapter_progress[i] > 0) {
            fprintf(progress_file, "%d %d\n", i, chapter_progress[i]);
        }
    }
    
    flock(fileno(progress_file), LOCK_UN);
    fclose(progress_file);
    return 0;
}

int run_exercise_test_suite(const Exercise* ex, const char* work_path, const char* build_path) {
    (void)work_path;  // Silence unused parameter warning
    if (!ex->test_suite) {
        return 0;  // No test suite for this exercise
    }

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

int get_chapter_from_path(const char* path) {
    // Expected format: exercises/N_chaptername/N_MMM_exercisename.c
    // Extract the first number N
    
    const char* exercises_dir = "exercises/";
    if (strncmp(path, exercises_dir, strlen(exercises_dir)) != 0) {
        return -1;
    }
    
    const char* chapter_start = path + strlen(exercises_dir);
    return atoi(chapter_start);
}

int get_exercise_attempts(const char* exercise_path) {
    FILE* attempts_file = fopen(attempts_filename, "r");
    if (!attempts_file) {
        return 0;  // No attempts yet
    }

    char line[MAX_PATH * 2];
    while (fgets(line, sizeof(line), attempts_file) != NULL) {
        // Format: exercise_path attempts_count
        char stored_path[MAX_PATH];
        int attempts;
        if (sscanf(line, "%s %d", stored_path, &attempts) == 2) {
            if (strcmp(stored_path, exercise_path) == 0) {
                fclose(attempts_file);
                return attempts;
            }
        }
    }

    fclose(attempts_file);
    return 0;  // No attempts for this exercise
}

void increment_exercise_attempts(const char* exercise_path) {
    int current_attempts = get_exercise_attempts(exercise_path);
    current_attempts++;
    
    // Read all current attempts
    FILE* attempts_file = fopen(attempts_filename, "r");
    FILE* temp_file = fopen(".temp_attempts.txt", "w");
    
    int found = 0;
    
    if (attempts_file && temp_file) {
        char line[MAX_PATH * 2];
        while (fgets(line, sizeof(line), attempts_file) != NULL) {
            char stored_path[MAX_PATH];
            int attempts;
            if (sscanf(line, "%s %d", stored_path, &attempts) == 2) {
                if (strcmp(stored_path, exercise_path) == 0) {
                    // Update existing entry
                    fprintf(temp_file, "%s %d\n", exercise_path, current_attempts);
                    found = 1;
                } else {
                    // Copy other entries unchanged
                    fputs(line, temp_file);
                }
            }
        }
        
        // Add new entry if not found
        if (!found) {
            fprintf(temp_file, "%s %d\n", exercise_path, current_attempts);
        }
        
        fclose(attempts_file);
        fclose(temp_file);
        
        rename(".temp_attempts.txt", attempts_filename);
    } else {
        // Handle file open errors
        if (attempts_file) fclose(attempts_file);
        if (temp_file) fclose(temp_file);
        
        // If we couldn't open the existing file, create a new one
        attempts_file = fopen(attempts_filename, "w");
        if (attempts_file) {
            fprintf(attempts_file, "%s %d\n", exercise_path, current_attempts);
            fclose(attempts_file);
        }
    }
}

int read_config(void) {
    FILE* config_file = fopen(config_filename, "r");
    if (!config_file) {
        // Config file doesn't exist, create it with default values
        update_config(hard_mode);
        return 0;
    }

    char buffer[MAX_CONFIG_SIZE];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, config_file);
    fclose(config_file);
    
    if (bytes_read == 0) {
        // Empty file, create with defaults
        update_config(hard_mode);
        return 0;
    }
    
    buffer[bytes_read] = '\0';

    // Simple JSON parsing for hard_mode
    char* hard_mode_pos = strstr(buffer, "\"hard_mode\"");
    if (hard_mode_pos) {
        char* value_start = strchr(hard_mode_pos, ':');
        if (value_start) {
            value_start++; // Skip the colon
            // Skip whitespace
            while (*value_start == ' ' || *value_start == '\t') {
                value_start++;
            }
            
            if (strncmp(value_start, "true", 4) == 0) {
                hard_mode = 1;
            } else if (strncmp(value_start, "false", 5) == 0) {
                hard_mode = 0;
            }
        }
    }

    return 0;
}

int update_config(int new_hard_mode) {
    FILE* config_file = fopen(config_filename, "w");
    if (!config_file) {
        fprintf(stderr, "Failed to open config file for writing\n");
        return 1;
    }

    fprintf(config_file, "{\n    \"hard_mode\": %s\n}", new_hard_mode ? "true" : "false");
    fclose(config_file);
    
    // Update the global variable
    hard_mode = new_hard_mode;
    
    return 0;
}

char* get_hint_for_exercise(const char* exercise_path) {
    // Extract the file name from the path
    const char* file_name = strrchr(exercise_path, '/');
    if (!file_name) {
        file_name = exercise_path;
    } else {
        file_name++; // Skip the '/'
    }
    
    // Remove the .c extension if present
    char base_name[MAX_PATH];
    strncpy(base_name, file_name, sizeof(base_name) - 1);
    base_name[sizeof(base_name) - 1] = '\0';
    char* dot = strrchr(base_name, '.');
    if (dot) {
        *dot = '\0';
    }
    
    // Extract chapter directory from path
    char chapter_dir[MAX_PATH] = {0};
    const char* exercises_dir = "exercises/";
    const char* chapter_start = strstr(exercise_path, exercises_dir);
    
    if (chapter_start) {
        chapter_start += strlen(exercises_dir);
        const char* chapter_end = strchr(chapter_start, '/');
        if (chapter_end) {
            size_t dir_len = chapter_end - chapter_start;
            if (dir_len < sizeof(chapter_dir)) {
                strncpy(chapter_dir, chapter_start, dir_len);
                chapter_dir[dir_len] = '\0';
            }
        }
    }
    
    if (chapter_dir[0] == '\0') {
        // Fallback if we couldn't extract the chapter directory
        return NULL;
    }
    
    // Ensure the hints directory exists
    struct stat st = {0};
    if (stat(hints_dir, &st) == -1) {
        // Main hints directory doesn't exist, create it
        mkdir(hints_dir, 0755);
    }
    
    // Ensure the chapter directory exists within hints
    char chapter_path[MAX_PATH];
    snprintf(chapter_path, sizeof(chapter_path), "%s/%s", hints_dir, chapter_dir);
    
    if (stat(chapter_path, &st) == -1) {
        // Chapter directory doesn't exist, create it
        mkdir(chapter_path, 0755);
    }
    
    // Construct hint file path
    char hint_path[MAX_PATH];
    snprintf(hint_path, sizeof(hint_path), "%s/%s/%s.hint", 
             hints_dir, chapter_dir, base_name);
    
    // Read hint file
    FILE* hint_file = fopen(hint_path, "r");
    if (!hint_file) {
        // Hint file doesn't exist, try to create a default one
        hint_file = fopen(hint_path, "w");
        if (hint_file) {
            fprintf(hint_file, "No specific hint available for this exercise yet.");
            fclose(hint_file);
            hint_file = fopen(hint_path, "r");
        }
        
        if (!hint_file) {
            return NULL;
        }
    }
    
    char* hint = malloc(MAX_HINT_SIZE);
    if (!hint) {
        fclose(hint_file);
        return NULL;
    }
    
    size_t bytes_read = fread(hint, 1, MAX_HINT_SIZE - 1, hint_file);
    fclose(hint_file);
    
    if (bytes_read == 0) {
        free(hint);
        return NULL;
    }
    
    hint[bytes_read] = '\0';
    return hint;
} 