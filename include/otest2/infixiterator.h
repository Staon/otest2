/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2.
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

#ifndef OTest2__INCLUDE_OTEST2_INFIXITERATOR_H_
#define OTest2__INCLUDE_OTEST2_INFIXITERATOR_H_

#include <assert.h>
#include <functional>
#include <iostream>
#include <iterator>

namespace OTest2 {

template<typename T_>
struct IdentityAccessor {
    T_&& operator()(T_&& item_) {
      return std::forward<T_>(item_);
    }
};

/**
 * @brief ostream iterator with infix delimiter
 */
template<typename T_, typename Accessor_ = IdentityAccessor<T_>, typename CharT_ = char, typename Traits_ = std::char_traits<CharT_> >
class InfixIterator
    : public std::iterator<std::output_iterator_tag, void, void, void, void> {
  private:
    std::basic_ostream<CharT_, Traits_>* os;
    Accessor_ accessor;
    const CharT_* delimiter;
    bool first_elem;

  public:
    typedef CharT_ char_type;
    typedef Traits_ traits_type;
    typedef std::basic_ostream<CharT_, Traits_> ostream_type;

    /**
     * @brief Ctor
     *
     * @param os_ An output stream
     * @param delim_ The infix delimiter
     */
    InfixIterator(
        ostream_type* os_,
        const CharT_* delim_) :
      os(os_),
      delimiter(delim_),
      first_elem(true) {
      assert(delimiter != nullptr);

    }

    InfixIterator<T_, Accessor_, CharT_, Traits_>& operator=(
        const T_& item_) {
      if(!first_elem)
        *os << delimiter;
      else
        first_elem = false;
      *os << accessor(item_);
      return *this;
    }

    InfixIterator<T_, Accessor_, CharT_, Traits_>& operator*() {
        return *this;
    }

    InfixIterator<T_, Accessor_, CharT_, Traits_>& operator++() {
        return *this;
    }

    InfixIterator<T_, Accessor_, CharT_, Traits_>& operator++(int) {
        return *this;
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_INFIXITERATOR_H_ */
