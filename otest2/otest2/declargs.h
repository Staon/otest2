#ifndef OTest2OTEST2_DECLARGS_H_
#define OTest2OTEST2_DECLARGS_H_

#include <vector>

#include "declaration.h"
#include "declcore.h"

namespace OTest2 {

class DeclCore;

/**
 * @brief List of function arguments
 */
class DeclArgs : public Declaration {
  private:
    typedef std::vector<DeclCorePtr> Arguments;
    Arguments arguments;

    /* -- avoid copying */
    DeclArgs(
        const DeclArgs&);
    DeclArgs& operator =(
        const DeclArgs&);

  public:
    /**
     * @brief Ctor
     */
    DeclArgs();

    /**
     * @brief Dtor
     */
    virtual ~DeclArgs();

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

    /**
     * @brief Append next argument
     *
     * @param arg_ The argument
     */
    void appendArgument(
        DeclCore* arg_);
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_DECLARGS_H_ */
