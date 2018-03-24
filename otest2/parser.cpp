#include "parser.h"

#include <clang-c/Index.h>
#include <iostream>
#include <string>

#include "error.h"
#include "excparser.h"
#include "generatorprinter.h"
#include "location.h"

namespace OTest2 {

template<typename Function_>
class ScopeGuard {
  private:
    Function_ function;

  public:
    ScopeGuard(
        Function_ function_) :
      function(function_) {

    }

    ~ScopeGuard() {
      function();
    }
};

template<typename Function_>
ScopeGuard<Function_> scopeGuard(
    Function_ function_) {
  return ScopeGuard<Function_>(function_);
}

void throwClangError(
    const std::string& msgbase_,
    CXErrorCode info_) {
  std::string message_(msgbase_);
  message_ += ": ";
  switch(info_) {
    case CXError_Failure:
      message_ += "generic failure";
      break;
    case CXError_Crashed:
      message_ += "crashed";
      break;
    case CXError_InvalidArguments:
      message_ += "invalid arguments";
      break;
    case CXError_ASTReadError:
      message_ += "AST read error";
      break;
    default:
      message_ += "unknown error";
      break;
  }

  throw Error(message_);
}

std::string getClangString(
    CXString str_) {
  std::string retval_(clang_getCString(str_));
  clang_disposeString(str_);
  return retval_;
}

Location createLocation(
    CXSourceLocation loc_) {
  unsigned int line_;
  unsigned int column_;
  clang_getSpellingLocation(loc_, nullptr, &line_, &column_, nullptr);
  return Location(line_, column_);
}

struct ParserContext {
    /* -- error reporting */
    bool failure;
    ParserException exception;

    /* -- output generation */
    Generator* generator;
    Location last_location;

    explicit ParserContext(
        Generator* generator_);

