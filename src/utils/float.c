/**
 * Float to Bytes Conversion for UDP Transmission
 * 
 * This program demonstrates how to convert floating point values to byte arrays
 * and send them over UDP for communication with tools like Packet Sender.
 * 
 * Compile with: gcc -o float_udp float.c
 * Run with: ./float_udp
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * Function to convert a float value to a byte array
 * 
 * @param value  The float value to convert
 * @param bytes  Pointer to the byte array where the result will be stored
 */
void floatToBytes(float value, unsigned char *bytes) {
    memcpy(bytes, &value, sizeof(float));
}

int main() {
    float floatValue = 3.14;  // Test value

    // Create an array of float values to send
    float byteBuffer[2] = {1.2345, 2.6};

    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        return 1;
    }

    // Configure server address
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    
    // Enter port number here
    saddr.sin_port = htons(9091); 
    
    // Enter IP address here
    saddr.sin_addr.s_addr = inet_addr("192.168.188.29");

    // Send the float data over UDP
    sendto(sockfd, byteBuffer, sizeof(float), 0, (struct sockaddr *)&saddr, sizeof(saddr));

    // Close the socket
    close(sockfd);

    printf("Float data has been sent successfully.\n");

    return 0;
}
