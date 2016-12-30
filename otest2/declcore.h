#ifndef OTest2OTEST2_DECLCORE_H_
#define OTest2OTEST2_DECLCORE_H_

#include <datstr/dstring.h>
#include <datstr/intrusiveptrdecl.h>
#include <datstr/referencecounter.h>

#include "declaration.h"

namespace OTest2 {

class DeclArgs;

/**
 * @brief Name of the declaration
 */
class DeclCore {
  private:
    ReferenceCounter refcount;

    dstring name;
    DeclarationPtr decl;

    /* -- avoid copying */
    DeclCore(
        const DeclCore&);
    DeclCore& operator =(
        const DeclCore&);

    void incRef();
    void decRef();
    friend class cIntrusivePtrFull<DeclCore>;

  public:
    /**
     * @brief Anonymous declaration
     */
    DeclCore();

    /**
     * @brief Named declaration
     *
     * @param name_ The name
     */
    explicit DeclCore(
        const dstring& name_);

    /**
     * @brief Dtor
     */
    ~DeclCore();

    /**
     * @brief Check if the variable is anonymous
     */
    bool isAnonymous() const;

    /**
     * @brief Get name of the variable
     */
    const dstring& getName() const;

    /**
     * @brief Get declaration of the variable
     */
    const DeclarationPtr& getDeclaration() const;

    /**
     * @brief Apply right part of the declaration
     *
     * @param decl_ The declaration. The ownership is taken.
     */
    void applyRightDecl(
        Declaration* decl_);

    /**
     * @brief Print the declaration
     *
     * @param os_ An output stream
     * @return The output stream
     */
    std::ostream& printDeclaration(
        std::ostream& os_) const;
};

typedef cIntrusivePtrFull<DeclCore> DeclCorePtr;

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_DECLCORE_H_ */
