#ifndef OTest2OTEST2_DECLREFERENCE_H_
#define OTest2OTEST2_DECLREFERENCE_H_

#include "declmod.h"

namespace OTest2 {

/**
 * @brief Reference to a type
 */
class DeclReference : public DeclMod {
  private:
    DeclarationPtr decl;

    /* -- avoid copying */
    DeclReference(
        const DeclReference&);
    DeclReference& operator =(
        const DeclReference&);

    virtual bool isChildRightAggressive() const;
    virtual void printLeftBody(
        std::ostream& os_,
        bool print_parenthesis_);
    virtual void printRightBody(
        std::ostream& os_);

  public:
    /**
     * @brief Ctor
     */
    DeclReference();

    /**
     * @brief Dtor
     */
    virtual ~DeclReference();

    /* -- declaration interface */
    virtual void applyRightDecl(
        Declaration* decl_);
    virtual void applyModifiersDeep(
        std::uint8_t modifier_);

    /* -- declleft interface */
    virtual void applyLeftDecl(
        DeclLeft* decl_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_DECLREFERENCE_H_ */
