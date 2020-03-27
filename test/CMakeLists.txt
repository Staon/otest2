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

add_custom_command(
    OUTPUT testmarks.ot2.cpp
    COMMAND otest2 -I${PROJECT_SOURCE_DIR}/include -I/usr/local/lib/clang/7.1.0/include -o testmarks.ot2.cpp ${CMAKE_CURRENT_SOURCE_DIR}/testmarks.ot2
    DEPENDS testmarks.ot2
)

add_executable(selftest
    main.cpp
    reportermock.cpp
    runtime.cpp
    timesourcemock.cpp
    
    testmarks.ot2.cpp
)
target_link_libraries(selftest PRIVATE libotest2)
