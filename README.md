# 📡 DigitalComm-QPSK-UDP: QPSK Modulation and UDP Communication

## 🌐 Project Overview

This repository contains a complete implementation of digital communication techniques focusing on QPSK (Quadrature Phase Shift Keying) modulation and UDP (User Datagram Protocol) transmission. The project demonstrates the entire signal processing pipeline from random data generation to transmission over a network, including visualization of the modulation process.

[![Live Demo](https://img.shields.io/badge/Live%20Demo-View%20Visualization-blue?style=for-the-badge&logo=react)](https://yliu.tech/projects/qpsk-modulation)

The implementation covers:
- Random data generation
- QPSK modulation
- Channel noise simulation
- Various UDP transmission methods
- Interactive visualization

## 📁 Repository Structure

```
DigitalComm-QPSK-UDP/
├── src/
│   ├── modulation/                # QPSK modulation implementations
│   │   ├── QPSK.c                 # Basic QPSK modulation
│   │   ├── random.c               # Random bit generation
│   │   ├── noise.c                # QPSK with noise addition
│   │   └── noise_combo.c          # Combined implementation with complex numbers
│   │
│   ├── networking/                # UDP communication implementations
│   │   ├── Client.c               # Basic UDP client
│   │   ├── UDP_ASCII.c            # ASCII data over UDP
│   │   ├── UDP_float.c            # Float data over UDP
│   │   ├── UDP_padding.c          # UDP with data padding
│   │   └── UDP_final.c            # Complete UDP implementation
│   │
│   └── utils/                     # Utility functions
│       └── float.c                # Float conversion utilities
│
├── web/                           # Web-based visualization
│   └── qpsk-visualization.html    # Self-contained QPSK visualization
│
├── config/                        # Configuration files
│   ├── config.h                   # UDP configuration header
│   └── README.md                  # Configuration guide
│
├── bin/                           # Compiled binaries (not tracked in git)
│   └── .gitignore                 # To keep directory but ignore binaries
│
├── Makefile                       # Build automation
├── README.md                      # This file
└── LICENSE                        # License information
```

## 🚀 Getting Started

### Prerequisites
- GCC compiler for C code
- Web browser for visualization
- Packet Sender (or similar UDP testing tool)
- Basic knowledge of digital communication concepts

### Setup and Installation
1. Clone this repository
   ```bash
   git clone https://github.com/jumpjumptiger007/DigitalComm-QPSK-UDP.git
   cd DigitalComm-QPSK-UDP
   ```

2. Use the Makefile to build all components
   ```bash
   make all
   ```

3. Or build specific components
   ```bash
   # Build just modulation components
   make modulation
   
   # Build networking components
   make networking
   ```

### Configuration Guide

#### Using the Configuration Header

The `config/config.h` provides a comprehensive configuration mechanism for UDP transmission:

1. **IP Address Configuration**
   - Default is localhost (127.0.0.1)
   - Modify for network transmission
   
   Example:
   ```c
   // For home network
   #define DEFAULT_IP "192.168.1.100"
   ```

2. **Port Configuration**
   - Default port is 9090
   - Choose an unused port that's firewall-friendly
   
   Example:
   ```c
   // Alternative port
   #define DEFAULT_PORT 8080
   ```

3. **Dynamic Configuration**
   - Use `load_udp_config()` function to load settings from a file
   - Allows changing configurations without recompiling

4. **Configuration Methods**
   - Inline header configuration
   - Separate configuration file
   - Runtime configuration loading

#### Configuration Examples

Basic configuration in code:
```c
UDPConfig config;
init_udp_config(&config);  // Sets default values
load_udp_config(&config, "custom_config.txt");  // Load custom settings
print_udp_config(&config);  // Verify configuration
```

## 📊 Features

- **Modulation**
  - Generation of random binary data
  - Implementation of QPSK modulation
  - Simulation of channel noise with adjustable Signal-to-Noise Ratios (SNR)
  - Complex number representation of symbols

- **Networking**
  - UDP packet creation and transmission
  - Support for various data formats:
    - ASCII text representation of symbols
    - Binary float data
    - Padded data for specific receiver requirements
  - Client implementation for testing

- **Visualization**
  - Interactive web-based QPSK constellation diagram
  - Adjustable noise levels
  - Real-time bit and symbol visualization
  - Educational tool for understanding digital modulation

## 📖 Complete User Guide

This guide will walk you through using all components of this digital communications laboratory project.

### Step 1: Random Data Generation

The first step in the digital communication pipeline is generating random binary data:

```bash
# Compile the random bit generator
make random
# or manually: gcc -o bin/random src/modulation/random.c

# Run to generate random bits
./bin/random
```

This will output an array of random binary digits (0s and 1s) that serve as the input data for QPSK modulation.

### Step 2: QPSK Modulation

QPSK modulation converts pairs of bits into complex symbols:

```bash
# Compile the QPSK modulator
make qpsk
# or manually: gcc -o bin/qpsk src/modulation/QPSK.c -lm

# Run QPSK modulation
./bin/qpsk
```

This performs the following steps:
1. Generates random bits (or you can provide your own)
2. Groups them into pairs
3. Maps each pair to a QPSK symbol according to the constellation diagram:
   - 00 → (+1/√2, +1/√2)
   - 01 → (-1/√2, +1/√2)
   - 10 → (+1/√2, -1/√2)
   - 11 → (-1/√2, -1/√2)
4. Outputs the real and imaginary parts of each symbol

**Constellation Diagram:**

The QPSK constellation maps 2 bits to one of four possible phase shifts:

```
            Imag
              |
    01 x      |      x 00
              |
    ----------+----------> Real
              |
    11 x      |      x 10
              |
```

### Step 3: Adding Noise

In real-world communications, signals are affected by noise. We simulate this with:

```bash
# Compile the noise simulation
make noise
# or manually: gcc -o bin/noise src/modulation/noise.c -lm

# Run the noise simulation
./bin/noise
```

This adds Gaussian noise to both the real and imaginary components of each symbol. The `NOISE_STD_DEV` constant controls the noise level.

### Step 4: UDP Transmission

Now we can transmit our modulated symbols over UDP:

```bash
# Compile and run a complete implementation
make udp_final
./bin/udp_final
```

Different transmission formats are available:
- ASCII format: `./bin/udp_ascii`
- Float format: `./bin/udp_float`
- Padded format: `./bin/udp_padding`

### Step 5: Visualization

Open the web-based visualization to see QPSK modulation in action:

```bash
# Simply open the HTML file in your browser
open web/qpsk-visualization.html
```

You can also access the live demo online at [https://yliu.tech/DigitalComm-QPSK-UDP](https://yliu.tech/DigitalComm-QPSK-UDP)

The visualization allows you to:
- Generate random data bits
- See the QPSK constellation diagram
- Adjust noise levels and observe the effects
- View tabular data of symbols before and after noise

## 🔧 Advanced Usage

### Modifying Parameters

You can customize the behavior by modifying these constants in the code:

- `BITS_COUNT` - Number of random bits to generate
- `SYMBOLS_COUNT` - Number of QPSK symbols (BITS_COUNT/2)
- `NOISE_STD_DEV` - Standard deviation of noise (controls SNR)
- Port numbers and IP addresses in UDP code

### Combined Implementation with Complex Numbers

```bash
make noise_combo
./bin/noise_combo
```

This version uses a `Complex` struct to handle the real and imaginary parts together.

## 🔍 Troubleshooting

### Compilation Issues
- Ensure you have GCC installed: `gcc --version`
- Include the math library with `-lm` flag when compiling
- Use the Makefile to avoid compilation errors

### UDP Transmission Issues
- Check if your firewall is blocking UDP traffic
- Verify the IP address and port are correctly configured
- Use `netstat -u` to check if the UDP socket is listening

### Visualization Issues
- Ensure your browser supports modern JavaScript
- Try refreshing the page if the visualization doesn't load

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Feel free to check the [issues page](https://github.com/jumpjumptiger007/DigitalComm-QPSK-UDP/issues).

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.
