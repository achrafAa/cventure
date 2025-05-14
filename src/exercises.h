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
    bool skip_output_check;  // Skip output check completely (for interactive programs)
} Exercise;

static Exercise exercises[] = {
    // 1. Hello World
    {
        "exercises/1_hello/1_001_hello.c",
        "Hello, World!",
        true,
        "tests/1_hello/1_001_hello_test.c",
        "Hello World Test Suite",
        false
    },
    {
        "exercises/1_hello/1_002_welcome_message.c",
        "Welcome to C programming!\nLet the adventure begin!",
        true,
        "tests/1_hello/1_002_welcome_message_test.c",
        "Welcome Message Test Suite",
        false
    },
    {
        "exercises/1_hello/1_003_welcome_message2.c",
        "Welcome to C programming!\nLet the adventure begin!",
        true,
        "tests/1_hello/1_003_welcome_message2_test.c",
        "Welcome Message 2 Test Suite",
        false
    },
    {
        "exercises/1_hello/1_004_escape_runes.c",
        "This is a \"quoted\" text\nThe file is located at C:\\Program Files\\App\n10% of the work is done\n",
        true,
        "tests/1_hello/1_004_escape_runes_test.c",
        "Escape Runes Test Suite",
        false
    },
    {
        "exercises/1_hello/1_005_potion_recipe.c",
        "=== MAGIC POTION RECIPE ===\nName: Elixir of Wisdom\nGrade: A\n\nINGREDIENTS:\n- Dragon Scales: 3 pieces\n- Fairy Dust: 1.50 ounces\n\nMAGIC PROPERTIES:\n- Power Level: 42.42\n",
        true,
        "tests/1_hello/1_005_potion_recipe_test.c",
        "Potion Recipe Test Suite",
        false
    },
    {
        "exercises/1_hello/1_006_magic_variables.c",
        "Wizard level: 42\nSpell power: 9.75\nMagic symbol: *\n",
        true,
        "tests/1_hello/1_006_magic_variables_test.c",
        "Magic Variables Test Suite",
        false
    },
    {
        "exercises/1_hello/1_007_magical_input.c",
        "Exercise 1_007_magical_input.c requires input",
        false,
        "tests/1_hello/1_007_magical_input_test.c",
        "Magical Input Test Suite",
        true 
    },
    {
        "exercises/1_hello/1_008_spell_components_input.c",
        "Exercise 1_008_spell_components_input.c requires input",
        false,
        "tests/1_hello/1_008_spell_components_input_test.c",
        "Spell Components Input Test Suite",
        true
    },
    {
        "exercises/1_hello/1_009_magic_calculator.c",
        "Magic Calculator Results:\n7 + 3 = 10\n7 - 3 = 4\n7 * 3 = 21\n7 / 3 = 2\n7 % 3 = 1",
        true,
        "tests/1_hello/1_009_magic_calculator_test.c",
        "Magic Calculator Test Suite",
        false
    },
    {
        "exercises/1_hello/1_010_spell_shortcuts.c",
        "Initial mana pool: 100\nAfter adding 50: 150\nAfter subtracting 25: 125\nAfter multiplying by 2: 250\nAfter dividing by 5: 50",
        true,
        "tests/1_hello/1_010_spell_shortcuts_test.c",
        "Spell Shortcuts Test Suite",
        false
    },
    {
        "exercises/1_hello/1_011_potion_counter.c",
        "Initial potion count: 5\nPost-increment: 5 (now 6)\nPre-increment: 7\nPost-decrement: 7 (now 6)\nPre-decrement: 5",
        true,
        "tests/1_hello/1_011_potion_counter_test.c",
        "Potion Counter Test Suite",
        false
    },
    // 2. Variables
    {
        "exercises/2_variables/2_001_foggy_labels.c",
        "Mana Capacity: 50\nMana Regeneration: 73.2\nRank Symbol: R\n",
        true,
        "tests/2_variables/2_001_foggy_labels_test.c",
        "Foggy Labels Test Suite",
        false
    }
};

#endif // EXERCISES_H 