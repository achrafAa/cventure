#include <stdio.h>

int main() {
    // Initialize potion_count to 5
    int potion_count = 5;
    int temp;
    
    // Display initial potion_count
    printf("Initial potion count: %d\n", potion_count);
    
    // Display potion_count and then increment it (post-increment)
    temp = potion_count;
    potion_count++;
    printf("Post-increment: %d (now %d)\n", temp, potion_count);
    
    // Increment potion_count and then display it (pre-increment)
    potion_count++;
    printf("Pre-increment: %d\n", potion_count);
    
    // Display potion_count and then decrement it (post-decrement)
    temp = potion_count;
    potion_count--;
    printf("Post-decrement: %d (now %d)\n", temp, potion_count);
    
    // Decrement potion_count and then display it (pre-decrement)
    potion_count--;
    printf("Pre-decrement: %d\n", potion_count);
    
    return 0;
} 