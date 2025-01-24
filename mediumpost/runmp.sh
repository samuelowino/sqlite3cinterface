#!/bin/bash

# Exit immediately if any command fails
set -e

# Define variables for clarity
MAKEFILE="mp.make"
OBJECT_FILE="userscrud.o"
EXECUTABLE="mp"
LIBRARY="-lsqlite3"

# Function to print and execute a command
execute() {
  echo "Executing: $1"
  eval $1
}

# Step 1: Run the Makefile to compile object files
echo "Step 1: Executing make file $MAKEFILE"
execute "make -f $MAKEFILE"

# Step 2: Link the object file using gcc to create the executable
echo "Step 2: Invoking gcc to link object file $OBJECT_FILE"
execute "gcc $OBJECT_FILE -o $EXECUTABLE $LIBRARY"

# Step 3: Run the generated executable
echo "Step 3: Running the executable program $EXECUTABLE"
execute "./$EXECUTABLE"

# Optional: Clean up generated files (uncomment if needed)
echo "Cleaning up generated files..."
execute "make clean"

echo "Script completed successfully!"
