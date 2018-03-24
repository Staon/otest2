#ifndef OTest2INCLUDE_CASE_H_
#define OTest2INCLUDE_CASE_H_

namespace OTest2 {

/**
 * @brief Interface of a test case
 */
class Case {
  private:
    /* -- avoid copying */
    Case(
        const Case&);
    Case& operator =(
        const Case&);

  public:
    /**
     * @brief Ctor
     */
    Case();

    /**
     * @brief Dtor
     */
    virtual ~Case();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2INCLUDE_CASE_H_ */
