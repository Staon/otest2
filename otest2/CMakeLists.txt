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

find_package(Clang REQUIRED CONFIG)

add_executable(otest2
    error.cpp
    exc.cpp
    excparser.cpp
    excreader.cpp
    filereader.cpp
    formatting.cpp
    generator.cpp
    generatorprinter.cpp
    generatorstd.cpp
    generfmt.cpp
    lcstream.cpp
    location.cpp
    main.cpp
    options.cpp
    parser.cpp
    typetemplate.cpp
    vartable.cpp
)
set_property(TARGET otest2 PROPERTY CXX_STANDARD 14)
target_compile_options(otest2 PRIVATE -fno-rtti -fexceptions)
target_link_libraries(otest2 PRIVATE clangTooling)

# -- install the otest2 preprocessor
install(TARGETS otest2 DESTINATION bin EXPORT otest2)
install(PROGRAMS otest2_discovery.sh DESTINATION bin)
