#ifndef OTest2OTEST2_DECLARRAY_H_
#define OTest2OTEST2_DECLARRAY_H_

#include <datstr/dstring.h>

#include "declaration.h"

namespace OTest2 {

/**
 * @brief Array declaration
 */
class DeclArray : public Declaration {
  private:
    dstring size;
    DeclarationPtr decl;

    /* -- avoid copying */
    DeclArray(
        const DeclArray&);
    DeclArray& operator =(
        const DeclArray&);

  public:
    /**
     * @brief Ctor
     *
     * @param size_ Size of the array
     * @param decl_ Type of the array items. The ownership is taken.
     */
    explicit DeclArray(
        const dstring& size_,
        Declaration* decl_);

    /**
     * @brief Dtor
     */
    virtual ~DeclArray();

    /* -- declaration interface */
    virtual void applyRightDecl(
        Declaration* decl_);
    virtual bool isRightAggressive() const;
    virtual void applyModifiersDeep(
        std::uint8_t modifier_);
    virtual void printLeftDeclaration(
        std::ostream& os_);
    virtual void printRightDeclaration(
        std::ostream& os_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_DECLARRAY_H_ */
