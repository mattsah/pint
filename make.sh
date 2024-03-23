#!/bin/bash

#bison pint.cpp.bnf --feature=syntax-only -Wcounterexamples

if [ $? -eq 0 ]; then
	#npm run prepare
	clang-18 -g -O0 -std=c++23 -o bin/pint src/main.cpp -lstdc++
fi
