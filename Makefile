# === Project Configuration ===
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g -D_POSIX_C_SOURCE=200809L #-DDEBUG

# === Paths ===
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

# === Files ===
# C source files to compile
SRCS = src/pingpong-scheduler.c \
	   src/ppos-core-aux.c

# Corresponding .o files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Precompiled object files (never touched)
PRECOMPILED_OBJS = obj/ppos-all.o \
				   obj/queue.o

# Output executable
TARGET = $(BIN_DIR)/main

# === Rules ===
.PHONY: all clean

all: $(TARGET)

# Link step
$(TARGET): $(OBJS) $(PRECOMPILED_OBJS) Makefile
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(PRECOMPILED_OBJS)

# Compile .c -> .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled (but NOT precompiled) files
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGET)