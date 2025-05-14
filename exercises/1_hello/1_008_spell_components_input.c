#include <stdio.h>

/**
 * Exercise: Spell Components Input
 * 
 * A potion master needs to gather information about spell components.
 * Create a program that collects different types of data from the user.
 * 
 * Your task:
 * - Use scanf to read each input with the right format specifier
 */

int main() {
    int spell_power;    
    float ingredient_weight;
    char magical_grade;
    // TODO: Prompt for and get the spell power (integer)
    printf("Enter spell power: ");

    // TODO: Prompt for and get the ingredient weight (float)
    printf("Enter ingredient weight: ");

    // TODO: Prompt for and get the magical grade (character)
    printf("Enter magical grade: ");

    printf("Spell Inventory:\n");
    printf("- Power: %d\n", spell_power);
    printf("- Weight: %.2f oz\n", ingredient_weight);
    printf("- Grade: %c\n", magical_grade);
    return 0;
}
