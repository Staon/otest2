#include <exception>
#include <iostream>
#include <string>

#include "exc.h"
#include "parser.h"

int main(
    int argc_,
    char* argv_[]) {
  try {
    ::OTest2::parse("example.cpp");
  }
  catch(::OTest2::Exception& exc_) {
    std::cerr << exc_.reason() << std::endl;
    return -1;
  }
  catch(std::exception& exc_) {
    std::cerr << exc_.what() << std::endl;
    return -1;
  }
  catch(...) {
    std::cerr << "Unknown error" << std::endl;
    return -1;
  }

  return 0;
}
