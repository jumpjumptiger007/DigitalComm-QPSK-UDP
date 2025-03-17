/**
 * QPSK Modulation Implementation
 * 
 * This program demonstrates Quadrature Phase Shift Keying (QPSK) modulation,
 * a digital modulation technique that encodes two bits per symbol.
 * 
 * Compile with: gcc -o qpsk_demo QPSK.c -lm
 * Run with: ./qpsk_demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define BITS_COUNT 40        // Total number of random bits to generate
#define SYMBOLS_COUNT 20     // Number of QPSK symbols (each symbol encodes 2 bits)

int main() {
    int i, j, bit1, bit2;
    double symbol_I, symbol_Q, symbol_magnitude, symbol_phase;
    
    // Initialize random number generator with current time as seed
    srand(time(0));

    // Step 1: Generate random data bits
    printf("Random Generator for %d data bits:\n", BITS_COUNT);
    int data_bits[BITS_COUNT];
    printf("data_bit[] = {");
    for (i = 0; i < BITS_COUNT; i++) {
        // Generate a random binary data bit (0 or 1)
        data_bits[i] = rand() % 2; 
        printf("%d", data_bits[i]);
        if (i < BITS_COUNT - 1) {
            printf(",");
        }
    }
    printf("}\n");

    // Step 2: Perform QPSK modulation to create symbols
    // Each QPSK symbol represents 2 bits of data
    printf("QPSK modulation for %d symbols:\n", SYMBOLS_COUNT);
    double symbols_I[SYMBOLS_COUNT], symbols_Q[SYMBOLS_COUNT];
    for (i = 0, j = 0; i < BITS_COUNT; i += 2, j++) {
        bit1 = data_bits[i];         // First bit
        bit2 = data_bits[i+1];       // Second bit

        // Map the two bits to a QPSK symbol
        // The mapping follows Gray code to minimize bit errors:
        // 00 -> (+1/√2, +1/√2)  | First quadrant
        // 01 -> (-1/√2, +1/√2)  | Second quadrant
        // 10 -> (+1/√2, -1/√2)  | Fourth quadrant
        // 11 -> (-1/√2, -1/√2)  | Third quadrant
        if (bit1 == 0 && bit2 == 0) {
            symbol_I = 1 / sqrt(2);   // Real part (I)
            symbol_Q = 1 / sqrt(2);   // Imaginary part (Q)
        } else if (bit1 == 0 && bit2 == 1) {
            symbol_I = -1 / sqrt(2);
            symbol_Q = 1 / sqrt(2);
        } else if (bit1 == 1 && bit2 == 0) {
            symbol_I = 1 / sqrt(2);
            symbol_Q = -1 / sqrt(2);
        } else if (bit1 == 1 && bit2 == 1) {
            symbol_I = -1 / sqrt(2);
            symbol_Q = -1 / sqrt(2);
        }

        // Store the I and Q components of the symbol
        symbols_I[j] = symbol_I;
        symbols_Q[j] = symbol_Q;

        // Uncomment to print each symbol
        // printf("Symbol %d: (%f,%f)\n", j+1, symbol_I, symbol_Q);
    }

    // Step 3: Output the QPSK symbols in arrays of real and imaginary parts
    printf("QPSK modulation for %d symbols:\n", SYMBOLS_COUNT);
    double qpsk_symbol_real[SYMBOLS_COUNT], qpsk_symbol_imag[SYMBOLS_COUNT];
    
    // Output the real parts
    printf("qpsk_symbol_real[] = {");
    for (i = 0; i < SYMBOLS_COUNT; i++) {
        qpsk_symbol_real[i] = symbols_I[i];
        printf("%f", qpsk_symbol_real[i]);
        if (i < SYMBOLS_COUNT - 1) {
            printf(",");
        }
    }
    printf("}\n");

    // Output the imaginary parts
    printf("qpsk_symbol_imag[] = {");
    for (i = 0; i < SYMBOLS_COUNT; i++) {
        qpsk_symbol_imag[i] = symbols_Q[i];
        printf("%f", qpsk_symbol_imag[i]);
        if (i < SYMBOLS_COUNT - 1) {
            printf(",");
        }
    }
    printf("}\n");

    return 0;
}