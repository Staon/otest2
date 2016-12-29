#ifndef OTest2OTEST2_EXCPARSER_H_
#define OTest2OTEST2_EXCPARSER_H_

#include <datstr/dstring.h>
#include <oexception/excstring.h>
#include <oexception/rethrowable.h>

namespace OTest2 {

class ParserException : public OExceptionRethrowable {
  private:
    excstring message;   /**< error message */
    excstring file;        /**< location of the error */
    int begin_line;
    int begin_column;
    int end_line;
    int end_column;

    /* -- avoid copying */
    ParserException& operator =(
        const ParserException&);

  public:
    /**
     * @brief Ctor
     */
    ParserException();

    /**
     * @brief Copy ctor
     */
    ParserException(
        const ParserException& exc_);

    /**
     * @brief Dtor
     */
    virtual ~ParserException();

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
        const dstring& message_,
        const dstring& file_,
        int begin_line_,
        int begin_column_,
        int end_line_,
        int end_column_);

    /* -- oexception interface */
    virtual dstring reason() const;

    /* -- rethrowable exception interface */
    virtual void rethrow() const;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_EXCPARSER_H_ */
