CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = -lGL -lGLU -lglut -lm

# Main targets
all: snakes_ladders

# OpenGL version
snakes_ladders: main.cpp
	$(CXX) $(CXXFLAGS) -o snakes_ladders main.cpp $(LDFLAGS)

# Console version
snakes_ladders_console: snakes_ladders_console.cpp
	$(CXX) $(CXXFLAGS) -o snakes_ladders_console snakes_ladders_console.cpp

# Clean up
clean:
	rm -f snakes_ladders snakes_ladders_console

# Run the OpenGL version
run: snakes_ladders
	./snakes_ladders

# Run the console version
run-console: snakes_ladders_console
	./snakes_ladders_console