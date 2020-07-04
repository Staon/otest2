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

#ifndef OTest2_INCLUDE_OTEST2_REPEATERVALUESIMPL_H_
#define OTest2_INCLUDE_OTEST2_REPEATERVALUESIMPL_H_

#include <assert.h>
#include <otest2/repeatervalues.h>
#include <utility>

namespace OTest2 {

template<typename Value_>
RepeaterValue<Value_>::RepeaterValue(
    std::vector<Value_>&& values_) :
  values(std::move(values_)),
  index(0) {

}

template<typename Value_>
RepeaterValue<Value_>::~RepeaterValue() {

}

template<typename Value_>
Value_ RepeaterValue<Value_>::getValue() const {
  assert(index < values.size());
  return values[index];
}

template<typename Value_>
int RepeaterValue<Value_>::getIndex() const {
  return index;
}

template<typename Value_>
bool RepeaterValue<Value_>::hasNextRun(
    const Context& context_) const noexcept {
  return index + 1 < values.size();
}

template<typename Value_>
std::shared_ptr<RepeaterValue<Value_>> RepeaterValue<Value_>::createNext(
    const Context& context_,
    std::shared_ptr<RepeaterValue<Value_>>& current_,
    std::vector<Value_>&& values_) {
  if(current_ == nullptr)
    /* -- first run */
    return std::make_shared<RepeaterValue<Value_>>(std::move(values_));
  else {
    /* -- following runs */
    ++current_->index;
    assert(current_->index < current_->values.size());
    return current_;
  }
}

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_REPEATERVALUESIMPL_H_ */
