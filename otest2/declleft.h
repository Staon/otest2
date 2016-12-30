#ifndef OTest2OTEST2_DECLLEFT_H_
#define OTest2OTEST2_DECLLEFT_H_

#include <cstdint>

#include "declaration.h"

namespace OTest2 {

/**
 * @brief Left part of the declaration
 */
class DeclLeft : public Declaration {
  private:
    /* -- avoid copying */
    DeclLeft(
        const DeclLeft&);
    DeclLeft& operator =(
        const DeclLeft&);

  public:
    /**
     * @brief Ctor
     */
    DeclLeft();

    /**
     * @brief Dtor
     */
    virtual ~DeclLeft();

    /**
     * @brief Apply a modifier mask
     *
     * @param modifier_ The mask
     */
    virtual void applyModifiers(
        std::uint8_t modifier_) = 0;

    /**
     * @brief Apply left declaration part
     *
     * @param decl_ The part. The ownership is taken.
     */
    virtual void applyLeftDecl(
        Declaration* decl_) = 0;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_DECLLEFT_H_ */
