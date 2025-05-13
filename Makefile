CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Directories
EXERCISES_DIR = exercises
BUILD_DIR = build
HINTS_DIR = hints

.PHONY: all test clean reset chapter hard normal config-hard config-normal

all: test

test: build/cventure
	@echo "Running tests..."
	@./build/cventure

# Run exercises for a specific chapter
chapter: build/cventure
	@echo "Running exercises for chapter $(chapter)..."
	@./build/cventure --chapter=$(chapter)

# Run exercises in hard mode (no hints)
hard: build/cventure
	@echo "Running exercises in hard mode (no hints)..."
	@./build/cventure --hard

# Run exercises in normal mode (with hints)
normal: build/cventure
	@echo "Running exercises in normal mode (with hints)..."
	@./build/cventure --normal

# Configure hard mode permanently
config-hard: build/cventure
	@echo "Setting hard mode as default in config..."
	@./build/cventure --hard

# Configure normal mode permanently
config-normal: build/cventure
	@echo "Setting normal mode as default in config..."
	@./build/cventure --normal

clean:
	rm -rf $(BUILD_DIR)
	rm -f .progress.txt
	rm -f .attempts.txt
	find $(EXERCISES_DIR) -name "*.exe" -delete

# Build the Cventure runner
build/cventure: build.c src/common.h src/exercises.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) build.c -o $@

# Helper targets
reset:
	@./build/cventure --reset 