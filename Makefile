# --- Unified Makefile (for Mac and Docker/Linux) ---
#
# This Makefile automatically detects the OS.
# How to run: just type 'make'
#
# On Mac, 'brew install libomp' and 'brew install llvm' are required.
#
# Auto-detect OS: "Darwin" (macOS) or "Linux"
UNAME_S := $(shell uname -s)

# --- Environment-Specific Configuration ---
ifeq ($(UNAME_S), Darwin)
	# --- macOS 'clang' Configuration ---
	
	# Find Homebrew LLVM (which includes clang+openmp)
	LLVM_PATH := $(shell brew --prefix llvm 2>/dev/null)
	
	# Find libomp (runtime library)
	OMP_PATH := $(shell brew --prefix libomp 2>/dev/null)
	
	# Check if brew exists and packages are installed
	ifeq ($(LLVM_PATH),)
		$(error Cannot find 'llvm'. Please run 'brew install llvm' first.)
	endif
	ifeq ($(OMP_PATH),)
		$(error Cannot find 'libomp'. Please run 'brew install libomp' first.)
	endif

	# Use the 'clang' from the brew-installed LLVM
	CC := $(LLVM_PATH)/bin/clang
	
	# Flags for macOS
	CPPFLAGS := -I$(OMP_PATH)/include
	# Add libomp's lib path
	LDFLAGS := -L$(OMP_PATH)/lib -lomp
	# --- FIX: Removed quotes from variable ---
	ENV_MSG := (for macOS)
else
	# --- Docker 'gcc' Configuration (Default for Linux) ---
	CC := gcc
	CPPFLAGS :=
	LDFLAGS :=
	# --- FIX: Removed quotes from variable ---
	ENV_MSG := (for Docker/Linux)
endif

# --- Common Project Settings ---
BIN := test
BUILD_DIR := build

# --- Common CFLAGS (for both builds) ---
CFLAGS := -ggdb3
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Os 
CFLAGS += -std=c99
CFLAGS += -fopenmp
CFLAGS += -march=native

# --- Common LDFLAGS (for both builds) ---
# We add the environment-specific LDFLAGS (like -lomp) first
LDFLAGS += -ggdb3
LDFLAGS += -Os
LDFLAGS += -fopenmp

SRCS := src/test.c src/maths.c src/tokenizer.c
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(BUILD_DIR)/$(BIN)

# Rule to link the executable
$(BUILD_DIR)/$(BIN): $(OBJS)
	mkdir -p $(@D)
	$(CC) $^ $(LDFLAGS) -o $@
	@echo "Linked executable $(ENV_MSG): $@"

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	@echo "Compiled object $(ENV_MSG): $@"

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN)