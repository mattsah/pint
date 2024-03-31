#!/bin/bash

bison pint.cpp.bnf --feature=syntax-only -Wcounterexamples -Werror=conflicts-rr -Werror=conflicts-sr

if [ $? -eq 0 ]; then
	npx syntax-cli -l pint.cpp.lex -g pint.cpp.bnf -m lalr1 -o src/parser.h

	if [ $? -eq 0 ]; then
		clang-18 -g -O0 -std=c++23 -o bin/pint src/main.cpp -lstdc++
	fi
fi
