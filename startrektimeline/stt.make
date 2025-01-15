# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -I/usr/include
LDFLAGS = -lsqlite3

# Source files
SRC = star_trek_time.c
OBJ = $(SRC:.c=.o)

# Output binary
OUT = stt

# Default target
all: $(OUT)

# Compile the C source file to an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create the final executable
$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUT) $(LDFLAGS)

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(OUT)

# Rebuild everything from scratch
rebuild: clean all

