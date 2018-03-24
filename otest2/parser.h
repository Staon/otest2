#ifndef OTest2OTEST2_PARSER_H_
#define OTest2OTEST2_PARSER_H_

#include <iosfwd>
#include <string>

namespace OTest2 {

class Generator;

/**
 * @brief Parse a test specification file
 *
 * @param filename_ Name of the input file
 * @param generator_ Generator of the output file
 */
void parse(
    const std::string& filename_);

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_PARSER_H_ */
