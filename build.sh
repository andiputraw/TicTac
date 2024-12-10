#!/bin/bash

# Set compiler
# CC=tcc
# Uncomment the next line if using zig cc instead
CC="gcc"


echo "Compiling..."

# Define input files
INPUT="TicTac.c src/game.c src/board.c src/control.c src/menu.c src/leaderboard.c"

# Define flags, include paths, library paths, and libraries
FLAGS="-Wall -Wextra -g"
I_PATH="-I./src -I./raylib/include"
L_PATH="-L./raylib/lib"
LIBS="-l:libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11"

# Compile the program
$CC $FLAGS -o out/TicTac $I_PATH $INPUT $L_PATH $LIBS

# Check for compilation errors
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

echo "Compilation completed successfully."

# Run the binary
export LD_LIBRARY_PATH=./raylib/lib:$LD_LIBRARY_PATH
echo "Running the binary..."
./out/TicTac
