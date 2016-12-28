#ifndef OTest2OTEST2_DECLMEMPTR_H_
#define OTest2OTEST2_DECLMEMPTR_H_

#include <datstr/dstring.h>

#include "declaration.h"
#include "declmod.h"

namespace OTest2 {

/**
 * @brief Member pointer
 */
class DeclMemPtr : public DeclMod {
  private:
    dstring type;
    DeclarationPtr decl;

    /* -- avoid copying */
    DeclMemPtr(
        const DeclMemPtr&);
    DeclMemPtr& operator =(
        const DeclMemPtr&);

    virtual bool isChildRightAggressive() const;
    virtual void printLeftBody(
        std::ostream& os_,
        bool print_parenthesis_);
    virtual void printRightBody(
        std::ostream& os_);

  public:
    /**
     * @brief Ctor
     *
     */
    explicit DeclMemPtr(
        const dstring& type_);

    /**
     * @brief Dtor
     */
    virtual ~DeclMemPtr();

    /* -- declaration interface */
    virtual void applyRightDecl(
        Declaration* decl_);
    virtual void applyModifiersDeep(
        std::uint8_t modifier_);

    /* -- declleft interface */
    virtual void applyLeftDecl(
        Declaration* decl_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_DECLMEMPTR_H_ */
