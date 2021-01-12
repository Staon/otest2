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

#ifndef OTest2__INCLUDE_OTEST2_ASSERTIONANNOTATION_H_
#define OTest2__INCLUDE_OTEST2_ASSERTIONANNOTATION_H_

#ifndef OTEST2_PARSER_ACTIVE

#define TEST_ASSERTION_MARK(class_, method_)
#define TEST_ASSERTION_MARK_TMPL(class_, method_)
#define TEST_ASSERTION_EXPR_MARK(class_, generic_method_, comparison_method_)

#else /* -- OTEST2_PARSER_ACTIVE */

#define TEST_ASSERTION_MARK(class_, method_) __attribute__((annotate("otest2::assertion(" #class_ ";" #method_ ")")))
#define TEST_ASSERTION_MARK_TMPL(class_, method_) __attribute__((annotate("otest2::assertion(" class_ ";" method_ ")")))
#define TEST_ASSERTION_EXPR_MARK(class_, generic_method_, comparison_method_) __attribute__((annotate("otest2::assertionExpr(" class_ ";" generic_method_ ";" comparison_method_ ")")))

#endif /* -- OTEST2_PARSER_ACTIVE */

#endif /* -- OTest2__INCLUDE_OTEST2_ASSERTIONANNOTATION_H_ */
