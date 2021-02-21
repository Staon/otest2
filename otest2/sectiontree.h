/*
 * Copyright (C) 2021 Ondrej Starek
 *
 * This file is part of OTest2
 *
 * OTest2 is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * OTest2 is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OTest2.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OTest2_OTEST2_SECTIONTREE_H_
#define OTest2_OTEST2_SECTIONTREE_H_

#include <iosfwd>
#include <string>
#include <vector>

namespace OTest2 {

namespace Parser {

/**
 * @brief A tree container of section records
 */
class SectionTree {
  private:
    struct Section {
      std::string name;
      std::vector<Section> children;
    };
    std::vector<Section> root_sections;
    std::vector<Section*> section_stack;

    void printRegistrationImpl(
        std::ostream& os_,
        const Section& section,
        int indent_) const;

  public:
    /* -- avoid copying */
    SectionTree(
        const SectionTree&) = delete;
    SectionTree& operator = (
        const SectionTree&) = delete;

    /**
     * @brief Ctor
     */
    SectionTree();

    /**
     * @brief Dtor
     */
    ~SectionTree();

    /**
     * @brief Check whether the tree is empty
     */
    bool empty() const noexcept;

    /**
     * @brief Clear the section tree
     */
    void clear() noexcept;

    /**
     * @brief Push new section
     *
     * @param name_ Name of the section
     */
    void pushSection(
        const std::string& name_);

    /**
     * @brief Pop section from the top of the stack
     */
    void popSection();

    /**
     * @brief Get full path of the section at the top of the stack
     */
    std::string getSectionPath() const;

    /**
     * @brief Generate code which registers the sections
     *
     * @param os_ An output stream
     * @param indent_ Indentation level
     */
    void printRegistration(
        std::ostream& os_,
        int indent_) const;
};

} /* -- namespace Parser */

} /* -- namespace OTest2 */

#endif /* -- OTest2_OTEST2_SECTIONTREE_H_ */
