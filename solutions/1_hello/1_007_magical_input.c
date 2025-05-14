#include <stdio.h>

/* Solution: Magical Input */

int main() {
    // Declare a variable to store the spell number
    int spellNumber;
    
    // Ask the user to enter a spell number
    printf("Enter spell number: ");
    
    // Use scanf to read the input
    scanf("%d", &spellNumber);
    
    // Print the spell number that was entered
    printf("You selected spell #%d!\n", spellNumber);
    
    return 0;
} 