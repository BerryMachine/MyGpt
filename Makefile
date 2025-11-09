CC := gcc
BIN := test
BUILD_DIR := build

OMP_PATH := $(shell brew --prefix libomp)

CFLAGS := -ggdb3
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Os 
CFLAGS += -std=c99

LDFLAGS := -ggdb3
LDFLAGS += -Os

LDFLAGS += -L$(OMP_PATH)/lib  # <-- FIX: Tell the linker WHERE to find the library

# --- CPPFLAGS ---
# These are flags for the C-Preprocessor (handles #include)
INCLUDE_DIRS := -I$(OMP_PATH)/include
CPPFLAGS := $(INCLUDE_DIRS) 

SRCS := src/test.c src/maths.c

# This line automatically generates the .o file paths
# e.g., 'test.c' -> 'build/test.o'
# e.g., 'src/maths.c' -> 'build/src/maths.o'
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)


.PHONY: all
all: $(BUILD_DIR)/$(BIN)

# Rule to link the executable
$(BUILD_DIR)/$(BIN): $(OBJS)
	mkdir -p $(@D)
	$(CC) $^ $(LDFLAGS) -o $@

# This pattern rule is smart enough to find 'test.c' and 'src/maths.c'
# and put their .o files in the correct build directory.
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN)

	