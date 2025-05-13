#include <stdio.h>

/**
 * Solution: Potion Recipe
 */

int main() {
    // Potion ingredients and measurements
    int dragon_scales = 3;
    float fairy_dust = 1.5;
    char potion_grade = 'A';
    float magic_power = 42.424242;
    char potion_name[] = "Elixir of Wisdom";
    
    printf("=== MAGIC POTION RECIPE ===\n");
    printf("Name: %s\n", potion_name);
    printf("Grade: %c\n", potion_grade);
    
    printf("\nINGREDIENTS:\n");
    printf("- Dragon Scales: %d pieces\n", dragon_scales);
    printf("- Fairy Dust: %.2f ounces\n", fairy_dust);
    
    printf("\nMAGIC PROPERTIES:\n");
    printf("- Power Level: %.2f\n", magic_power);
    
    return 0;
} 