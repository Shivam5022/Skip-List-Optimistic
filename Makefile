CXX = g++
CXXFLAGS = -std=c++20 -Wall
SRC = main.cpp 
TARGET = prog

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run:$(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)