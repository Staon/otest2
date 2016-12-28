#include "parsercontext.h"

#include <iostream>

#include "generator.h"

namespace OTest2 {

ParserContext::ParserContext(
    Generator* generator_) :
  lexan(0),
  linenum(1),
  colnum(1),
  catch_block(false),
  block(),
  generator(generator_) {

}

ParserContext::~ParserContext() {
  clearPool();
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

void ParserContext::incCol(
    int delta_) {
  colnum += delta_;
}

void ParserContext::incLine() {
  ++linenum;
  colnum = 1;
}

void ParserContext::startCatching() {
  catch_block = true;
}

dstring* ParserContext::stopCatching() {
  catch_block = false;
  return allocateString(block.str().Strlen(), block.str().Str());
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

} /* -- namespace OTest */
