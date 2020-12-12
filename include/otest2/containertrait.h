/*
 * Copyright (C) 2020 Ondrej Starek
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

#ifndef OTest2_INCLUDE_OTEST2_CONTAINERTRAIT_H_
#define OTest2_INCLUDE_OTEST2_CONTAINERTRAIT_H_

#include <map>
#include <unordered_map>
#include <utility>

#include <otest2/typetraits.h>

namespace OTest2 {

/**
 * @brief A simple trait allowing access to list container's iterators
 *
 * This trait is used by the item-wise and lexicographical assertions
 * to get the begin and end iterator. This default implementation works
 * well with the STL containers.
 *
 * One can write own specialization of the the trait to plug own container
 * into the assertions. The iterators must follow:
 *   - the * operator to access the pointed value,
 *   - the prefix ++ operator moving to next item,
 *   - the != operator to to compare iterators.
 */
template<typename Container_>
struct ListContainerTrait {
    /**
     * @brief Get beginning iterator
     */
    static typename Container_::const_iterator begin(
        const Container_& cont_) {
      return cont_.begin();
    }

    /**
     * @brief Get ending iterator
     */
    static typename Container_::const_iterator end(
        const Container_& cont_) {
      return cont_.end();
    }
};

namespace Private {

template<typename Type_>
class HasContainerKeyComp {
  private:
    template<typename T_> struct SfinaeCheck : public std::true_type {};

    template<typename T_>
    static auto test_fce(void*) -> SfinaeCheck<decltype(&T_::key_comp)>;
    template<typename T_>
    static std::false_type test_fce(...);

  public:
    static constexpr bool value = decltype(test_fce<Type_>(nullptr))::value;
};

template<typename Container_>
struct MapTraitImplCommon {
    /**
     * @brief Type of the key
     */
    typedef typename Container_::key_type KeyType;

    /**
     * @brief Type of the mapped value
     */
    typedef typename Container_::mapped_type ValueType;

    typedef typename Container_::const_iterator const_iterator;

    /**
     * @brief Get the beginning forward iterator
     */
    static const_iterator begin(
        const Container_& cont_) {
      return cont_.begin();
    }

    /**
     * @brief Get ending iterator
     */
    static const_iterator end(
        const Container_& cont_) {
      return cont_.end();
    }

    /**
     * @brief Get range of items with the same key
     */
    static std::pair<const_iterator, const_iterator> equalRange(
        const Container_& cont_,
        typename TypeTrait<KeyType>::BestArg key_) {
      return cont_.equal_range(key_);
    }
};

template<typename Container_, bool key_comp_>
struct MapTraitImpl;

template<typename Container_>
struct MapTraitImpl<Container_, true> : public MapTraitImplCommon<Container_>
{
    /**
     * @brief Compare equality of two keys
     */
    static bool keyEqual(
        const Container_& cont_,
        typename TypeTrait<typename MapTraitImplCommon<Container_>::KeyType>::BestArg a_key_,
        typename TypeTrait<typename MapTraitImplCommon<Container_>::KeyType>::BestArg b_key_) {
      return !cont_.key_comp()(a_key_, b_key_) && !cont_.key_comp()(b_key_, a_key_);
    }
};

template<typename Container_>
struct MapTraitImpl<Container_, false> : public MapTraitImplCommon<Container_>
{
    /**
     * @brief Compare equality of two keys
     */
    static bool keyEqual(
        const Container_& cont_,
        typename TypeTrait<typename MapTraitImplCommon<Container_>::KeyType>::BestArg a_key_,
        typename TypeTrait<typename MapTraitImplCommon<Container_>::KeyType>::BestArg b_key_) {
      return cont_.key_eq()(a_key_, b_key_);
    }
};

} /* -- namespace Private */

/**
 * @brief A trait for access to a map container
 *
 * The trait must offer:
 *   * typedef KeyType: type of the keys
 *   * typedef ValueType: type of the mapped values
 *   * function begin(container_): access to the beginning iterator. The
 *     iterator must points a std::pair<KeyType, ValueType>.
 *   * function end(container_): access to the ending iterator.
 *   * function equalRange(container_, key_): the function returns a pair
 *     of iterators pointing a range of items mapped with the same key.
 *   * function keyEqual(container_, key1_, key2_): the function returns
 *     true if the keys equal.
 */
template<typename Container_>
struct MapContainerTrait :
    public Private::MapTraitImpl<
        Container_, Private::HasContainerKeyComp<Container_>::value> {};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_CONTAINERTRAIT_H_ */
