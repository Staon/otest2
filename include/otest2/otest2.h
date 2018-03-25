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

#ifndef OTest2__INCLUDE_OTEST2_OTEST2_H_
#define OTest2__INCLUDE_OTEST2_OTEST2_H_

#ifndef OTEST2_PARSER_ACTIVE

#define TEST_START_UP()
#define TEST_TEAR_DOWN()
#define TEST_SUITE(name_) void name_()
#define TEST_CASE(name_)
#define TEST_STATE(name_)

#else /* -- OTEST2_PARSER_ACTIVE */

namespace OTest2 {

struct StartUpMark { };
struct TearDownMark { };
struct CaseMark { };
struct StateMark { };

} /* -- namespace OTest2 */

#define TEST_START_UP() OTest2::StartUpMark otest2_start_up_mark;
#define TEST_TEAR_DOWN() OTest2::TearDownMark otest2_tear_down_mark;
#define TEST_SUITE(name_) void name_() __attribute__((annotate("otest2::suite")))
#define TEST_CASE(name_) OTest2::CaseMark name_;
#define TEST_STATE(name_) OTest2::StateMark name_;

#endif /* -- OTEST2_PARSER_ACTIVE */

#endif /* -- OTest2__INCLUDE_OTEST2_OTEST2_H_ */
