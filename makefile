
CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SOURCES=tests-main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=runtests

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: test
test: all
	./$(EXECUTABLE)

.PHONY: clean
clean:
	-rm -f *.o $(EXECUTABLE)