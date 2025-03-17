/**
 * UDP Configuration Header
 * 
 * Common configuration for UDP transmission to ensure consistency
 * across different implementations.
 * 
 * Configuration Guide:
 * 
 * 1. IP Address Configuration:
 *    - Default is localhost (127.0.0.1)
 *    - For local testing: Keep default
 *    - For network transmission: Replace with target machine's IP
 *    
 *    Example network configurations:
 *    - Home network: ip_address=192.168.1.100
 *    - Remote server: ip_address=10.0.0.1
 * 
 * 2. Port Configuration:
 *    - Default port is 9090
 *    - Choose a port that is:
 *      a) Not used by other applications
 *      b) Allowed through your firewall
 *    
 *    Example alternative ports:
 *    - Local testing: port=9092
 *    - Specific application: port=8080
 * 
 * 3. Modifying Configuration:
 *    - Recommended to use a separate config file
 *    - Allows changing settings without recompiling
 *    - Load configuration using load_udp_config() function
 */

#ifndef UDP_CONFIG_H
#define UDP_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Default configuration for local testing
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 9090

/**
 * Structure to hold UDP connection configuration
 */
typedef struct {
    char ip_address[64];
    int port;
} UDPConfig;

/**
 * Initialize UDP configuration with default values
 * 
 * @param config Pointer to UDPConfig structure to initialize
 */
void init_udp_config(UDPConfig *config) {
    strncpy(config->ip_address, DEFAULT_IP, sizeof(config->ip_address) - 1);
    config->ip_address[sizeof(config->ip_address) - 1] = '\0';
    config->port = DEFAULT_PORT;
}

/**
 * Load UDP configuration from file if available
 * 
 * @param config Pointer to UDPConfig structure to update
 * @param filename Name of the configuration file
 * @return 1 if successful, 0 if file not found or invalid
 */
int load_udp_config(UDPConfig *config, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Configuration file %s not found, using default settings.\n", filename);
        return 0;
    }
    
    char line[256];
    char key[64];
    char value[192];
    
    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r')
            continue;
            
        // Parse "key=value" pairs
        if (sscanf(line, "%63[^=]=%191s", key, value) == 2) {
            // Remove whitespace
            char *p = key;
            while (*p) {
                if (*p == ' ' || *p == '\t') {
                    *p = '\0';
                    break;
                }
                p++;
            }
            
            // Check which key we have
            if (strcmp(key, "ip") == 0 || strcmp(key, "ip_address") == 0) {
                strncpy(config->ip_address, value, sizeof(config->ip_address) - 1);
                config->ip_address[sizeof(config->ip_address) - 1] = '\0';
            } else if (strcmp(key, "port") == 0) {
                config->port = atoi(value);
            }
        }
    }
    
    fclose(file);
    return 1;
}

/**
 * Display the current UDP configuration
 * 
 * @param config Pointer to UDPConfig structure to display
 */
void print_udp_config(const UDPConfig *config) {
    printf("UDP Configuration:\n");
    printf("  IP Address: %s\n", config->ip_address);
    printf("  Port: %d\n", config->port);
}

#endif /* UDP_CONFIG_H */
