#include "parser.h"

#include <clang-c/Index.h>
#include <iostream>
#include <string>

#include "error.h"
#include "excparser.h"
#include "generatorprinter.h"
#include "location.h"

namespace OTest2 {

const std::string SUITE_ANNOTATION("otest2::suite");
const std::string START_UP_MARK("OTest2::StartUpMark");
const std::string TEAR_DOWN_MARK("OTest2::TearDownMark");
const std::string CASE_MARK("OTest2::CaseMark");
const std::string STATE_MARK("OTest2::StateMark");

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

    /* -- parser state */
    enum State {
      ROOT = 0,
      SUITE_VAR,
      SUITE_START_UP,
      SUITE_START_UP_END,
      SUITE_TEAR_DOWN,
      SUITE_TEAR_DOWN_END,
      CASE_BEGIN,
      CASE_VAR,
      CASE_START_UP,
      CASE_START_UP_END,
      CASE_TEAR_DOWN,
      CASE_TEAR_DOWN_END,
      CASE_NEXT_STATE,
      CASE_FINISHED,
      STATE_BEGIN,
    };
    State state;

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
    void copyInputOfCursor(
        CXCursor cursor_);
    void moveToEnd(
        CXCursor cursor_);
};

ParserContext::ParserContext(
    Generator* generator_) :
  failure(false),
  exception(),
  state(ROOT),
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
  clang_getSpellingLocation(end_, nullptr, &end_line_, &end_column_, nullptr);

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

void ParserContext::copyInputOfCursor(
    CXCursor cursor_) {
  CXSourceRange range_(clang_getCursorExtent(cursor_));
  CXSourceLocation beg_(clang_getRangeStart(range_));
  CXSourceLocation end_(clang_getRangeEnd(range_));
  generator->copySource(createLocation(beg_), createLocation(end_));
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
  std::string spelling_(getClangString(clang_getCursorSpelling(cursor_)));
  std::string kind_(getClangString(clang_getCursorKindSpelling(clang_getCursorKind(cursor_))));

  /* -- parse location */
  CXSourceRange range_(clang_getCursorExtent(cursor_));
  CXSourceLocation begin_(clang_getRangeStart(range_));
  CXSourceLocation end_(clang_getRangeEnd(range_));
  std::cout << spelling_ << "(" << kind_ << ") at " << createLocation(begin_)
            << "-" << createLocation(end_) << std::endl;

  return CXChildVisit_Recurse;
}

struct VariableInfo {
    std::string name;
    CXType type;
    std::string typestr;
    bool init_present;
    CXCursor initializer;
    Location begin;
    Location end;

    VariableInfo();
};

VariableInfo::VariableInfo() :
    name(),
    type(),
    typestr(),
    init_present(false),
    initializer(),
    begin(),
    end() {

}

CXChildVisitResult cursorVariableInitializer(
    CXCursor cursor_,
    CXCursor parent_,
    CXClientData udata_) {
  VariableInfo* info_(static_cast<VariableInfo*>(udata_));

  CXCursorKind kind_(clang_getCursorKind(cursor_));
  if(clang_isExpression(kind_)) {
    if(!info_->init_present) {
      info_->init_present = true;
      info_->initializer = cursor_;
      return CXChildVisit_Continue;
    }

    /* -- repeated expression */
    return CXChildVisit_Break;
  }

  return CXChildVisit_Continue;
}

