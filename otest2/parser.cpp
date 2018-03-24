#include "parser.h"

#include <clang-c/Index.h>
#include <iostream>
#include <string>

namespace OTest2 {


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
  CXIndex index(clang_createIndex(1, 1));

  const char* a_[] = {"-std=c++11", nullptr};
  CXTranslationUnit tu(
      clang_parseTranslationUnit(
          index,
          "example.cpp",
          a_,
          1,
          nullptr,
          0,
          CXTranslationUnit_None));

  CXCursor cursor(clang_getTranslationUnitCursor(tu));
  clang_visitChildren(cursor, cursorPrinter, nullptr);

  for(unsigned I = 0, N = clang_getNumDiagnostics(tu); I != N; ++I) {
    CXDiagnostic Diag = clang_getDiagnostic(tu, I);
    CXString String = clang_formatDiagnostic(
        Diag,
        clang_defaultDiagnosticDisplayOptions());
    fprintf(stderr, "%s\n", clang_getCString(String));
    clang_disposeString(String);
  }

  clang_disposeTranslationUnit(tu);
  clang_disposeIndex(index);
}

} /* -- namespace OTest2 */
