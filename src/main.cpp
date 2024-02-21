#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include "compiler.h"
#include "parser.h"

int main(const int argc, const char* argv[])
{
   pint::Compiler pc;
   std::ifstream fd(argv[1]);
   std::stringstream buffer;

   buffer << fd.rdbuf();

   pc.parse(buffer.str());

   return 0;
}