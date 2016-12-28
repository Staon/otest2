#ifndef OTest2OTEST2_VARTABLE_H_
#define OTest2OTEST2_VARTABLE_H_

#include <datstr/dstring.h>
#include <datstr/intrusiveptrdecl.h>
#include <datstr/referencecounter.h>
#include <iosfwd>
#include <map>
#include <vector>

#include "declaration.h"

namespace OTest2 {

class VarTable;

typedef cIntrusivePtrFull<VarTable> VarTablePtr;

/**
 * @brief Table of variables
 */
class VarTable {
  private:
    ReferenceCounter refcount;

    dstring name;
    VarTablePtr level;
    struct Record {
        bool mine;
        DeclarationPtr declaration;
        dstring initializer;
    };
    typedef std::map<dstring, Record> Variables;
    Variables variables;
    typedef std::vector<dstring> Order;
    Order order;

    /* -- avoid copying */
    VarTable(
        const VarTable&);
    VarTable& operator =(
        const VarTable&);

    /* -- intrusive pointer */
    void incRef();
    void decRef();
    friend class cIntrusivePtrFull<VarTable>;

  public:
    /**
     * @brief Ctor
     *
     * @param name_ Name of current level.
     * @param level_ Previous namespace level. The ownership is taken. It can
     *     be null.
     */
    explicit VarTable(
        const dstring& name_,
        VarTable* level_);

    /**
     * @brief Dtor
     */
    ~VarTable();

    /**
     * @brief Get name of the level
     *
     * @return The name
     */
    const dstring& getName() const;

    /**
     * @brief Get previous variable level
     */
    const VarTablePtr& getPrevLevel() const;

    /**
     * @brief Append new variable
     *
     * @param name_ Name of the variable
     * @param declaration_ Variable's declaration
     */
    void appendVariable(
        const dstring& name_,
        const DeclarationPtr& declaration_);

    /**
     * @brief Set initializer of a variable
     *
     * @param name_ Name of the variable
     * @param body_ Body of the initializer
     * @return True if the initializer is set
     */
    bool setInitializer(
        const dstring& name_,
        const dstring& body_);

    /**
     * @brief Print declarations
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     */
    void printDeclarations(
        std::ostream& os_,
        int indent_) const;

    /**
     * @brief Print the initializers
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     */
    void printInitializers(
        std::ostream& os_,
        int indent_) const;
};

} /* -- namespace OTest2 */

#endif /* -- OTest2OTEST2_VARTABLE_H_ */
