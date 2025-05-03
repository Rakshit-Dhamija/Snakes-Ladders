#!/bin/bash

echo "Building Snakes and Ladders Game..."

# Compile the console version
echo "Compiling console version..."
g++ -std=c++11 -o snakes_ladders_console snakes_ladders_console.cpp && echo "✓ Console version compiled successfully!"

# Try to compile the OpenGL version, but don't fail if libraries are missing
echo "Attempting to compile OpenGL version..."
g++ -std=c++11 -o snakes_ladders main.cpp -lGL -lGLU -lglut -lm && echo "✓ OpenGL version compiled successfully!" || echo "✗ Could not compile OpenGL version (missing libraries)"

echo
echo "Build complete. To run the game:"
echo "1. Run the console version: ./run_game.sh"
echo "2. Run the OpenGL version (if compiled): ./run_opengl_game.sh"
echo "3. Use the menu: ./play.sh"