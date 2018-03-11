gethin
======
A small (300 sloc) cross-platform header-only c++11 getopt library

[![Build Status](https://travis-ci.org/MattiasLiljeson/gethin.svg?branch=master)](https://travis-ci.org/MattiasLiljeson/gethin)
[![codecov](https://codecov.io/gh/MattiasLiljeson/gethin/branch/master/graph/badge.svg)](https://codecov.io/gh/MattiasLiljeson/gethin)

Usage
-----
There are three types of options you can use: `String`, `Flag` and `Set`.

1. Create the options you need.
2. Create a OptionReader object. Supply the options in a vector.
3. Call the read method on the OptionReader object with argc and argv. 
4. Call the value method on the options to get the parsed arguments.

Example:

```c++
#include "gethin.hpp"

String f = String()
                .shortOpt('f')
                .longOpt("foo")
                .mandatory(false)
                .name("argument name, shown in help")
                .help("some help text about foo");

Flag b = Flag().shortOpt('b').longOpt("bar").help("some help text about bar");

Set pga = Set()
            .shortOpt('a')
            .longOpt("palette-generation-algorithm")
            .mandatory(false)
            .alternatives({"kmeans", "iterative", "histogram", "mixed"})
            .help(
                "Specify 'kmeans', 'iterative', 'histogram' or 'mixed' "
                "where 'mixed' combines the results of 'histogram' and "
                "'iterative'");

OptionReader optReader(vector<Parameter*>{&f, &b, &pga});
```

Tests
-----
The unit tests can be found in `test/`.
They can be executed by running `make test`.
They document the usage and the (few) caveats of this lib.

As we say in swedish, "Mycket nöje!"
