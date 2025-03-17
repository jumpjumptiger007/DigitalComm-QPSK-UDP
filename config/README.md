# UDP Configuration Header

This directory contains the configuration header for UDP transmitters in the DigitalComm-QPSK-UDP project.

## Configuration Header (`config.h`)

The `config.h` header provides a comprehensive configuration mechanism for UDP transmission. It includes detailed documentation and guidance for setting up IP addresses and ports.

## Configuration Guide

### 1. IP Address Configuration

- **Default**: Localhost (127.0.0.1)
- For local testing: Keep the default configuration
- For network transmission: Replace with the target machine's IP

Example network configurations:
- Home network: `ip_address=192.168.1.100`
- Remote server: `ip_address=10.0.0.1`

### 2. Port Configuration

- **Default Port**: 9090
- Choose a port that is:
  a) Not used by other applications
  b) Allowed through your firewall

Example alternative ports:
- Local testing: `port=9092`
- Specific application: `port=8080`

### 3. Modifying Configuration

Recommended approaches:
- Use a separate configuration file
- Allows changing settings without recompiling
- Use `load_udp_config()` function to load configurations

## Inline Configuration Comments

The `config.h` header provides extensive inline comments to guide users on configuration options, best practices, and potential use cases.

## Using the Configuration

1. Include the header in your UDP transmission code
2. Use `init_udp_config()` to set default values
3. Use `load_udp_config()` to load custom configurations
4. Use `print_udp_config()` to verify settings
