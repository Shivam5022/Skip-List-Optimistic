CXX = g++
CXXFLAGS = -std=c++11 -Wall -lpthread
SRC = main.cpp 
TARGET = prog

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run:$(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)