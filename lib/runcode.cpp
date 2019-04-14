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

#include "runcode.h"

#include <exception>
#include <sstream>
#include <string>

#include <context.h>
#include <exccatcher.h>
#include <reporter.h>
#include <semanticstack.h>

namespace OTest2 {

bool runUserCode(
    const Context& context_,
    std::function<void(const Context&)> ftor_) {
  /* -- prepare the return value */
  context_.semantic_stack->push(true);

  /* -- run the functor */
  std::string message_;
  if(context_.exception_catcher->catchException(context_, ftor_, message_)) {
    /* -- fail the testing object */
    context_.semantic_stack->setTop(false);

    /* -- report the failure */
    context_.reporter->reportError(context_, message_);
  }

  /* -- handle the return value */
  bool retval_(context_.semantic_stack->top());
  context_.semantic_stack->popAnd();

  return retval_;
}

} /* -- namespace OTest2 */
