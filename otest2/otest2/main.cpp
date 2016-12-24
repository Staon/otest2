#define YYDEBUG 1

#include "FlexLexer.h"
#include "generatorstd.h"
#include "parsercontext.h"
#include "otest2.tab.hpp"

int main(
    int argc_,
    char* argv_[]) {
  /* -- create context */
  ::OTest2::GeneratorStd generator_(&std::cout);
  ::OTest2::ParserContext context_(&generator_);
  otest2FlexLexer lexan_;
  context_.lexan = &lexan_;

  /* -- initialize the lexical analyzer */
//  context_.lexan -> set_debug(1);
//  context_.lexan -> switch_streams(&is_, &os_);

  /* -- do analysis */
  otest2::parser parser_(&context_);
  parser_.set_debug_stream(std::cerr);
  parser_.set_debug_level(4);
  parser_.parse();

  return 0;
}
