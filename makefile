INCLUDE_DIRS=include Catch2/single_include/
INC_PARAMS=$(foreach d, $(INCLUDE_DIRS), -I$d)
CFLAGS=-c -Wall -std=c++11 $(INC_PARAMS) -fprofile-arcs -ftest-coverage
LDFLAGS=-lgcov --coverage

TEST_SOURCE_FILES=test/tests-main.cpp test/optionalTest.cpp
TEST_OBJECTS=$(TEST_SOURCE_FILES:.cpp=.o)
TEST_EXECUTABLE=runtests

EXAMPLE_SOURCE_FILES=example/drawshape.cpp
EXAMPLE_OBJECTS=$(EXAMPLE_SOURCE_FILES:.cpp=.o)
EXAMPLE_EXECUTABLE=drawshape

all: $(TEST_SOURCE_FILES) $(TEST_EXECUTABLE) $(EXAMPLE_SOURCE_FILES) $(EXAMPLE_EXECUTABLE)
    
$(TEST_EXECUTABLE): $(TEST_OBJECTS) 
	$(CXX) $(LDFLAGS) $(TEST_OBJECTS) -o $@

$(EXAMPLE_EXECUTABLE): $(EXAMPLE_OBJECTS) 
	$(CXX) $(LDFLAGS) $(EXAMPLE_OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@

.PHONY: test
test: all
	./$(TEST_EXECUTABLE)

.PHONY: clean
clean:
	-rm -f $(TEST_EXECUTABLE)
	-rm -f $(EXAMPLE_EXECUTABLE)
	find . -name "*.gcda" -print0 | xargs -0 rm -f
	find . -name "*.gcno" -print0 | xargs -0 rm -f
	find . -name "*.o" -print0 | xargs -0 rm -f

HEADER_FILES=$(shell find include/ -type f -name '*.hpp')

.PHONY: format
format:
	$(foreach file,$(TEST_SOURCE_FILES),clang-format -i -style=file $(file);)
	$(foreach file,$(EXAMPLE_SOURCE_FILES),clang-format -i -style=file $(file);)
	$(foreach file,$(HEADER_FILES),clang-format -i -style=file $(file);)