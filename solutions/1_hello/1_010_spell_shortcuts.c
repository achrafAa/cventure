#include <stdio.h>

/* Solution: Spell Shortcuts */

int main() {
    // Initialize mana_pool to 100
    int mana_pool = 100;
    
    // Display the initial mana_pool value
    printf("Initial mana pool: %d\n", mana_pool);
    
    // Add 50 to mana_pool using += and display the result
    mana_pool += 50;
    printf("After adding 50: %d\n", mana_pool);
    
    // Subtract 25 from mana_pool using -= and display the result
    mana_pool -= 25;
    printf("After subtracting 25: %d\n", mana_pool);
    
    // Multiply mana_pool by 2 using *= and display the result
    mana_pool *= 2;
    printf("After multiplying by 2: %d\n", mana_pool);
    
    // Divide mana_pool by 5 using /= and display the result
    mana_pool /= 5;
    printf("After dividing by 5: %d\n", mana_pool);
    
    return 0;
} 