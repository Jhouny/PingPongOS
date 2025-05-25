# === Project Configuration ===
CC := gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -g -D_POSIX_C_SOURCE=200809L #-DDEBUG

# === Paths ===
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# === Files ===
# C source files to compile
BASE_SRCS := $(SRC_DIR)/ppos-core-aux.c

# === Source sets for each target ===
SCHEDULER_SRCS := $(BASE_SRCS) $(SRC_DIR)/pingpong-scheduler.c
PREEMPCAO_SRCS := $(BASE_SRCS) $(SRC_DIR)/pingpong-preempcao.c
CONTAB_SRCS    := $(BASE_SRCS) $(SRC_DIR)/pingpong-contab.c

# === Object files for each target ===
SCHEDULER_OBJS := $(SCHEDULER_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
PREEMPCAO_OBJS := $(PREEMPCAO_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CONTAB_OBJS    := $(CONTAB_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# === Binaries ===
SCHEDULER_BIN := $(BIN_DIR)/scheduler
PREEMPCAO_BIN := $(BIN_DIR)/preempcao
CONTAB_BIN    := $(BIN_DIR)/contab

# Precompiled object files (never touched)
PRECOMPILED_OBJS := obj/ppos-all.o \
				   obj/queue.o

# === Rules ===
.PHONY: all clean scheduler preempcao contab

# === Main build entry ===
all: scheduler preempcao contab

scheduler: $(SCHEDULER_BIN)
preempcao: $(PREEMPCAO_BIN)
contab: $(CONTAB_BIN)

# === Build binaries ===
$(SCHEDULER_BIN): $(SCHEDULER_OBJS) $(PRECOMPILED_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	
$(PREEMPCAO_BIN): $(PREEMPCAO_OBJS) $(PRECOMPILED_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(CONTAB_BIN): $(CONTAB_OBJS) $(PRECOMPILED_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# === Compile .c -> .o ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# === Clean rule ===
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*