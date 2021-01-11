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
#ifndef OTest2__INCLUDE_OTEST2_CONTROLS_H_
#define OTest2__INCLUDE_OTEST2_CONTROLS_H_

#ifndef OTEST2_PARSER_ACTIVE

#define TEST_CONTROLS_SWITCH_STATE()

#else /* -- OTEST2_PARSER_ACTIVE */

#define TEST_CONTROLS_SWITCH_STATE() __attribute__((annotate("otest2::switchState")))

#endif /* -- OTEST2_PARSER_ACTIVE */

namespace OTest2 {

namespace Controls {

/**
 * @brief Prepare next test state
 *
 * This method prepares run of a state, which will be run after leaving of
 * current test state.
 *
 * @param state_ The next state
 * @param delay_ Delay in milliseconds between end of the current state
 *     and run of the next state.
 */
template<typename... Args_>
void switchState(
    void (*state_)(Args_... args_),
    int delay_) TEST_CONTROLS_SWITCH_STATE();

}  /* -- namespace Controls */

}  /* -- namespace OTest2 */

#endif /* OTest2__INCLUDE_OTEST2_CONTROLS_H_ */
