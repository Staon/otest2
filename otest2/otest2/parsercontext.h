#ifndef OTest2OTEST2_PARSERCONTEXT_H_
#define OTest2OTEST2_PARSERCONTEXT_H_

#include <astraios/dstream.h>
#include <datstr/dstring.h>
#include <iostream>
#include <list>

class otest2FlexLexer;

namespace OTest2 {

class Generator;
class SourceMarker;

/**
 * @brief Parser context
 */
class ParserContext {
  public:
    otest2FlexLexer* lexan;
    int linenum;
    int colnum;

    bool catch_block;
    dstrostream block;

    Generator* generator;

  private:
    /* -- string pool */
    enum { PREDEFINED_STRING_SIZE = 16 };
    typedef std::list<dstring> StringPool;
    typedef std::list<dstring *> FreePool;
    StringPool pool;
    FreePool free;

  public:
    explicit ParserContext(
        Generator* generator_);
    ~ParserContext();

    /* -- dstring allocation pool */
    dstring* allocateString(
        int length_,
        const char* string_);
    void freeString(
        dstring* string_);
    void clearPool();

    /* -- lexical analyzer interface */
    void incCol(
        int delta_);
    void incLine();

    /* -- catching of free blocks */
    void startCatching();
    dstring* stopCatching();

    /* -- generator interface */
    void dumpString(
        const dstring& string_);
    void enterSuite(
        const dstring& suite_);
    void enterCase(
        const dstring& case_);
    void enterState(
        const dstring& state_);
    void leaveState();
    void leaveCase();
    void leaveSuite();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_PARSERCONTEXT_H_ */
