#include "excparser.h"

#include <astraios/dstream.h>

namespace OTest2 {

ParserException::ParserException() :
  message(),
  file(),
  begin_line(0),
  begin_column(0),
  end_line(0),
  end_column(0) {

}

ParserException::ParserException(
    const ParserException& exc_) :
  OExceptionRethrowable(exc_),
  message(exc_.message),
  file(exc_.file),
  begin_line(exc_.begin_line),
  begin_column(exc_.begin_column),
  end_line(exc_.end_line),
  end_column(exc_.end_column) {

}

ParserException::~ParserException() {

}

void ParserException::setException(
    const dstring& message_,
    const dstring& file_,
    int begin_line_,
    int begin_column_,
    int end_line_,
    int end_column_) {
  message = message_;
  file = file_;
  begin_line = begin_line_;
  begin_column = begin_column_;
  end_line = end_line_;
  end_column = end_column_;
}

dstring ParserException::reason() const {
  dstrostream dos_;
  dos_ << file << '(' << begin_line << '.' << begin_column << '-' << end_line
      << '.' << end_column << "): " << message;
  return dos_.str();
}

void ParserException::rethrow() const {
  throw ParserException(*this);
}

} /* -- namespace OTest2 */