    void setError(
        const std::string& message_,
        CXCursor cursor_);
    void copyInput(
        CXCursor cursor_,
        bool end_ = false);
    void moveToEnd(
        CXCursor cursor_);
};

ParserContext::ParserContext(
    Generator* generator_) :
  failure(false),
  exception(),
  generator(generator_),
  last_location() {

}

void ParserContext::setError(
    const std::string& message_,
    CXCursor cursor_) {
  /* -- get error location */
  CXSourceRange range_(clang_getCursorExtent(cursor_));
  CXSourceLocation begin_(clang_getRangeStart(range_));
  CXFile file_;
  unsigned int beg_line_;
  unsigned int beg_column_;
  unsigned int offset_;
  clang_getSpellingLocation(begin_, &file_, &beg_line_, &beg_column_, nullptr);
  std::string filename_(getClangString(clang_getFileName(file_)));
  CXSourceLocation end_(clang_getRangeEnd(range_));
  unsigned int end_line_;
  unsigned int end_column_;
  clang_getSpellingLocation(begin_, nullptr, &end_line_, &end_column_, nullptr);

  /* -- fill the exception */
  failure = true;
  exception.setException(
      message_,
      filename_,
      beg_line_,
      beg_column_,
      end_line_,
      end_column_);
}

void ParserContext::copyInput(
    CXCursor cursor_,
    bool end_) {
  CXSourceRange range_(clang_getCursorExtent(cursor_));
  CXSourceLocation loc_;
  if(!end_)
    loc_ = clang_getRangeStart(range_);
  else
    loc_ = clang_getRangeEnd(range_);
  Location l2_(createLocation(loc_));

  if(last_location < l2_)
    generator->copySource(last_location, l2_);
}

void ParserContext::moveToEnd(
    CXCursor cursor_) {
  CXSourceRange range_(clang_getCursorExtent(cursor_));
  CXSourceLocation end_(clang_getRangeEnd(range_));
  last_location = createLocation(end_);
}

ParserContext* getContext(
    CXClientData udata_) {
  return static_cast<ParserContext*>(udata_);
}

CXChildVisitResult cursorPrinter(
    CXCursor cursor_,
    CXCursor parent_,
    CXClientData udata_) {
  CXString spelling_(clang_getCursorSpelling(cursor_));
  CXString kind_(clang_getCursorKindSpelling(clang_getCursorKind(cursor_)));

  /* -- parse location */
  CXSourceLocation location_(clang_getCursorLocation(cursor_));
  CXFile file_;
  unsigned int line_;
  unsigned int column_;
  unsigned int offset_;
  clang_getSpellingLocation(location_, &file_, &line_, &column_, &offset_);
  CXString filename_(clang_getFileName(file_));
  const char* cfilename_(clang_getCString(filename_));

  std::cout
      << "Cursor '"
      << clang_getCString(spelling_)
      << "' of kind '"
      << clang_getCString(kind_)
      << "' at "
      << (cfilename_ ? cfilename_ : "") << ':' << line_ << ':' << column_
      << std::endl;

  clang_disposeString(filename_);
  clang_disposeString(kind_);
  clang_disposeString(spelling_);

  return CXChildVisit_Recurse;
}

CXChildVisitResult cursorSuiteVariables(
    CXCursor cursor_,
    CXCursor parent_,
    CXClientData udata_) {

}

CXChildVisitResult cursorSuiteBody(
    CXCursor cursor_,
    CXCursor parent_,
    CXClientData udata_) {
  auto pc_(getContext(udata_));

  CXCursorKind kind_(clang_getCursorKind(cursor_));
  switch(kind_) {
    case CXCursor_AnnotateAttr:
      /* -- I expect the otest2::suite annotation */
      return CXChildVisit_Continue;
    case CXCursor_CompoundStmt:
      /* -- the beginning of the suite body */
      clang_visitChildren(cursor_, cursorPrinter, udata_);

      /* -- skip the source of the suite */
      pc_->moveToEnd(cursor_);

      return CXChildVisit_Break;
    default:
      pc_->setError("invalid suite body", cursor_);
      return CXChildVisit_Break;
  }
}

CXChildVisitResult cursorSuite(
    CXCursor cursor_,
    CXCursor parent_,
    CXClientData udata_) {
  auto pc_(getContext(udata_));

  CXCursorKind kind_(clang_getCursorKind(cursor_));
  if(kind_ == CXCursor_AnnotateAttr
     && getClangString(clang_getCursorSpelling(cursor_)) == "otest2::suite") {
    /* -- copy the input file */
    pc_->copyInput(parent_);

    /* -- generate the suite entering code */
    std::string suitename_(getClangString(clang_getCursorSpelling(parent_)));
    pc_->generator->enterSuite(suitename_);

    /* -- parse the suite */
    clang_visitChildren(parent_, cursorSuiteBody, udata_);
  }
  return CXChildVisit_Recurse;
}

//int main(
//    int argc_,
//    char* argv_[]) {
//  CXIndex index(clang_createIndex(1, 1));
//
//  const char* a_[] = {"-std=c++11", nullptr};
//  CXTranslationUnit tu(
//      clang_parseTranslationUnit(
//          index,
//          "example.cpp",
//          a_,
//          1,
//          nullptr,
//          0,
//          CXTranslationUnit_None));
//
//  CXCursor cursor(clang_getTranslationUnitCursor(tu));
//  clang_visitChildren(cursor, cursorPrinter, nullptr);
//
//  for(unsigned I = 0, N = clang_getNumDiagnostics(tu); I != N; ++I) {
//    CXDiagnostic Diag = clang_getDiagnostic(tu, I);
//    CXString String = clang_formatDiagnostic(
//        Diag,
//        clang_defaultDiagnosticDisplayOptions());
//    fprintf(stderr, "%s\n", clang_getCString(String));
//    clang_disposeString(String);
//  }
//
//  clang_disposeTranslationUnit(tu);
//  clang_disposeIndex(index);
//
//  return 0;
//}

void parse(
    const std::string& filename_) {
  /* -- open the Clang index */
  CXIndex index_(clang_createIndex(1, 1));
  auto sg1_(scopeGuard([&index_]() { clang_disposeIndex(index_); }));

  /* -- parse the input file */
  const char* args_[] = {
      "-std=c++11",    /* -- I need the C++11 features */
      "-DOTEST2_PARSER_ACTIVE", /* -- define the parser's ID macro */
  };
  CXTranslationUnit tu_;
  CXErrorCode info_(
      clang_parseTranslationUnit2(
          index_,
          "example.cpp",
          args_,
          sizeof(args_) / sizeof(const char*),
          nullptr,
          0,
          CXTranslationUnit_None,
          &tu_));
  if(info_ != CXError_Success)
    throwClangError("Clang parser failed", info_);
  auto sg2_(scopeGuard([&tu_](){ clang_disposeTranslationUnit(tu_); }));

  /* -- report first diagnostic error */
  const int diag_errors_(clang_getNumDiagnostics(tu_));
  if(diag_errors_ > 0) {
    CXDiagnostic diag_(clang_getDiagnostic(tu_, 0));
    std::string errmsg_("diagnostic error: ");
    CXString message_(
        clang_formatDiagnostic(
            diag_,
            clang_defaultDiagnosticDisplayOptions()));
    auto scopeGuard([&message_](){ clang_disposeString(message_); });
    errmsg_ += clang_getCString(message_);
    throw Error(errmsg_);
  }

  /* -- parse the AST */
  CXCursor cursor_(clang_getTranslationUnitCursor(tu_));
  GeneratorPrinter printer_;
  ParserContext context_(&printer_);
  clang_visitChildren(cursor_, cursorSuite, &context_);

  /* -- report an error */
  if(context_.failure)
    throw ParserException(std::move(context_.exception));

  /* -- copy rest of the file */
  context_.copyInput(cursor_, true);
}

} /* -- namespace OTest2 */
