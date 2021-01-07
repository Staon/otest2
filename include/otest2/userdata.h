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

#ifndef OTest2__INCLUDE_OTEST2_USERDATA_H_
#define OTest2__INCLUDE_OTEST2_USERDATA_H_

#include <assert.h>
#include <memory>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include <otest2/exc.h>
#include <otest2/typetraits.h>
#include <otest2/utils.h>

namespace OTest2 {

/**
 * @brief An exception thrown if user datum is missing
 */
class UserDataMissingException : public Exception {
  private:
    std::string name;

  public:
    /**
     * @brief Ctor
     *
     * @param name_ Name of the datum
     */
    explicit UserDataMissingException(
        const std::string& name_);

    /**
     * @brief Move ctor
     */
    UserDataMissingException(
        UserDataMissingException&& exc_) noexcept;

    /**
     * @brief Dtor
     */
    ~UserDataMissingException();

    /* -- avoid copying */
    UserDataMissingException(
        const UserDataMissingException&) = delete;
    UserDataMissingException& operator = (
        const UserDataMissingException&) = delete;

    /* -- exception interface */
    virtual std::string reason() const override;
};

/**
 * @brief An exception thrown if the expected type of a user datum is different
 *     than the actual one.
 */
class UserDataWrongTypeException : public Exception {
  private:
    std::string name;
    std::string actual_type;
    std::string expected_type;

  public:
    /**
     * @brief Ctor
     *
     * @param name_ Name of the user datum
     * @param actual_ Actual type (stored in the user data object)
     * @param expected_ Expected type (requested by the test description)
     */
    explicit UserDataWrongTypeException(
        const std::string& name_,
        const std::string& actual_,
        const std::string& expected_);

    /**
     * @brief Move ctor
     */
    UserDataWrongTypeException(
        UserDataWrongTypeException&& exc_) noexcept;

    /**
     * @brief Dtor
     */
    ~UserDataWrongTypeException();

    /* -- avoid copying */
    UserDataWrongTypeException(
        const UserDataWrongTypeException&) = delete;
    UserDataWrongTypeException& operator = (
        const UserDataWrongTypeException&) = delete;

    /* -- exception interface */
    virtual std::string reason() const override;
};

/**
 * @brief An object keeping user data passed from user's custom main function
 */
class UserData {
  private:
    struct Impl;
    Impl* pimpl;

    class Datum {
      public:
        Datum();
        virtual ~Datum();

        /* -- avoid copying */
        Datum(
            const Datum&) = delete;
        Datum& operator = (
            const Datum&) = delete;
    };

    template<typename Type_>
    class TypedDatum : public Datum {
      private:
        Type_* datum;

      public:
        explicit TypedDatum(
            Type_* datum_) :
          datum(datum_) {
          assert(datum != nullptr);

        }

        virtual ~TypedDatum() = default;

        /* -- avoid copying */
        TypedDatum(
            const TypedDatum&) = delete;
        TypedDatum& operator = (
            const TypedDatum&) = delete;

        typename std::add_lvalue_reference<Type_>::type getValue() const noexcept {
          return *datum;
        }
    };

    void doSetDatum(
        const std::string& name_,
        std::unique_ptr<Datum>&& datum_);
    Datum* doGetDatum(
        const std::string& name_) const;


  public:
    /* -- avoid copying */
    UserData(
        const UserData&) = delete;
    UserData& operator = (
        const UserData&) = delete;

    /**
     * @brief Ctor
     */
    UserData();

    /**
     * @brief Dtor
     */
    ~UserData();

    /**
     * @brief Set a user datum
     *
     * @param name_ Name of the datum.
     * @param datum_ The actual datum. The ownership is not taken and the
     *     object must exist for the whole lifetime of this object.
     */
    template<typename Type_>
    void setDatum(
        const std::string& name_,
        Type_* datum_) {
      doSetDatum(name_, ::OTest2::make_unique<TypedDatum<Type_> >(datum_));
    }

    /**
     * @brief Get user datum of specified type
     *
     * @tparam Type_ The requested type of the datum
     * @param name_ Name of the datum
     * @return Reference to the user datum value
     * @exception UserDataMissingException if the datum doesn't exist
     * @exception UserDataWrongTypeException if the requested type is different
     *     than the actual.
     */
    template<typename Type_>
    typename std::add_lvalue_reference<Type_>::type getDatum(
        const std::string& name_) const {
      Datum* d_(doGetDatum(name_));

      /* -- exact type */
      auto* td1_(dynamic_cast<TypedDatum<typename std::remove_reference<Type_>::type>*>(d_));
      if(td1_ != nullptr)
        return td1_->getValue();

      /* -- remove const and volatile modifier */
      auto* td2_(dynamic_cast<TypedDatum<
          typename std::remove_cv<
              typename std::remove_reference<Type_>::type>::type>*>(d_));
      if(td2_ != nullptr)
        return td2_->getValue();

      /* -- invalid type request */
      throw UserDataWrongTypeException(
          name_,
          typeid(*d_).name(),
          typeid(TypedDatum<typename std::remove_reference<Type_>::type>).name());
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2__INCLUDE_OTEST2_USERDATA_H_ */
