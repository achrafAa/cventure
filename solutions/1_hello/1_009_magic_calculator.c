#include <stdio.h>

/* Solution: Magic Calculator */

int main() {
    // Declare and initialize two integer variables
    int first_number = 7;
    int second_number = 3;
    
    // Calculate the sum
    int sum = first_number + second_number;
    
    // Calculate the difference
    int difference = first_number - second_number;
    
    // Calculate the product
    int product = first_number * second_number;
    
    // Calculate the quotient
    int quotient = first_number / second_number;
    
    // Calculate the remainder
    int remainder = first_number % second_number;
    
    // Print all the results
    printf("Magic Calculator Results:\n");
    printf("%d + %d = %d\n", first_number, second_number, sum);
    printf("%d - %d = %d\n", first_number, second_number, difference);
    printf("%d * %d = %d\n", first_number, second_number, product);
    printf("%d / %d = %d\n", first_number, second_number, quotient);
    printf("%d %% %d = %d\n", first_number, second_number, remainder);
    
    return 0;
} 