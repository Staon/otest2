#ifndef OTest2OTEST2_DECLTYPE_H_
#define OTest2OTEST2_DECLTYPE_H_

#include <datstr/dstring.h>

#include "declmod.h"

namespace OTest2 {

/**
 * @brief Named type
 */
class DeclType : public DeclMod {
  private:
    dstring type;

    /* -- avoid copying */
    DeclType(
        const DeclType&);
    DeclType& operator =(
        const DeclType&);

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
     * @param type_ Name of the type
     */
    explicit DeclType(
        const dstring& type_);

    /**
     * @brief Dtor
     */
    virtual ~DeclType();

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

#endif /* -- OTest2OTEST2_DECLTYPE_H_ */
