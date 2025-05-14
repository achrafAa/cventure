#include <stdio.h>

/* Solution: Spell Components Input */

int main() {
    // Declare variables for different data types
    int spell_power;
    float ingredient_weight;
    char magical_grade;
    
    // Prompt for and get the spell power (integer)
    printf("Enter spell power: ");
    scanf("%d", &spell_power);
    
    // Prompt for and get the ingredient weight (float)
    printf("Enter ingredient weight: ");
    scanf("%f", &ingredient_weight);
    
    // Prompt for and get the magical grade (character)
    printf("Enter magical grade: ");
    scanf(" %c", &magical_grade);  // Note the space before %c
    
    // Print all the collected information
    printf("Spell Inventory:\n");
    printf("- Power: %d\n", spell_power);
    printf("- Weight: %.2f oz\n", ingredient_weight);
    printf("- Grade: %c\n", magical_grade);
    
    return 0;
} 