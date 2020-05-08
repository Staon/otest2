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
    error.h
    excparser.cpp
    excparser.h
    excreader.cpp
    excreader.h
    filereader.cpp
    filereader.h
    formatting.cpp
    formatting.h
    function.cpp
    function.h
    functions.cpp
    functions.h
    generator.cpp
    generator.h
    generatorstd.cpp
    generatorstd.h
    generfmt.cpp
    generfmt.h
    lcstream.cpp
    lcstream.h
    location.cpp
    location.h
    main.cpp
    options.cpp
    options.h
    parser.cpp
    parser.h
    parserannotation.cpp
    parserannotation.h
    parserannotationimpl.h
    parserassert.cpp
    parserassert.h
    parsercase.cpp
    parsercase.h
    parsercode.cpp
    parsercode.h
    parsercontext.cpp
    parsercontext.h
    parsercontextimpl.h
    parserfixture.cpp
    parserfixture.h
    parserfunction.cpp
    parserfunction.h
    parsersuite.cpp
    parsersuite.h
    parsertype.cpp
    parsertype.h
    typetemplate.cpp
    typetemplate.h
    vartable.cpp
    vartable.h
)
set_property(TARGET otest2 PROPERTY CXX_STANDARD 14)
target_compile_options(otest2 PRIVATE -fno-rtti -fexceptions)
target_link_libraries(otest2 PRIVATE clangTooling libotest2common)

# -- install the otest2 preprocessor
install(TARGETS otest2 DESTINATION bin EXPORT otest2)
install(PROGRAMS otest2_discovery.sh DESTINATION bin)
