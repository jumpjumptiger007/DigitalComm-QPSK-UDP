# Makefile for DigiComm-Lab

# Compiler settings
CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

# Directories
SRC_DIR = src
MOD_DIR = $(SRC_DIR)/modulation
NET_DIR = $(SRC_DIR)/networking
UTIL_DIR = $(SRC_DIR)/utils
CONFIG_DIR = config
BIN_DIR = bin

# Make sure the bin directory exists
$(shell mkdir -p $(BIN_DIR))

# Default configuration file
CONFIG_FILE = $(CONFIG_DIR)/udp_config.txt

# Targets
.PHONY: all clean modulation networking udp qpsk vis config

# Default target: build everything
all: modulation networking

# Make sure config file exists
config:
	@if [ ! -f $(CONFIG_FILE) ]; then \
		echo "Creating default configuration file..."; \
		mkdir -p $(CONFIG_DIR); \
		echo "# UDP Configuration File" > $(CONFIG_FILE); \
		echo "# This file configures the IP address and port for UDP transmission" >> $(CONFIG_FILE); \
		echo "" >> $(CONFIG_FILE); \
		echo "# IP address (default: 127.0.0.1 for localhost)" >> $(CONFIG_FILE); \
		echo "ip_address=127.0.0.1" >> $(CONFIG_FILE); \
		echo "" >> $(CONFIG_FILE); \
		echo "# Port number" >> $(CONFIG_FILE); \
		echo "port=9090" >> $(CONFIG_FILE); \
	fi

# Build only modulation-related binaries
modulation: $(BIN_DIR)/random $(BIN_DIR)/qpsk $(BIN_DIR)/noise $(BIN_DIR)/noise_combo

# Build only networking-related binaries
networking: config $(BIN_DIR)/udp_ascii $(BIN_DIR)/udp_float $(BIN_DIR)/udp_padding $(BIN_DIR)/udp_final $(BIN_DIR)/client

# Random bit generator
$(BIN_DIR)/random: $(MOD_DIR)/random.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# QPSK modulation
$(BIN_DIR)/qpsk: $(MOD_DIR)/QPSK.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# QPSK with noise
$(BIN_DIR)/noise: $(MOD_DIR)/noise.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Combined noise and QPSK
$(BIN_DIR)/noise_combo: $(MOD_DIR)/noise_combo.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# UDP with ASCII encoding
$(BIN_DIR)/udp_ascii: $(NET_DIR)/UDP_ASCII.c $(UTIL_DIR)/udp_config.h
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# UDP with float data
$(BIN_DIR)/udp_float: $(NET_DIR)/UDP_float.c $(UTIL_DIR)/udp_config.h
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# UDP with padding
$(BIN_DIR)/udp_padding: $(NET_DIR)/UDP_padding.c $(UTIL_DIR)/udp_config.h
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Final UDP implementation
$(BIN_DIR)/udp_final: $(NET_DIR)/UDP_final.c $(UTIL_DIR)/udp_config.h
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Client implementation
$(BIN_DIR)/client: $(NET_DIR)/Client.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Clean up compiled binaries
clean:
	rm -rf $(BIN_DIR)/*

# Shortcuts
qpsk: $(BIN_DIR)/qpsk
	./$(BIN_DIR)/qpsk

noise: $(BIN_DIR)/noise
	./$(BIN_DIR)/noise

udp: $(BIN_DIR)/udp_final
	./$(BIN_DIR)/udp_final

# Run visualization
vis:
	echo "Open web/qpsk-visualization.html in your browser to view the visualization"
