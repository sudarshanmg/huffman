
CXX = g++

CXXFLAGS = -std=c++11 -Wall

TARGET = huffman_compress

OBJS = main.o huffman.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp huffman.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

huffman.o: huffman.cpp huffman.hpp
	$(CXX) $(CXXFLAGS) -c huffman.cpp

clean:
	rm -f $(TARGET) $(OBJS)


