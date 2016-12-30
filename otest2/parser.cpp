#define YYDEBUG 1

#include "parser.h"

#include <datstr/dstring.h>
#include <iostream>

#include "FlexLexer.h"
#include "generator.h"
#include "otest2.tab.hpp"
#include "parsercontext.h"

namespace OTest2 {

void parse(
    std::istream& is_,
    const dstring& filename_,
    Generator& generator_) {
  /* -- create context */
  ::OTest2::ParserContext context_(filename_, &generator_);

  /* -- initialize the lexical analyzer */
  otest2FlexLexer lexan_;
  lexan_.set_debug(0);
  context_.lexan = &lexan_;

  /* -- initialize the syntax parser */
  otest2::parser parser_(&context_);
//  parser_.set_debug_stream(std::cerr);
//  parser_.set_debug_level(4);

  /* -- do the analysis */
  if(parser_.parse())
    context_.error.rethrow();
}

} /* -- namespace OTest2 */
