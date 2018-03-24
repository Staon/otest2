#ifndef OTest2OTEST2_EXCPARSER_H_
#define OTest2OTEST2_EXCPARSER_H_

#include <string>

#include <exc.h>

namespace OTest2 {

/**
 * @brief An error during parsing of the test file
 */
class ParserException : public Exception {
  private:
    std::string message;   /**< error message */
    std::string file;      /**< location of the error */
    int begin_line;
    int begin_column;
    int end_line;
    int end_column;

  public:
    /**
     * @brief Ctor
     */
    ParserException();

    /**
     * @brief Move ctor
     */
    ParserException(
        ParserException&& exc_);

    /**
     * @brief Dtor
     */
    virtual ~ParserException();

    /* -- avoid copying */
    ParserException(
        const ParserException&) = delete;
    ParserException& operator =(
        const ParserException&) = delete;

    /**
     * @brief Set attributes of the exception
     *
     * @param message_ Error message
     * @param file_ location of the error - filename
     * @param begin_line_ location of the error - beginning line number
     * @param begin_column_ location of the error - beginning column number
     * @param end_line_ location of the error - ending line number
     * @param end_column_ location of the error - ending line column
     */
    void setException(
        const std::string& message_,
        const std::string& file_,
        int begin_line_,
        int begin_column_,
        int end_line_,
        int end_column_);

    /* -- exception interface */
    virtual std::string reason() const;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_EXCPARSER_H_ */
