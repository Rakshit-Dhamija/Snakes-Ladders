#!/bin/bash

# Create a directory for the GitHub repository
mkdir -p snakes_and_ladders_game

# Copy essential files
cp main.cpp snakes_ladders_console.cpp Makefile play.sh \
   run_opengl_game.sh run_game.sh README.md .gitignore \
   snakes_and_ladders_game/

# Make scripts executable in the new directory
chmod +x snakes_and_ladders_game/*.sh

echo "Files have been copied to the 'snakes_and_ladders_game' directory."
echo "You can now create a GitHub repository with these files."
echo 
echo "Here's a guide to create your GitHub repository:"
echo "1. Go to https://github.com/new"
echo "2. Name your repository 'Snakes-and-Ladders-CPP' or similar"
echo "3. Add a short description like: '2D Snakes and Ladders game in C++ with OpenGL/FreeGLUT'"
echo "4. Set it to Public if you want to share it"
echo "5. Initialize with a README if you want (optional, we already have one)"
echo "6. Create the repository"
echo "7. Follow GitHub's instructions to push your local repository"
echo
echo "Basic Git commands to use:"
echo "  git init"
echo "  git add ."
echo "  git commit -m 'Initial commit'"
echo "  git remote add origin YOUR_GITHUB_REPO_URL"
echo "  git push -u origin main"