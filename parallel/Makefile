# Compiler and flags
CC = smpicc
CFLAGS = -Wall -Wextra -Werror -Iinclude -O3
LDFLAGS = -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
IMAGES_DIR = images
CONFIG_DIR = config
UTILS_DIR = $(SRC_DIR)/utils

# Definition
OUTPUT_NAME = julia_mpi

# Output binary name
TARGET = $(BUILD_DIR)/$(OUTPUT_NAME)

# Source files
SRCS = $(SRC_DIR)/1D_parallel_julia.c \
       $(SRC_DIR)/compute_julia_pixel.c \
       $(SRC_DIR)/write_bmp_header.c \
	   $(SRC_DIR)/write_partial_bmp.c \
	   $(UTILS_DIR)/display_progress.c

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(filter-out $(UTILS_DIR)/%.c, $(SRCS))) \
       $(patsubst $(UTILS_DIR)/%.c, $(BUILD_DIR)/utils/%.o, $(filter $(UTILS_DIR)/%.c, $(SRCS)))

# Default target: Build the program
.PHONY: all
all: $(TARGET)

# Build the target binary
$(TARGET): $(OBJS) | $(BUILD_DIR)
	@echo "Linking objects into binary..."
	docker run --rm -it -v $(shell pwd):/home/smpi --user $(shell id -u):$(shell id -g) henricasanova/ics632_smpi $(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete. Run './run.sh <np> <N>' to execute the program."

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	docker run --rm -it -v $(shell pwd):/home/smpi --user $(shell id -u):$(shell id -g) henricasanova/ics632_smpi $(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/utils/%.o: $(UTILS_DIR)/%.c | $(BUILD_DIR)/utils
	@echo "Compiling utility $<..."
	docker run --rm -it -v $(shell pwd):/home/smpi --user $(shell id -u):$(shell id -g) henricasanova/ics632_smpi $(CC) $(CFLAGS) -c $< -o $@

# Create the build directories if they don't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/utils:
	@mkdir -p $(BUILD_DIR)/utils

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning up build files..."
	@rm -rf $(BUILD_DIR)/* $(IMAGES_DIR)/*
	@echo "Clean complete."
