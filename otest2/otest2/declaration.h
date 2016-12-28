#ifndef OTest2OTEST2_DECLARATION_H_
#define OTest2OTEST2_DECLARATION_H_

#include <cstdint>
#include <datstr/intrusiveptrdecl.h>
#include <datstr/referencecounter.h>
#include <iosfwd>

namespace OTest2 {

class Declaration;
class DeclArgs;

typedef cIntrusivePtrFull<Declaration> DeclarationPtr;

/**
 * @brief Generic declaration interface
 */
class Declaration {
  public:
    enum {
      CONST = 1 << 0,   /**< "const" modifier */
    };

  private:
    ReferenceCounter refcount;

    /* -- avoid copying */
    Declaration(
        const Declaration&);
    Declaration& operator =(
        const Declaration&);

    /* -- intrusive pointer */
    void incRef();
    void decRef();
    friend class cIntrusivePtrFull<Declaration>;

  public:
    /**
     * @brief Ctor
     */
    Declaration();

    /**
     * @brief Dtor
     */
    virtual ~Declaration();

    /**
     * @brief Apply right declaration
     *
     * @param decl_ The right part. The ownership is taken.
     */
    virtual void applyRightDecl(
        Declaration* decl_) = 0;

    /**
     * @brief Is the node right aggressive?
     */
    virtual bool isRightAggressive() const = 0;

    /**
     * @brief Apply a modifier mask the deepest node
     *
     * @param modifier_ The mask
     */
    virtual void applyModifiersDeep(
        std::uint8_t modifier_) = 0;

    /**
     * @brief Print the left part of the declaration
     *
     * @param os_ An output stream
     */
    virtual void printLeftDeclaration(
        std::ostream& os_) = 0;

    /**
     * @brief Print right part of the declaration
     *
     * @param os_ An output stream
     */
    virtual void printRightDeclaration(
        std::ostream& os_) = 0;

    /**
     * @brief Make reference to current type
     *
     * @return The reference declaration object
     */
    virtual DeclarationPtr makeReference();
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_DECLARATION_H_ */
