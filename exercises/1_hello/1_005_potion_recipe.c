#include <stdio.h>

/**
 * Exercise: Potion Recipe
 * 
 * As a wizard's apprentice, you need to create a potion recipe card.
 * The recipe requires precise measurements and formatting!
 * 
 * Your task:
 * 1. Fix the printf statements to display the recipe correctly
 * 2. Use the correct format specifiers for each variable type
 * 3. Make sure the output matches the expected format
 */

int main() {
    // Potion ingredients and measurements
    int dragon_scales = 3;
    float fairy_dust = 1.5;
    char potion_grade = 'A';
    float magic_power = 42.424242;
    char potion_name[] = "Elixir of Wisdom";
    
    // TODO: Fix the format specifiers in the printf statements below
    
    printf("=== MAGIC POTION RECIPE ===\n");
    printf("Name: XXXX\n", potion_name);
    printf("Grade: XXXX\n", potion_grade);
    
    printf("\nINGREDIENTS:\n");
    printf("- Dragon Scales: XXXX pieces\n", dragon_scales);
    printf("- Fairy Dust: XXXX ounces\n", fairy_dust);
    
    printf("\nMAGIC PROPERTIES:\n");
    printf("- Power Level: XXXX\n", magic_power);
    
    return 0;
} 