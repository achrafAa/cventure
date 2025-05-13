CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Directories
EXERCISES_DIR = exercises
BUILD_DIR = build

.PHONY: all test clean reset

all: test

test: build/cventure
	@echo "Running tests..."
	@./build/cventure

clean:
	rm -rf $(BUILD_DIR)
	rm -f .progress.txt
	find $(EXERCISES_DIR) -name "*.exe" -delete

# Build the Cventure runner
build/cventure: build.c src/common.h src/exercises.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) build.c -o $@

# Helper targets
reset:
	@./build/cventure --reset 