#ifndef EXERCISES_H
#define EXERCISES_H

#include <stdbool.h>
#include <stddef.h>  // For NULL definition

typedef struct {
    const char* main_file;
    const char* output;
    bool check_stdout;
    const char* test_suite;  // Path to test suite file
    const char* test_name;   // Name of the test suite
} Exercise;

static Exercise exercises[] = {
    // 1. Hello World
    {
        "exercises/1_hello/1_001_hello.c",
        "Hello, World!",
        true,
        "tests/1_hello/1_001_hello_test.c",
        "Hello World Test Suite"
    },
    {
        "exercises/1_hello/1_002_welcome_message.c",
        "Welcome to C programming!\nLet the adventure begin!",
        true,
        "tests/1_hello/1_002_welcome_message_test.c",
        "Welcome Message Test Suite"
    },
    {
        "exercises/1_hello/1_003_welcome_message2.c",
        "Welcome to C programming!\nLet the adventure begin!",
        true,
        "tests/1_hello/1_003_welcome_message2_test.c",
        "Welcome Message 2 Test Suite"
    },
    {
        "exercises/1_hello/1_004_escape_runes.c",
        "This is a \"quoted\" text\nThe file is located at C:\\Program Files\\App\n10% of the work is done\n",
        true,
        "tests/1_hello/1_004_escape_runes_test.c",
        "Escape Runes Test Suite"
    },
    {
        "exercises/1_hello/1_005_potion_recipe.c",
        "=== MAGIC POTION RECIPE ===\nName: Elixir of Wisdom\nGrade: A\n\nINGREDIENTS:\n- Dragon Scales: 3 pieces\n- Fairy Dust: 1.50 ounces\n\nMAGIC PROPERTIES:\n- Power Level: 42.42\n",
        true,
        "tests/1_hello/1_005_potion_recipe_test.c",
        "Potion Recipe Test Suite"
    }
};

#endif // EXERCISES_H 