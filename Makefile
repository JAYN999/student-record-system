# Windows-friendly Makefile (g++ / MinGW)
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET   = StudentRecordSystem
SRCS     = main.cpp Student.cpp StudentManager.cpp Utils.cpp
OBJS     = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: all
	./$(TARGET)

clean:
	del /Q *.o $(TARGET).exe 2>nul || rm -f *.o $(TARGET)
