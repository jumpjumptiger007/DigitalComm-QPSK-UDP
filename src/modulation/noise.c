/**
 * QPSK Modulation with Noise
 * 
 * This program demonstrates QPSK modulation with the addition of random noise
 * to simulate a realistic communication channel.
 * 
 * Compile with: gcc -o qpsk_noise noise.c -lm
 * Run with: ./qpsk_noise
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define BITS_COUNT 40         // Total number of random bits to generate
#define SYMBOLS_COUNT 20      // Number of QPSK symbols (each symbol encodes 2 bits)
#define NOISE_STD_DEV 0.05    // Standard deviation of the noise to be added to symbols
                              // Smaller values = less noise, larger values = more noise

int main() {
    int i, j, bit1, bit2;
    double symbol_I, symbol_Q, symbol_magnitude, symbol_phase, noise_I, noise_Q;
    
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

    // Step 2: Perform QPSK modulation to create symbols (without noise)
    double symbols_I[SYMBOLS_COUNT], symbols_Q[SYMBOLS_COUNT];
    for (i = 0, j = 0; i < BITS_COUNT; i += 2, j++) {
        bit1 = data_bits[i];
        bit2 = data_bits[i+1];

        // Map the two bits to a symbol using QPSK modulation
        // The mapping follows Gray code to minimize bit errors
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

        symbols_I[j] = symbol_I;
        symbols_Q[j] = symbol_Q;
    }
        
    // Step 3: Add noise to the symbols to simulate a noisy channel
    // Noise is added to both the I and Q components of each symbol
    for (i = 0; i < SYMBOLS_COUNT; i++) {
        // Generate Gaussian noise with zero mean and standard deviation of NOISE_STD_DEV
        // This is a simplified approach to generate approximately Gaussian noise
        noise_I = NOISE_STD_DEV * ((double) rand() / RAND_MAX) * sqrt(-2 * log((double) rand() / RAND_MAX));
        noise_Q = NOISE_STD_DEV * ((double) rand() / RAND_MAX) * sqrt(-2 * log((double) rand() / RAND_MAX));
        
        // Add the noise to the symbol components
        symbols_I[i] += noise_I;
        symbols_Q[i] += noise_Q;
        
        // Uncomment to see the effect of noise on each symbol
        // printf("Symbol %d: (%f,%f) -> (%f,%f)\n", i+1, symbols_I[i]-noise_I, symbols_Q[i]-noise_Q, symbols_I[i], symbols_Q[i]);
    }

    // Step 4: Output the noisy QPSK symbols
    printf("QPSK modulation for %d symbols with noise:\n", SYMBOLS_COUNT);
    
    // Output the real parts (with noise)
    double qpsk_symbol_real[SYMBOLS_COUNT], qpsk_symbol_imag[SYMBOLS_COUNT];
    printf("qpsk_symbol_real[] = {");
    for (i = 0; i < SYMBOLS_COUNT; i++) {
        qpsk_symbol_real[i] = symbols_I[i];
        printf("%f", qpsk_symbol_real[i]);
        if (i < SYMBOLS_COUNT - 1) {
            printf(",");
        }
    }
    printf("}\n");

    // Output the imaginary parts (with noise)
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