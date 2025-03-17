/**
 * UDP Client Implementation
 * 
 * This program creates a UDP client that can send messages to a server
 * and receive responses. It continues this loop until "end" is typed.
 * 
 * Compile with: gcc -o client Client.c
 * Run with: ./client
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sockfd != -1);

    // Configure server address
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Loop for sending and receiving messages
    while(1)
    {
        char buff[128] = {0};
        printf("input:\n");

        // Get user input
        fgets(buff, 128, stdin);

        // Check if user wants to exit
        if (strncmp(buff, "end", 3) == 0)
        {
            break;
        }

        // Send the message to the server
        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&saddr, sizeof(saddr));
        memset(buff, 0, 128);

        // Receive response from the server
        int len = sizeof(saddr);
        recvfrom(sockfd, buff, 127, 0, (struct sockaddr*)&saddr, &len);

        // Display the response
        printf("buff=%s\n", buff);
    }

    // Close the socket
    close(sockfd);
    
    return 0;
}