CXChildVisitResult cursorVariable(
    CXCursor cursor_,
    CXCursor parent_,
    CXClientData udata_) {
  VariableInfo* info_(static_cast<VariableInfo*>(udata_));

  CXCursorKind kind_(clang_getCursorKind(cursor_));
  if(kind_ == CXCursor_VarDecl && info_->name.empty()) {
    /* -- variable name */
    info_->name = getClangString(clang_getCursorSpelling(cursor_));

    /* -- variable type */
    info_->type = clang_getCursorType(cursor_);
    info_->typestr = getClangString(clang_getTypeSpelling(info_->type));

    /* -- get variable location */
    CXSourceRange range_(clang_getCursorExtent(cursor_));
    CXSourceLocation begin_(clang_getRangeStart(range_));
    CXSourceLocation end_(clang_getRangeEnd(range_));
    info_->begin = createLocation(begin_);
    info_->end = createLocation(end_);

    /* -- get initializer */
    if(clang_visitChildren(cursor_, cursorVariableInitializer, udata_))
      return CXChildVisit_Break;

    clang_visitChildren(cursor_, cursorPrinter, udata_);

    std::cout << info_->name << ' ' << info_->typestr << std::endl;
    std::cout << "  " << (info_->init_present ? "yes" : "no") << std::endl;

    return CXChildVisit_Continue;
  }

  return CXChildVisit_Break;
}

CXChildVisitResult cursorCaseBodyInside(
    CXCursor cursor_,
    CXCursor parent_,
    CXClientData udata_) {
  auto pc_(getContext(udata_));

  CXCursorKind kind_(clang_getCursorKind(cursor_));
  switch(kind_) {
    case CXCursor_DeclStmt: {
      /* -- parse the variable */
      VariableInfo varinfo_;
      if(clang_visitChildren(cursor_, cursorVariable, &varinfo_)) {
        pc_->setError("a variable is expected", cursor_);
        return CXChildVisit_Break;
      }

      switch(pc_->state) {
        case ParserContext::CASE_VAR:
          /* -- suite constructor */
          if(varinfo_.typestr == START_UP_MARK) {
            pc_->state = ParserContext::CASE_START_UP;
            return CXChildVisit_Continue;
          }
          /* -- missing break is expected */

        case ParserContext::CASE_START_UP_END:
          /* -- suite destructor */
          if(varinfo_.typestr == TEAR_DOWN_MARK) {
            pc_->state = ParserContext::CASE_TEAR_DOWN;
            return CXChildVisit_Continue;
          }
          /* -- missing break is expected */

        case ParserContext::CASE_TEAR_DOWN_END:
        case ParserContext::CASE_NEXT_STATE:
          if(varinfo_.typestr == STATE_MARK) {
            /* -- beginning of a case */
            pc_->state = ParserContext::STATE_BEGIN;
            pc_->generator->enterState(varinfo_.name);

            return CXChildVisit_Continue;
          }
          /* -- missing break is expected */

          if(pc_->state == ParserContext::CASE_VAR) {
            /* -- TODO: case variable */
            return CXChildVisit_Continue;
          }
          break;

        default:
          break;
      }

      pc_->setError("invalid case body", cursor_);
      return CXChildVisit_Break;
    }
    case CXCursor_CompoundStmt:
      switch(pc_->state) {
        case ParserContext::CASE_START_UP:
          /* -- suite constructor */
          pc_->copyInputOfCursor(cursor_);
          pc_->state = ParserContext::CASE_START_UP_END;
          return CXChildVisit_Continue;

        case ParserContext::CASE_TEAR_DOWN:
          /* -- suite destructor */
          pc_->copyInputOfCursor(cursor_);
          pc_->state = ParserContext::CASE_TEAR_DOWN_END;
          return CXChildVisit_Continue;

        case ParserContext::STATE_BEGIN:
          /* -- copy the body of the state */
          pc_->copyInputOfCursor(cursor_);

          /* -- allow another case */
          pc_->state = ParserContext::CASE_NEXT_STATE;

          return CXChildVisit_Continue;

        case ParserContext::CASE_VAR:
        case ParserContext::CASE_START_UP_END:
        case ParserContext::CASE_TEAR_DOWN_END:
          /* -- test case without states */
          pc_->generator->enterState("Init");
          pc_->copyInputOfCursor(cursor_);

          /* -- no other states are allowed */
          pc_->state = ParserContext::CASE_FINISHED;

          return CXChildVisit_Continue;

        default:
          pc_->setError("expected a test state", cursor_);
          return CXChildVisit_Break;
      }
    default:
      pc_->setError("invalid case body", cursor_);
      return CXChildVisit_Break;
  }
}

