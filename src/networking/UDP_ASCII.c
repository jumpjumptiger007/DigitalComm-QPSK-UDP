/**
 * UDP ASCII Transmission of QPSK Symbols
 * 
 * This program demonstrates generating QPSK modulated symbols with noise,
 * converting them to ASCII text representation, and sending them via UDP.
 * 
 * Compile with: gcc -o udp_ascii UDP_ASCII.c -lm
 * Run with: ./udp_ascii
 * 
 * Note: Configure the IP address and port before running.
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

#define BITS_COUNT 40          // Total number of random bits to generate
#define SYMBOLS_COUNT 20       // Number of QPSK symbols (each symbol encodes 2 bits)
#define NOISE_STD_DEV 0.1      // Standard deviation of noise to be added to symbols
#define BUFFER_SIZE 256        // Size of the buffer for data transmission

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
        data_bits[i] = rand() % 2;
    }

    // Step 2: Perform QPSK modulation to create symbols
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
    
    // Step 4: Convert the combined symbols to a string buffer
    char buff[BUFFER_SIZE];
    memset(buff, 0, sizeof(buff));
    int offset = 0;
    for (i = 0; i < SYMBOLS_COUNT; i++) {
        offset += snprintf(buff + offset, BUFFER_SIZE - offset, "(%f,%f)", 
                         qpsk_symbols[i].real, qpsk_symbols[i].imag);
        if (i < SYMBOLS_COUNT - 1) {
            offset += snprintf(buff + offset, BUFFER_SIZE - offset, ",");
        }
    }

    // Step 5: Send the buffer over UDP
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        return 1;
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    
    // Configure port number and IP address
    saddr.sin_port = htons(9091); 
    saddr.sin_addr.s_addr = inet_addr("192.168.188.29");

    sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&saddr, sizeof(saddr));

    close(sockfd);

    printf("Message has been sent.\n");

    return 0;
}
