#include <iostream>

#include "generatorstd.h"
#include "parser.h"

int main(
    int argc_,
    char* argv_[]) {
  ::OTest2::GeneratorStd generator_(&std::cout);
  ::OTest2::parse(std::cin, generator_);

  return 0;
}
