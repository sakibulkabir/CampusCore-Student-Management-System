CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
SOURCES = src/main.cpp src/StudentManager.cpp src/ConsoleUI.cpp
TARGET = CampusCore

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
