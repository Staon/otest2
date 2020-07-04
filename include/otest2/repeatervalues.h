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

#ifndef OTest2_INCLUDE_OTEST2_REPEATERVALUES_H_
#define OTest2_INCLUDE_OTEST2_REPEATERVALUES_H_

#include <initializer_list>
#include <memory>
#include <otest2/repeater.h>
#include <vector>

namespace OTest2 {

/**
 * @brief Generic repeater for a list of values
 *
 * This repeater repeats the testing object once for each specified value.
 */
template<typename Value_>
class RepeaterValue : public Repeater {
  private:
    std::vector<Value_> values;
    int index;

  public:
    /* -- avoid copying */
    RepeaterValue(
        const RepeaterValue&) = delete;
    RepeaterValue& operator = (
        const RepeaterValue&) = delete;

    /**
     * @brief Ctor - initial values
     */
    explicit RepeaterValue(
        std::vector<Value_>&& values_);

    /**
     * @brief Ctor
     *
     * @param values_ The list of values
     */
    explicit RepeaterValue(
        std::initializer_list<Value_> values_);

    /**
     * @brief Ctor - initial values
     */
    template<typename Container_>
    explicit RepeaterValue(
        Container_&& values_);

    /**
     * @brief Ctor - initial values
     */
    template<typename Iter_>
    RepeaterValue(
        Iter_ begin_,
        Iter_ end_);

    /**
     * @brief Dtor
     */
    virtual ~RepeaterValue();

    /**
     * @brief Get current value
     */
    Value_ getValue() const;

    /**
     * @brief Get current value index
     */
    int getIndex() const;

    /* -- repeater interface */
    virtual bool hasNextRun(
        const Context& context_) const noexcept;

    /**
     * @brief Factory function of particular runs
     *
     * @param context_ The OTest2 context
     * @param current_ Current repeater instance
     * @param values_ Initial values
     * @return New instance of the repeater
     */
    static std::shared_ptr<RepeaterValue> createNext(
        const Context& context_,
        std::shared_ptr<RepeaterValue>& current_,
        std::vector<Value_>&& values_);

    /**
     * @brief Factory function of particular runs
     *
     * @param context_ The OTest2 context
     * @param current_ Current repeater instance
     * @param values_ Initial values. Any container which can be converted
     *     to the std::vector instance.
     * @return New instance of the repeater
     */
    template<typename Container_>
    static std::shared_ptr<RepeaterValue> createNext(
        const Context& context_,
        std::shared_ptr<RepeaterValue>& current_,
        Container_&& values_) {
      return createNext(
          context_,
          current_,
          std::vector<Value_>(std::forward<Container_>(values_)));
    }

    /**
     * @brief Factory function of particular runs
     *
     * @param context_ The OTest2 context
     * @param current_ Current repeater instance
     * @param begin_ Begin of the range of initial values
     * @param end_ End of the range
     * @return New instance of the repeater
     */
    template<typename Iter_>
    static std::shared_ptr<RepeaterValue> createNext(
        const Context& context_,
        std::shared_ptr<RepeaterValue>& current_,
        Iter_ begin_,
        Iter_ end_) {
      return createNext(context_, current_, std::vector<Value_>(begin_, end_));
    }
};

} /* -- namespace OTest2 */

#endif /* -- OTest2_INCLUDE_OTEST2_REPEATERVALUES_H_ */
