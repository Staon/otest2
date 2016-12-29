#ifndef OTest2OTEST2_PARSER_H_
#define OTest2OTEST2_PARSER_H_

#include <datstr/dstring.h>
#include <iosfwd>

namespace OTest2 {

class Generator;

/**
 * @brief Parse a test specification file
 *
 * @param is_ Input stream
 * @param filename_ Name of the stream (used for the location tracking)
 * @param generator_ Generator of the output file
 */
void parse(
    std::istream& is_,
    const dstring& filename_,
    Generator& generator_);

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_PARSER_H_ */
