# Copyright (C) 2020 Ondrej Starek
#
# This file is part of OTest2.
#
# OTest2 is free software: you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
#
# OTest2 is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with OTest2.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.17)

project(otest2-example-sections)

find_package(OTest2)

add_executable(sections "")
target_otest2_sources(sections
    sections.ot2
)
target_otest2_main(sections)
target_link_libraries(sections libotest2)

add_custom_target(check ALL COMMAND sections DEPENDS sections)

