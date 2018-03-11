INCLUDE_DIRS=include Catch2/single_include/
INC_PARAMS=$(foreach d, $(INCLUDE_DIRS), -I$d)
CFLAGS=-c -Wall -std=c++11 $(INC_PARAMS) -fprofile-arcs -ftest-coverage
LDFLAGS=-lgcov --coverage
SRC_DIR=test
SRC=tests-main.cpp
SOURCE_FILES=$(foreach s, $(SRC), $(SRC_DIR)/$s)
OBJECTS=$(SOURCE_FILES:.cpp=.o)
OBJ_DIR=obj
EXECUTABLE=runtests

all: $(SOURCE_FILES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@

.PHONY: test
test: all
	./$(EXECUTABLE)

.PHONY: clean
clean:
	-rm -f $(EXECUTABLE)
	find . -name "*.gcda" -print0 | xargs -0 rm -f
	find . -name "*.gcno" -print0 | xargs -0 rm -f
	find . -name "*.o" -print0 | xargs -0 rm -f

HEADER_FILES=$(shell find include/ -type f -name '*.h')

.PHONY: format
format:
	$(foreach file,$(SOURCE_FILES),clang-format -i -style=file $(file);)
	$(foreach file,$(HEADER_FILES),clang-format -i -style=file $(file);)