CXChildVisitResult cursorSuiteBodyInside(
    CXCursor cursor_,
    CXCursor parent_,
    CXClientData udata_) {
  auto pc_(getContext(udata_));

  CXCursorKind kind_(clang_getCursorKind(cursor_));
  switch(kind_) {
    case CXCursor_DeclStmt: {
      /* -- parse the variable */
      VariableInfo varinfo_;
      if(clang_visitChildren(cursor_, cursorVariable, &varinfo_)) {
        pc_->setError("a variable is expected", cursor_);
        return CXChildVisit_Break;
      }

      switch(pc_->state) {
        case ParserContext::SUITE_VAR:
          /* -- suite constructor */
          if(varinfo_.typestr == START_UP_MARK) {
            pc_->state = ParserContext::SUITE_START_UP;
            return CXChildVisit_Continue;
          }
          /* -- missing break is expected */

        case ParserContext::SUITE_START_UP_END:
          /* -- suite destructor */
          if(varinfo_.typestr == TEAR_DOWN_MARK) {
            pc_->state = ParserContext::SUITE_TEAR_DOWN;
            return CXChildVisit_Continue;
          }
          /* -- missing break is expected */

        case ParserContext::SUITE_TEAR_DOWN_END:
          if(varinfo_.typestr == CASE_MARK) {
            /* -- beginning of a case */
            pc_->state = ParserContext::CASE_BEGIN;
            pc_->generator->enterCase(varinfo_.name);

            return CXChildVisit_Continue;
          }
          /* -- missing break is expected */

          if(pc_->state == ParserContext::SUITE_VAR) {
            /* -- TODO: suite variable */
            return CXChildVisit_Continue;

          }

        default:
          pc_->setError("expected a suite variable", cursor_);
          return CXChildVisit_Break;
      }

      pc_->setError("invalid suite body", cursor_);
      return CXChildVisit_Break;
    }
    case CXCursor_CompoundStmt:
      switch(pc_->state) {
        case ParserContext::SUITE_START_UP:
          /* -- suite constructor */
          pc_->copyInputOfCursor(cursor_);
          pc_->state = ParserContext::SUITE_START_UP_END;
          return CXChildVisit_Continue;
        case ParserContext::SUITE_TEAR_DOWN:
          /* -- suite destructor */
          pc_->copyInputOfCursor(cursor_);
          pc_->state = ParserContext::SUITE_TEAR_DOWN_END;
          return CXChildVisit_Continue;
        case ParserContext::CASE_BEGIN:
          /* -- parse the case */
          pc_->state = ParserContext::CASE_VAR;
          if(clang_visitChildren(cursor_, cursorCaseBodyInside, udata_))
            return CXChildVisit_Break;

          /* -- allow another case */
          pc_->state = ParserContext::SUITE_TEAR_DOWN_END;

          return CXChildVisit_Continue;
        default:
          pc_->setError("expected a test case", cursor_);
          return CXChildVisit_Break;
      }
    default:
      pc_->setError("invalid suite body", cursor_);
      return CXChildVisit_Break;
  }
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
      /* -- parse suite body */
      pc_->state = ParserContext::SUITE_VAR;
      if(clang_visitChildren(cursor_, cursorSuiteBodyInside, udata_))
        return CXChildVisit_Break;

      /* -- skip the source of the suite */
      pc_->moveToEnd(cursor_);

      return CXChildVisit_Continue;
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
     && getClangString(clang_getCursorSpelling(cursor_)) == SUITE_ANNOTATION) {
    /* -- copy the input file */
    pc_->copyInput(parent_);

    /* -- generate the suite entering code */
    std::string suitename_(getClangString(clang_getCursorSpelling(parent_)));
    pc_->generator->enterSuite(suitename_);

    /* -- parse the suite */
    if(clang_visitChildren(parent_, cursorSuiteBody, udata_))
      return CXChildVisit_Break;
  }
  return CXChildVisit_Recurse;
}

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
