gethin
======
A small (300 sloc) cross-platform header-only c++11 getopt library

[![Build Status](https://travis-ci.org/MattiasLiljeson/gethin.svg?branch=master)](https://travis-ci.org/MattiasLiljeson/gethin)
[![codecov](https://codecov.io/gh/MattiasLiljeson/gethin/branch/master/graph/badge.svg)](https://codecov.io/gh/MattiasLiljeson/gethin)

Usage
-----
There are three types of options you can use: `String`, `Flag` and `Set`.

1. Create the options you need.
2. Create a OptionReader object. Supply the options to the constructor.
3. Call the read method on the OptionReader object with argc and argv. 
4. Call the value method on the options to get the parsed arguments.

Example:

```c++
#include <iostream>
#include "gethin.hpp"

void drawRectangle(int width, int height);
void drawEllipse(int width, int height);

int main(int argc, char* argv[]) {
  gethin::String width = gethin::String()
                             .shortOpt('x')
                             .longOpt("width")
                             .mandatory()
                             .name("width")
                             .help("The width of the drawn shape.");

  gethin::String height = gethin::String()
                              .shortOpt('y')
                              .longOpt("height")
                              .mandatory()
                              .name("height")
                              .help("The height of the shape.");

  gethin::Flag color = gethin::Flag().shortOpt('c').longOpt("colors").help(
      "Draw the shape with colored output.");

  gethin::Set shape = gethin::Set()
                          .shortOpt('s')
                          .longOpt("shape")
                          .mandatory()
                          .alternatives({"rectangle", "ellipse"})
                          .help("Which shape to draw.");

  gethin::OptionReader optReader({&width, &height, &color, &shape});
  try {
    optReader.read(argc, argv);
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Error during execution!" << std::endl;
  }

  if (shape.value() == "rectangle") {
    drawRectangle(std::stoi(width.value()), std::stoi(height.value()));
  } else if (shape.value() == "ellipse") {
    drawEllipse(std::stoi(width.value()), std::stoi(height.value()));
  }
}
```

Example
-------
An example is included in the example folder.
It is built with the make target all.
To build it, just run the command `make`.
Execute the example by running `./drawshape`. 

Tests
-----
The unit tests can be found in `test/`.
The test framework [Catch2](https://github.com/catchorg/Catch2) is used to write and execute the tests.
Catch2 is included as a submodule in this repo.
To be able to build the test, the Catch2 submodule must be present.
Either clone the repo with the switch `--recurse-submodules`,
or run the command `git submodule init && git submodule update`.
The unit tests can be built and executed by running `make test`.
They document the usage and the (few) caveats of this lib.

As we say in swedish, "Mycket nöje!"
