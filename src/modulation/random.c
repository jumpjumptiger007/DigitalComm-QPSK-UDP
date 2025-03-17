/**
 * Random Bit Generator
 * 
 * This program generates random binary data bits that can be used
 * as input for digital modulation schemes like QPSK.
 * 
 * Compile with: gcc -o random random.c
 * Run with: ./random
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BITS_COUNT 40  // Total number of random bits to generate

int main() {
    int i;
    
    // Initialize random number generator with current time as seed
    srand(time(0));  

    // Generate and output the random data bits
    printf("Random Generator:\n");
    int data_bits[BITS_COUNT];
    printf("data_bit[] = {");
    
    for (i = 0; i < BITS_COUNT; i++) {
        // Generate a random binary data bit (0 or 1)
        data_bits[i] = rand() % 2;  
        
        // Print the bit with appropriate formatting
        printf("%d", data_bits[i]);
        if (i < BITS_COUNT - 1) {
            printf(",");
        }
    }
    printf("}\n");
    
    return 0;
}
