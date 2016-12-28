#ifndef OTest2OTEST2_DECLMOD_H_
#define OTest2OTEST2_DECLMOD_H_

#include <cstdint>

#include "declleft.h"

namespace OTest2 {

/**
 * @brief A declaration with the modifier
 */
class DeclMod : public DeclLeft {
  private:
    std::uint8_t modifier;
    bool print_parenthesis;

    /* -- avoid copying */
    DeclMod(
        const DeclMod&);
    DeclMod& operator =(
        const DeclMod&);

    virtual bool isChildRightAggressive() const = 0;
    virtual void printLeftBody(
        std::ostream& os_,
        bool print_parenthesis_) = 0;
    virtual void printRightBody(
        std::ostream& os_) = 0;

  public:
    /**
     * @brief Ctor
     */
    DeclMod();

    /**
     * @brief Dtor
     */
    virtual ~DeclMod();

    /* -- declaration interface */
    virtual bool isRightAggressive() const;
    virtual void printLeftDeclaration(
        std::ostream& os_);
    virtual void printRightDeclaration(
        std::ostream& os_);

    /* -- declleft interface */
    virtual void applyModifiers(
        std::uint8_t modifier_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_DECLMOD_H_ */
