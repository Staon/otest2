/*
 * Copyright (C) 2018 Ondrej Starek
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

#ifndef OTest2__INCLUDE_OTEST2_OBJECTPTR_H_
#define OTest2__INCLUDE_OTEST2_OBJECTPTR_H_

#include <memory>

namespace OTest2 {

class Object;

/**
 * @brief Pointer to a testing object
 */
template<typename Object_>
class ObjectPtr {
  private:
    std::shared_ptr<Object> object_ptr;
    Object_* type_ptr;
    template<typename O2_> friend class ObjectPtr;

    ObjectPtr(
        std::shared_ptr<Object> object_ptr_,
        Object_* type_ptr_) :
      object_ptr(object_ptr_),
      type_ptr(type_ptr_) {

    }

  public:
    /**
     * @brief Default ctor - null pointer
     */
    ObjectPtr() :
      object_ptr(),
      type_ptr(nullptr) {

    }

    /**
     * @brief Ctor
     *
     * @param object_ The pointed object. The ownership is taken.
     */
    ObjectPtr(
        Object_* object_) :
      object_ptr(object_),
      type_ptr(object_) {

    }

    /**
     * @brief Copy with possible conversion
     *
     * @param src_ Source pointer
     */
    template<typename SrcObject_>
    ObjectPtr(
        const ObjectPtr<SrcObject_>& src_) :
      object_ptr(src_.object_ptr),
      type_ptr(src_.type_ptr) {

    }

    template<typename SrcObject_>
    ObjectPtr(
        ObjectPtr<SrcObject_>&& src_) :
      object_ptr(src_.object_ptr),
      type_ptr(src_.type_ptr) {

    }

    template<typename SrcObject_>
    ObjectPtr& operator = (
        const ObjectPtr<SrcObject_>& src_) {
      object_ptr = src_.object_ptr;
      type_ptr = src_.type_ptr;
      return *this;
    }

    template<typename SrcObject_>
    ObjectPtr& operator = (
        ObjectPtr<SrcObject_>&& src_) {
      object_ptr = src_.object_ptr;
      type_ptr = src_.type_ptr;
      return *this;
    }

    /* -- copy and move semantics */
    ObjectPtr(
        const ObjectPtr&) = default;
    ObjectPtr& operator =(
        const ObjectPtr&) = default;
    ObjectPtr(
        ObjectPtr&&) = default;
    ObjectPtr& operator =(
        ObjectPtr&&) = default;

    /**
     * @brief Dtor
     */
    ~ObjectPtr() = default;

    /**
     * @brief Check whether the pointer is null
     */
    bool isNull() const {
      return type_ptr == nullptr;
    }

    /**
     * @brief Get the object
     */
    Object_* operator->() const {
      return type_ptr;
    }

    /**
     * @brief Get the object
     */
    Object_* get() const {
      return type_ptr;
    }

    /**
     * @brief Static cast
     */
    template<typename DstObject_>
    ObjectPtr<DstObject_> staticCast() const {
      return ObjectPtr<DstObject_>(
          object_ptr, static_cast<DstObject_*>(type_ptr));
    }
};

/**
 * @brief Exception safe creation of an object
 */
template<typename Object_, typename... Args_>
inline ObjectPtr<Object_> makePointer(Args_&&... args_) {
  return new Object_(std::forward<Args_>(args_)...);
}

} /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_OBJECTPTR_H_ */
