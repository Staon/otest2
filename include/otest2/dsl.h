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

#ifndef OTest2__INCLUDE_OTEST2_DSL_H_
#define OTest2__INCLUDE_OTEST2_DSL_H_

#ifndef OTEST2_PARSER_ACTIVE

/* -- test structure description */
#define OT2_START_UP()
#define OT2_TEAR_DOWN()
#define OT2_SUITE(name_) namespace name_ { using namespace ::OTest2::Assertions; using namespace ::OTest2::Controls; } namespace name_
#define OT2_CASE(name_) namespace name_ { using namespace ::OTest2::Assertions; using namespace ::OTest2::Controls; } namespace name_
#define OT2_SCENARIO()
#define OT2_STATE()
#define OT2_SIMPLE() void AnonymousState() OT2_SCENARIO()

/* -- asserted try/catch block */
#define testTry try
#define testCatch(exc_type_, name_) catch(typename ::OTest2::TypePack<exc_type_>::Type name_)

/* -- user data variables */
#define OT2_USER_DATA()
#define OT2_USER_DATA_KEY(key_)

/* -- tags */
#define OT2_TAGS(tags_)

#else /* -- OTEST2_PARSER_ACTIVE */

#define OT2_START_UP() __attribute__((annotate("otest2::startUp")))
#define OT2_TEAR_DOWN() __attribute__((annotate("otest2::tearDown")))
#define OT2_SUITE(name_) namespace name_ { using namespace ::OTest2::Assertions; using namespace ::OTest2::Controls; } namespace name_ __attribute__((annotate("otest2::suite")))
#define OT2_CASE(name_) namespace name_ { using namespace ::OTest2::Assertions; using namespace ::OTest2::Controls; } namespace name_ __attribute__((annotate("otest2::case")))
#define OT2_SCENARIO() __attribute__((annotate("otest2::scenario")))
#define OT2_STATE() __attribute__((annotate("otest2::state")))
#define OT2_SIMPLE() void AnonymousState() OT2_SCENARIO()

#define testTry try
#define testCatch(exc_type_, name_) catch(typename ::OTest2::TypePack<exc_type_>::Type __attribute__((annotate("otest2::catch"))) name_ )

#define OT2_USER_DATA() __attribute__((annotate("otest2::userData()")))
#define OT2_USER_DATA_KEY(key_) __attribute__((annotate("otest2::userData(" key_ ")")))

#define OT2_TAGS(tags_) __attribute__((annotate("otest2::tags(" tags_ ")")))

#endif /* -- OTEST2_PARSER_ACTIVE */

#define OT2_SECTION(name_) if(::OTest2::Controls::Private::sectionName(#name_))

/* -- compact DSL syntax */
#define TEST_START_UP() void startUp() OT2_START_UP()
#define TEST_TEAR_DOWN() void tearDown() OT2_TEAR_DOWN()
#define TEST_SUITE(name_) OT2_SUITE(name_)
#define TEST_CASE(name_) OT2_CASE(name_)
#define TEST_SCENARIO(name_) void name_() OT2_SCENARIO()
#define TEST_STATE(name_) void name_() OT2_STATE()
#define TEST_SIMPLE() OT2_SIMPLE()

#endif /* -- OTest2__INCLUDE_OTEST2_DSL_H_ */
