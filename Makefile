.PHONY: clean debug run

CXX = g++
CXXFLAGS = -std=c++11 -march=native -O3
LDFLAGS =

SOURCES = checkers.cpp board.cpp game.cpp moves.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = checkers.exe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) debug.exe *.stackdump *~ *.dSYM/

debug:
	$(CXX) $(CXXFLAGS) -g -o debug.exe $(SOURCES)

run:
	./$(EXECUTABLE)
