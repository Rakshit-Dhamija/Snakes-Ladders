#!/bin/bash

# This script allows you to choose which version of the game to play

echo "===== Snakes and Ladders Game ====="
echo "1. Play console version (text-based, works in any environment)"
echo "2. Try OpenGL version (requires graphics support, may not work in all environments)"
echo "3. Compile/recompile both versions"
echo "4. Exit"
echo

read -p "Enter your choice (1-4): " choice

case $choice in
  1)
    clear
    echo "Starting console version..."
    ./snakes_ladders_console
    ;;
  2)
    echo "Attempting to start OpenGL version..."
    echo "Note: This may not work in all environments due to graphics requirements."
    echo "Press Ctrl+C to exit if it doesn't start properly."
    echo
    sleep 2
    ./snakes_ladders
    ;;
  3)
    echo "Compiling both versions..."
    make clean
    make snakes_ladders || echo "OpenGL version compilation failed!"
    make snakes_ladders_console || echo "Console version compilation failed!"
    echo "Compilation complete."
    ;;
  4)
    echo "Exiting."
    exit 0
    ;;
  *)
    echo "Invalid choice. Please run the script again."
    ;;
esac