#ifndef OTest2OTEST2_PARSERCONTEXT_H_
#define OTest2OTEST2_PARSERCONTEXT_H_

#include <astraios/dstream.h>
#include <datstr/dstring.h>
#include <iostream>
#include <list>

#include "declaration.h"
#include "excparser.h"
#include "position.hh"

class otest2FlexLexer;

namespace otest2 {

class location;

} /* -- namespace otest2 */

namespace OTest2 {

class Generator;
class SourceMarker;

/**
 * @brief Parser context
 */
class ParserContext {
  public:
    otest2FlexLexer* lexan;
    dstring filename;
    bool catch_block;
    dstrostream block;
    Generator* generator;
    ParserException error;

  private:
    /* -- string pool */
    enum { PREDEFINED_STRING_SIZE = 16 };
    typedef std::list<dstring> StringPool;
    typedef std::list<dstring *> FreePool;
    StringPool pool;
    FreePool free;

  public:
    explicit ParserContext(
        const dstring& filename_,
        Generator* generator_);
    ~ParserContext();

    /* -- printing */
    dstring printString(
        const dstring* string_) const;

    /* -- dstring allocation pool */
    dstring* allocateString(
        int length_,
        const char* string_);
    void freeString(
        dstring* string_);
    void clearPool();

    /* -- lexical analyzer interface */
    int returnToken(
        int token_,
        int yylen_,
        otest2::location* yyloc_) const;

    /* -- catching of free blocks */
    void startCatching();
    dstring* stopCatching();

    /* -- generator interface */
    void beginFile();
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
    void appendVariable(
        const dstring& name_,
        const DeclarationPtr& declaration_);
    void finishDeclarations();
    bool setInitializer(
        const dstring& variable_,
        const dstring& body_);
    void setCtorBody(
        const dstring& body_);
    void setDtorBody(
        const dstring& body_);
    void setError(
        const dstring& message_,
        const otest2::location& loc_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_PARSERCONTEXT_H_ */
