/**
 * Final UDP Transmission Implementation with QPSK
 * 
 * This program demonstrates the complete pipeline:
 * 1. Generate random bits
 * 2. QPSK modulation
 * 3. Add noise
 * 4. Format data with padding
 * 5. Send over UDP
 * 
 * Compile with: gcc -o udp_final UDP_final.c -lm
 * Run with: ./udp_final [config_file]
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>

#include "../utils/udp_config.h"

#define BITS_COUNT 40            // Total number of random bits to generate
#define SYMBOLS_COUNT 20         // Number of QPSK symbols (each symbol encodes 2 bits)
#define COMBINATION_LENGTH 256*3 // Length of combined data array
#define BUFFER_LENGTH 256*3*4    // Length of final buffer (4 copies of combined data)
#define NOISE_STD_DEV 0.5        // Standard deviation of noise to be added to symbols

#define CONFIG_FILE "config/udp_config.txt"  // Default configuration file path

/**
 * Function to convert a float value to a byte array
 * 
 * @param value  The float value to convert
 * @param bytes  Pointer to the byte array where the result will be stored
 */
void floatToBytes(float value, unsigned char *bytes) {
    memcpy(bytes, &value, sizeof(float));
}

int main(int argc, char *argv[]) {
    int i, j, bit1, bit2;
    double symbol_I, symbol_Q, symbol_magnitude, symbol_phase, noise_I, noise_Q;
    
    // Initialize UDP configuration with default values (localhost)
    UDPConfig config;
    init_udp_config(&config);
    
    // Load configuration from file if specified
    const char *config_file = (argc > 1) ? argv[1] : CONFIG_FILE;
    if (load_udp_config(&config, config_file)) {
        printf("Loaded configuration from %s\n", config_file);
    } else {
        printf("Using default configuration (localhost:9090)\n");
    }
    
    // Display current configuration
    print_udp_config(&config);
    
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
    double symbols_I[SYMBOLS_COUNT], symbols_Q[SYMBOLS_COUNT];
    for (i = 0, j = 0; i < BITS_COUNT; i += 2, j++) {
        bit1 = data_bits[i];
        bit2 = data_bits[i+1];

        // Map the two bits to a symbol using QPSK modulation
        if (bit1 == 0 && bit2 == 0) {
            symbol_I = 1 / sqrt(2);
            symbol_Q = 1 / sqrt(2);
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
        
    // Step 3: Add noise to the symbols
    for (i = 0; i < SYMBOLS_COUNT; i++) {
        // Generate noise with zero mean and standard deviation of NOISE_STD_DEV
        noise_I = NOISE_STD_DEV * ((double) rand() / RAND_MAX) * sqrt(-2 * log((double) rand() / RAND_MAX));
        noise_Q = NOISE_STD_DEV * ((double) rand() / RAND_MAX) * sqrt(-2 * log((double) rand() / RAND_MAX));
        symbols_I[i] += noise_I;
        symbols_Q[i] += noise_Q;
    }

    // Step 4: Display the noisy QPSK symbols
    printf("QPSK modulation for %d symbols with noise:\n", SYMBOLS_COUNT);
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

    printf("qpsk_symbol_imag[] = {");
    for (i = 0; i < SYMBOLS_COUNT; i++) {
        qpsk_symbol_imag[i] = symbols_Q[i];
        printf("%f", qpsk_symbol_imag[i]);
        if (i < SYMBOLS_COUNT - 1) {
            printf(",");
        }
    }
    printf("}\n");
    
    // Step 5: Prepare data for UDP transmission
    float comb[COMBINATION_LENGTH], buffer[BUFFER_LENGTH];
    printf("The Array : {");

    // Fill first 256 elements with zeros
    for (i = 0; i < 256; i++) {
        comb[i] = 0;
        printf("%d , ", (int)comb[i]);
    }

    // Fill next 20 elements with real parts of symbols
    for (i = 256; i < 276; i++) {
        comb[i] = qpsk_symbol_real[i-256];
        printf("%f , ", comb[i]);
    }

    // Fill next portion with zeros
    for(i = 276; i < 256*2; i++) {
        comb[i] = 0;
        printf("%d , ", (int)comb[i]);
    }

    // Fill next 20 elements with imaginary parts of symbols
    for(i = 256*2; i < 256*2+20; i++) {
        comb[i] = qpsk_symbol_imag[i-256*2];
        printf("%f , ", comb[i]);
    }

    // Fill remainder with zeros
    for(i = 256*2+20; i < COMBINATION_LENGTH; i++) {
        comb[i] = 0;
        printf("%d , ", (int)comb[i]);
    }

    // Print array values for debugging
    for(i = 0; i < 768; i++) {
        printf("%d,%f\n", i, comb[i]);
    }

    printf("}\n");

    // Step 6: Convert symbols to byte arrays for UDP transmission
    unsigned char byteBuffer[SYMBOLS_COUNT * sizeof(float) * 2];
    for (i = 0; i < 768; i++) {
        floatToBytes(comb[i], byteBuffer + 4*i);
    }   
    
    // Step 7: Setup UDP socket for transmission
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        return 1;
    }

    // Configure server address using loaded configuration
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(config.port);
    saddr.sin_addr.s_addr = inet_addr(config.ip_address);

    // Send the data
    sendto(sockfd, comb, (COMBINATION_LENGTH)*4, 0, (struct sockaddr *)&saddr, sizeof(saddr));

    // Close the socket
    close(sockfd);

    printf("Message has been sent to %s:%d.\n", config.ip_address, config.port);
    printf("\n");
    
    return 0;
}