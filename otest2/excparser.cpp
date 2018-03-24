#include "excparser.h"

#include <sstream>
#include <utility>

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
    ParserException&& exc_) :
  Exception(std::forward<Exception>(exc_)),
  message(std::move(exc_.message)),
  file(std::move(exc_.file)),
  begin_line(exc_.begin_line),
  begin_column(exc_.begin_column),
  end_line(exc_.end_line),
  end_column(exc_.end_column) {

}

ParserException::~ParserException() {

}

void ParserException::setException(
    const std::string& message_,
    const std::string& file_,
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

std::string ParserException::reason() const {
  std::ostringstream sos_;
  sos_ << file << '(' << begin_line << '.' << begin_column << '-' << end_line
      << '.' << end_column << "): " << message;
  return sos_.str();
}

} /* -- namespace OTest2 */
