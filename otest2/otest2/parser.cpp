#define YYDEBUG 1

#include "parser.h"

#include <iostream>

#include "FlexLexer.h"
#include "generator.h"
#include "otest2.tab.hpp"
#include "parsercontext.h"

namespace OTest2 {

void parse(
    std::istream& is_,
    Generator& generator_) {
  /* -- create context */
  ::OTest2::ParserContext context_(&generator_);

  /* -- initialize the lexical analyzer */
  otest2FlexLexer lexan_;
  lexan_.set_debug(0);
  context_.lexan = &lexan_;

  /* -- initialize the syntax parser */
  otest2::parser parser_(&context_);
//  parser_.set_debug_stream(std::cerr);
//  parser_.set_debug_level(4);

  /* -- do the analysis */
  parser_.parse();
}

} /* -- namespace OTest2 */
