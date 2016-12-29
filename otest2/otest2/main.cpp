#include <datstr/dstring.h>
#include <iostream>
#include <oexception/exception.h>

#include "generatorstd.h"
#include "parser.h"

int main(
    int argc_,
    char* argv_[]) {
  try {
    ::OTest2::GeneratorStd generator_(&std::cout);
    ::OTest2::parse(std::cin, "stdin", generator_);
  }
  catch(OException& exc_) {
    std::cerr << exc_.reason() << std::endl;
    return -1;
  }
  catch(...) {
    std::cerr << "Unknown error" << std::endl;
    return -1;
  }

  return 0;
}
