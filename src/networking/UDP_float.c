/**
 * UDP Float Transmission
 * 
 * This program demonstrates sending QPSK modulated symbols over UDP.
 * The float values (real and imaginary parts) are converted to byte arrays
 * before transmission.
 * 
 * Compile with: gcc -o udp_float UDP_float.c -lm
 * Run with: ./udp_float [config_file]
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

#define BITS_COUNT 40        // Total number of random bits to generate
#define SYMBOLS_COUNT 20     // Number of QPSK symbols (each symbol encodes 2 bits)
#define NOISE_STD_DEV 0.05   // Standard deviation of noise to be added to symbols
#define BUFFER_SIZE 256      // Size of the buffer for data transmission

#define CONFIG_FILE "config/udp_config.txt"  // Default configuration file path

/**
 * Structure to represent a complex number with real and imaginary parts
 */
typedef struct {
    double real;
    double imag;
} Complex;

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
    int data_bits[BITS_COUNT];
    for (i = 0; i < BITS_COUNT; i++) {
        data_bits[i] = rand() % 2;
    }

    // Step 2: Perform QPSK modulation to create symbols
    Complex symbols[SYMBOLS_COUNT];
    for (i = 0, j = 0; i < BITS_COUNT; i += 2, j++) {
        bit1 = data_bits[i];         // First bit
        bit2 = data_bits[i + 1];     // Second bit

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

        // Step 3: Add noise to the symbols
        double noise_real = NOISE_STD_DEV * (2 * ((double)rand() / RAND_MAX) - 1);
        double noise_imag = NOISE_STD_DEV * (2 * ((double)rand() / RAND_MAX) - 1);
        symbol.real += noise_real;
        symbol.imag += noise_imag;

        symbols[j] = symbol;
    }

    // Step 4: Display the modulated symbols
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
    
    // Step 5: Convert symbols to byte arrays for UDP transmission
    unsigned char byteBuffer[SYMBOLS_COUNT * sizeof(float) * 2];
    for (i = 0, j = 0; i < SYMBOLS_COUNT; i++, j += sizeof(float) * 2) {
        floatToBytes(symbols[i].real, byteBuffer + j);
        floatToBytes(symbols[i].imag, byteBuffer + j + sizeof(float));
    }

    // Step 6: Set up UDP socket for transmission
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

    // Step 7: Send the data
    sendto(sockfd, byteBuffer, sizeof(byteBuffer), 0, (struct sockaddr *)&saddr, sizeof(saddr));

    // Close the socket
    close(sockfd);

    printf("Message has been sent to %s:%d.\n", config.ip_address, config.port);

    return 0;
}