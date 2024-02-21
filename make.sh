#!/bin/bash

npm run prepare
clang-18 -std=c++17 -o bin/pint src/main.cpp -lstdc++
