#include <stdio.h>

/**
 * Exercise: Foggy Labels – Fix the Variable Names
 * 
 * A novice wizard wrote this code using unclear and cryptic variable names.
 * The spell still runs, but no one understands what it's doing!
 * 
 * Your task:
 * - Read the code and figure out what each variable likely represents.
 * - Replace the bad variable names with clear, meaningful ones.
 * - Keep the functionality exactly the same.
 * - Use comments to describe what each variable means.
 */

int main() {
    
   int mana_capacity = 50;
    float mana_regeneration = 73.2;
    char rank_symbol = 'R';

    printf("Mana Capacity: %d\n", mana_capacity);
    printf("Mana Regeneration: %.1f\n", mana_regeneration);
    printf("Rank Symbol: %c\n", rank_symbol);


    return 0;
}

