/**
 * QPSK Modulation with Noise (Complex Number Implementation)
 * 
 * This program demonstrates QPSK modulation using complex numbers and 
 * adds random noise to simulate a realistic communication channel.
 * 
 * Compile with: gcc -o noise_combo noise_combo.c -lm
 * Run with: ./noise_combo
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define BITS_COUNT 40        // Total number of random bits to generate
#define SYMBOLS_COUNT 20     // Number of QPSK symbols (each symbol encodes 2 bits)
#define NOISE_STD_DEV 0.05   // Standard deviation of noise to be added to symbols

/**
 * Structure to represent a complex number with real and imaginary parts
 */
typedef struct {
    double real;
    double imag;
} Complex;

int main() {
    int i, j, bit1, bit2;

    // Initialize random number generator with current time as seed
    srand(time(0));

    // Step 1: Generate random data bits
    int data_bits[BITS_COUNT];
    for (i = 0; i < BITS_COUNT; i++) {
        // Generate a random binary data bit (0 or 1)
        data_bits[i] = rand() % 2;
    }

    // Step 2: Perform QPSK modulation and add noise to create symbols
    Complex symbols[SYMBOLS_COUNT];
    for (i = 0, j = 0; i < BITS_COUNT; i += 2, j++) {
        bit1 = data_bits[i];
        bit2 = data_bits[i + 1];

        // Map the two bits to a symbol using QPSK modulation
        Complex symbol;
        if (bit1 == 0 && bit2 == 0) {
            symbol.real = 1 / sqrt(2);
            symbol.imag = 1 / sqrt(2);
        } else if (bit1 == 0 && bit2 == 1) {
            symbol.real = -1 / sqrt(2);
            symbol.imag = 1 / sqrt(2);
        } else if (bit1 == 1 && bit2 == 0) {
            symbol.real = 1 / sqrt(2);
            symbol.imag = -1 / sqrt(2);
        } else if (bit1 == 1 && bit2 == 1) {
            symbol.real = -1 / sqrt(2);
            symbol.imag = -1 / sqrt(2);
        }

        // Add noise to the symbols
        double noise_real = NOISE_STD_DEV * (2 * ((double)rand() / RAND_MAX) - 1);
        double noise_imag = NOISE_STD_DEV * (2 * ((double)rand() / RAND_MAX) - 1);
        symbol.real += noise_real;
        symbol.imag += noise_imag;

        symbols[j] = symbol;
    }

    // Step 3: Output the combined array of symbols
    printf("Combined array of symbols:\n");
    Complex qpsk_symbols[SYMBOLS_COUNT];
    printf("qpsk_symbols[] = {");
    for (i = 0; i < SYMBOLS_COUNT; i++) {
        qpsk_symbols[i] = symbols[i];
        printf("(%f,%f)", qpsk_symbols[i].real, qpsk_symbols[i].imag);
        if (i < SYMBOLS_COUNT - 1) {
            printf(",");
        }
    }
    printf("}\n");

    return 0;
}
