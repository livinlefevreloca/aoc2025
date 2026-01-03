# --- Configuration Variables ---

# Compiler
CC = cc

PREFIX          := /usr/local
INSTALL_LIB_DIR := $(PREFIX)/lib

# Compiler Flags for both modes (essential flags)
CFLAGS_COMMON = -Iinclude -Wall -Wextra -pedantic

# Debug Flags
CFLAGS_DEBUG = $(CFLAGS_COMMON) -g -DDEBUG

# Optimization Flags
CFLAGS_OPTIMIZED = $(CFLAGS_COMMON) -O3

# 2. Linker Flags (Library paths)
LDFLAGS = -L$(INSTALL_LIB_DIR) -Wl,-rpath,$(INSTALL_LIB_DIR)

LDLIBS = -lm

# Executable name
TARGET = aoc

# Directories
SRC_DIR = .
LIB_SRC_DIR = lib
DATA_DIR = data

# Source Files
# Automatically find all .c files in the current directory (for dayX.c and main.c)
# Exclude test files (test_*.c) from the normal build
SRC_FILES = $(filter-out $(SRC_DIR)/test_%.c, $(wildcard $(SRC_DIR)/*.c))

# Object Files - Change .c to .o and keep the directory structure
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,%.o,$(SRC_FILES))
# Filter out the main.c's object file from the OBJ_FILES list (optional, but clean)
# We will explicitly list all objects needed for linking

# The list of all .o files needed for the final link
ALL_OBJECTS = $(patsubst %.c,%.o,$(notdir $(filter %.c, $(SRC_FILES))))

# --- Default Target ---
.PHONY: all
all: optimized

# --- Optimized Build (Default) ---
.PHONY: optimized
optimized: CFLAGS = $(CFLAGS_OPTIMIZED)
optimized: $(TARGET)
	@echo "✨ Built $(TARGET) in optimized mode."

# --- Debug Build ---
.PHONY: debug
debug: CFLAGS = $(CFLAGS_DEBUG)
debug: $(TARGET)
	@echo "🐛 Built $(TARGET) in debug mode."

# --- Profiling Build ---
.PHONY: perf
perf: CFLAGS = $(CFLAGS_COMMON) -O2 -g -fno-omit-frame-pointer
perf: $(TARGET)
	@echo "📊 Built $(TARGET) in profiling mode."

# --- Linkage Rule ---
# Link all object files to create the final executable
$(TARGET): $(patsubst %.c,%.o,$(filter %.c, $(SRC_FILES)))
	$(CC) $(LDFLAGS) $^ $(LDLIBS)  -o $@

# --- Compilation Rules ---

# Rule for compiling C files in the root directory (main.c, dayX.c)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- Cleanup ---

.PHONY: clean
clean:
	@echo "🧹 Cleaning up..."
	rm -f $(TARGET) *.o lib/*.o

# --- Test ---

.PHONY: test
test: test_utils.c util.c
	$(CC) $(CFLAGS_DEBUG) test_utils.c util.c -o test
	./test

.PHONY: test-perf
test-perf: test_utils.c util.c
	$(CC) $(CFLAGS_COMMON) -O3 test_utils.c util.c -o test
	./test
