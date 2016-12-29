#include "parsercontext.h"

#include <astraios/dstream.h>
#include <cctype>
#include <iostream>
#include <ondrart/oassert.h>

#include "generator.h"
#include "location.hh"

namespace OTest2 {

ParserContext::ParserContext(
    const dstring& filename_,
    Generator* generator_) :
  lexan(0),
  filename(filename_),
  catch_block(false),
  block(),
  generator(generator_),
  error() {

}

ParserContext::~ParserContext() {
  clearPool();
}

dstring ParserContext::printString(
    const dstring* string_) const {
  OASSERT_1(string_ != 0 && !string_ -> IsNull());

  dstrostream dos_;
  const char* c_(string_ -> Str());
  while(*c_ != 0) {
    if(std::isprint(*c_))
      dos_.put(*c_);
    else
      dos_.put('.');
    ++c_;
  }
  return dos_.str();
}

dstring* ParserContext::allocateString(
    int length_,
    const char* string_) {
  if(free.empty()) {
    /* -- I already have no string allocated, so I must allocate
          one. */
    pool.push_back(dstring());
    if(length_ + 1 < PREDEFINED_STRING_SIZE)
      length_ = PREDEFINED_STRING_SIZE;
    pool.back().GrowAlloc(length_);
    pool.back() = string_;
    return &pool.back();
  }
  else {
    /* -- There is a free string, use it */
    dstring* retval_(free.front());
    *retval_ = string_;
    free.pop_front();
    return retval_;
  }
}

void ParserContext::freeString(
    dstring* string_) {
  free.push_back(string_);
}

void ParserContext::clearPool() {
  free.clear();
  pool.clear();
}

int ParserContext::returnToken(
    int token_,
    int yylen_,
    otest2::location* yyloc_) const {
  yyloc_ -> columns(yylen_);
  return token_;
}

void ParserContext::startCatching() {
  catch_block = true;
}

dstring* ParserContext::stopCatching() {
  catch_block = false;
  return allocateString(block.str().Strlen(), block.str().Str());
}

void ParserContext::beginFile() {
  generator -> beginFile();
}

void ParserContext::dumpString(
    const dstring& string_) {
  if(catch_block)
    block << string_;
  else
    generator -> dumpString(string_);
}

void ParserContext::enterSuite(
    const dstring& suite_) {
  generator -> enterSuite(suite_);
}

void ParserContext::enterCase(
    const dstring& case_) {
  generator -> enterCase(case_);
}

void ParserContext::enterState(
    const dstring& state_) {
  generator -> enterState(state_);
}

void ParserContext::leaveState() {
  generator -> leaveState();
}

void ParserContext::leaveCase() {
  generator -> leaveCase();
}

void ParserContext::leaveSuite() {
  generator -> leaveSuite();
}

void ParserContext::appendVariable(
    const dstring& name_,
    const DeclarationPtr& declaration_) {
  generator -> appendVariable(name_, declaration_);
}

void ParserContext::finishDeclarations() {
  generator -> finishDeclarations();
}

bool ParserContext::setInitializer(
    const dstring& variable_,
    const dstring& body_) {
  return generator -> setInitializer(variable_, body_);
}

void ParserContext::setCtorBody(
    const dstring& body_) {
  generator -> setCtorBody(body_);
}

void ParserContext::setDtorBody(
    const dstring& body_) {
  generator -> setDtorBody(body_);
}

void ParserContext::setError(
    const dstring& message_,
    const otest2::location& loc_) {
  otest2::position last_(loc_.end - 1);
  error.setException(
      message_,
      *loc_.begin.filename,
      loc_.begin.line,
      loc_.begin.column,
      last_.line,
      last_.column);
}

} /* -- namespace OTest */
