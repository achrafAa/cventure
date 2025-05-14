#include <stdio.h>

/**
 * Exercise: Potion Counter
 * 
 * Alchemists need to carefully track their potion inventory.
 * In C, increment (++) and decrement (--) operators help with precise counting.
 * 
 * Your task:   
 * - Demonstrate the following operations:
 *    a. Post-increment: display potion_count, then increment it
 *    b. Pre-increment: increment potion_count, then display it
 *    c. Post-decrement: display potion_count, then decrement it
 *    d. Pre-decrement: decrement potion_count, then display it
 * 
 * Rules:
 * - Use only the increment (++) and decrement (--) operators
 * - Use the correct pre/post notation as specified in the instructions
 */

int main() {

  // Initialize potion_count to 5
    int potion_count = 5;
    int temp;
    
    // Display initial potion_count
    printf("Initial potion count: %d\n", potion_count);
    
    // Display potion_count and then increment it (post-increment)
    temp = potion_count;
    // TODO: post-increment potion_count
    printf("Post-increment: %d (now %d)\n", temp, potion_count);
    
    // Increment potion_count and then display it (pre-increment)
    // TODO: pre-increment potion_count 
    printf("Pre-increment: %d\n", potion_count);
    
    // Display potion_count and then decrement it (post-decrement)
    // TODO: post-decrement potion_count
    printf("Post-decrement: %d (now %d)\n", temp, potion_count);
    
    // Decrement potion_count and then display it (pre-decrement)
    // TODO: pre-decrement potion_count
    printf("Pre-decrement: %d\n", potion_count);

    return 0;
}
