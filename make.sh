#!/bin/bash

#bison -L c++ --locations --header=src/parser.hh --output=src/parser.cpp pint.y
#flex --c++ -o src/tokens.cpp pint.l
clang-18 -std=c++23 -o bin/pint src/main.cpp
