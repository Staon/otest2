/*
 * Copyright (C) 2019 Ondrej Starek
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

#ifndef OTest2__LIB_TESTMARKHASH_H_
#define OTest2__LIB_TESTMARKHASH_H_

#include <cstdint>
#include <string>

#include <otest2/testmarkhashcode.h>

namespace OTest2 {

/**
 * @brief A simple generator of a testmark hash
 *
 * This class computes a hash value for a test mark tree. It's an attempt
 * to get a simple value to compare the whole subtree.
 *
 * The used FNV-1 function may be a wrong choice. However, I don't need
 * a strong hash function - a collision will cause no problem, just printing
 * of a false test-mark difference.
 */
class TestMarkHash {
  private:
    TestMarkHashCode hash;

  public:
    /**
     * @brief Ctor
     */
    TestMarkHash();

    /**
     * @brief Move ctor
     */
    TestMarkHash(
        TestMarkHash&& other_) noexcept;

    /**
     * @brief Dtor
     */
    ~TestMarkHash();

    /* -- avoid copying */
    TestMarkHash(
        const TestMarkHash&) = delete;
    TestMarkHash& operator =(
        const TestMarkHash&) = delete;

    /**
     * @brief Append piece of data
     *
     * @param data_ Data
     * @param size_ Length of data
     */
    void addData(
        const std::uint8_t* data_,
        std::size_t size_);

    /**
     * @brief Append a terminataor character into the hash
     */
    void addTerminator();

    /**
     * @brief Append a hash code of another testmark
     */
    void addHashCode(
        TestMarkHashCode code_);

    /**
     * @brief Append a string literal
     */
    void addString(
        const char* string_);

    /**
     * @brief Get currently computed hash code
     */
    TestMarkHashCode getHashCode() const noexcept;

    /**
     * @brief Add hash for a value of a basic type
     *
     * @param type_name_ Name of the type
     * @param data_ Binary form of data
     * @param size_ Size of data
     */
    void addBasicType(
        const char* type_name_,
        const std::uint8_t* data_,
        std::size_t size_);

    /**
     * @brief Add hash for a string value of a basic type
     *
     * @param type_name_ Name of the type
     * @param value_ The value
     */
    void addBasicType(
        const char* type_name_,
        const std::string& value_);

    template<typename Type_>
    static TestMarkHashCode hashBasicType(
        const char* type_name_,
        Type_ value_) {
      TestMarkHash hash_;
      hash_.addBasicType(
          type_name_,
          reinterpret_cast<const std::uint8_t*>(&value_),
          sizeof(value_));
      return hash_.getHashCode();
    }

    static TestMarkHashCode hashBasicType(
        const char* type_name_,
        const std::string& value_);
};

} /* -- namespace OTest2 */

#endif /* OTest2__LIB_TESTMARKHASH_H_ */
