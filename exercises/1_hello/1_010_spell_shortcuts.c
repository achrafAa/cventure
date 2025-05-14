#include <stdio.h>

/**
 * Exercise: Spell Shortcuts
 * 
 * Experienced wizards use shortcuts to modify their spell variables quickly.
 * These shortcuts are called compound assignment operators.
 * 
 * Your task:

 * - Use compound assignment operators to:
 *    a. Add 50 to mana_pool
 *    b. Subtract 25 from mana_pool
 *    c. Multiply mana_pool by 2
 *    d. Divide mana_pool by 5
 * 
 * Rules:
 * - You must use compound assignment operators (+=, -=, *=, /=)
 * - Do not use regular assignment (=) except for the initial value
 */

int main() {
   
    // Initialize mana_pool to 100
    int mana_pool = 100;
    
    // Display the initial mana_pool value
    printf("Initial mana pool: %d\n", mana_pool);
    
    // Add 50 to mana_pool using += and display the result

    printf("After adding 50: %d\n", mana_pool);
    
    // Subtract 25 from mana_pool using -= and display the result

    printf("After subtracting 25: %d\n", mana_pool);
    
    // Multiply mana_pool by 2 using *= and display the result

    printf("After multiplying by 2: %d\n", mana_pool);
    
    // Divide mana_pool by 5 using /= and display the result

    printf("After dividing by 5: %d\n", mana_pool);
    
    return 0;
}

