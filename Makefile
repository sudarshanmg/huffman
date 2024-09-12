# Define the compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Target executable name
TARGET = huffman_compress

# Object files
OBJS = main.o huffman.o

# Default rule: compile the program
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile main.cpp into main.o
main.o: main.cpp huffman.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Rule to compile huffman.cpp into huffman.o
huffman.o: huffman.cpp huffman.hpp
	$(CXX) $(CXXFLAGS) -c huffman.cpp

# Rule to clean up generated files (object files and the executable)
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets (to avoid conflicts with files named "clean" or "all")
.PHONY: all clean

