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
#ifndef OTest2__OTEST2_TYPETEMPLATE_H_
#define OTest2__OTEST2_TYPETEMPLATE_H_

#include <iosfwd>
#include <string>
#include <vector>

namespace OTest2 {

namespace Parser {

/**
 * @brief Expand one template
 *
 * @param os_ A stream which the template is expanded in
 * @param template_ The template
 * @param args_ List of template's parameters
 * @return False if the template cannot be parsed
 */
bool expandTemplate(
    std::ostream& os_,
    const std::string& template_,
    const std::vector<std::string>& args_);

/**
 * @brief Expand one template
 *
 * @param[in,out] template_ The template
 * @param[in] args_ List of templates' parameters
 * @return False if the templates cannot be parsed
 */
bool expandTemplate(
    std::string& templates_,
    const std::vector<std::string>& args_);

} /* -- namespace Parser */

} /* namespace OTest2 */

#endif /* OTest2__OTEST2_TYPETEMPLATE_H_ */
