#ifndef OTest2OTEST2_DECLPOINTER_H_
#define OTest2OTEST2_DECLPOINTER_H_

#include "declmod.h"

namespace OTest2 {

/**
 * @brief Declaration of a pointer
 */
class DeclPointer : public DeclMod {
  private:
    DeclarationPtr decl;

    /* -- avoid copying */
    DeclPointer(
        const DeclPointer&);
    DeclPointer& operator =(
        const DeclPointer&);

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
    DeclPointer();

    /**
     * @brief Dtor
     */
    virtual ~DeclPointer();

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

#endif /* -- OTest2OTEST2_DECLPOINTER_H_ */
