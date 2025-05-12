# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Source and object files
SRC = main.c utils.c
OBJ = $(SRC:.c=.o)
EXEC = my_program

# Test files
TEST_SRC = test.c utils.c
TEST_EXEC = test_program

.PHONY: all clean test

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the test
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Build test executable
$(TEST_EXEC): $(TEST_SRC:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^

# Clean build artifacts
clean:
	rm -f $(OBJ) $(EXEC) $(TEST_EXEC) *.o
