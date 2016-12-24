#ifndef OTest2INCLUDE_SUITE_H_
#define OTest2INCLUDE_SUITE_H_

namespace OTest2 {

/**
 * @brief Suite interface
 */
class Suite {
  private:
    /* -- avoid copying */
    Suite(
        const Suite&);
    Suite& operator =(
        const Suite&);

  public:
    /**
     * @brief Ctor
     */
    Suite();

    /**
     * @brief Dtor
     */
    virtual ~Suite();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_SUITE_H_ */
