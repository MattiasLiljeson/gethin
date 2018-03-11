
CC=g++
INCLUDE_DIRS=include Catch2/single_include/
INC_PARAMS=$(foreach d, $(INCLUDE_DIRS), -I$d)
CFLAGS=-c -Wall -std=c++11 $(INC_PARAMS)
LDFLAGS=
SRC_DIR=test
SRC=tests-main.cpp
SOURCES=$(foreach s, $(SRC), $(SRC_DIR)/$s)
OBJECTS=$(SOURCES:.cpp=.o)
OBJ_DIR=obj
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
	-rm -f $(SRC_DIR)/*.o $(EXECUTABLE)