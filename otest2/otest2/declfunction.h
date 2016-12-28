#ifndef OTest2OTEST2_DECLFUNCTION_H_
#define OTest2OTEST2_DECLFUNCTION_H_

#include "declaration.h"

namespace OTest2 {

class DeclArgs;

/**
 * @brief Function declaration
 */
class DeclFunction : public Declaration {
  private:
    DeclarationPtr rettype;
    DeclarationPtr arguments;

    /* -- avoid copying */
    DeclFunction(
        const DeclFunction&);
    DeclFunction& operator =(
        const DeclFunction&);

  public:
    /**
     * @brief Ctor
     *
     * @param rettype_ Return type. The ownership is taken.
     * @param arguments_ Arguments of the function. The ownership is taken.
     */
    explicit DeclFunction(
        Declaration* rettype_,
        DeclArgs* arguments_);

    /**
     * @brief Dtor
     */
    virtual ~DeclFunction();

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

#endif /* -- OTest2OTEST2_DECLFUNCTION_H_ */
