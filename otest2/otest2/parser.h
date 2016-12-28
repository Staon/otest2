#ifndef OTest2OTEST2_PARSER_H_
#define OTest2OTEST2_PARSER_H_

#include <iosfwd>

namespace OTest2 {

class Generator;

/**
 * @brief Parse a test specification file
 *
 * @param is_ Input stream
 * @param generator_ Generator of the output file
 */
void parse(
    std::istream& is_,
    Generator& generator_);

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_PARSER_H_ */
