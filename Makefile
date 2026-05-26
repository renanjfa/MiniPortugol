TARGET = portugol
SRC = portugol.c++
CXX = g++

CXXFLAGS = -Wall -g

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)