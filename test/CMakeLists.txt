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

function(TARGET_OTEST2_SOURCES target)
  set(one_value_args DOMAIN)
  cmake_parse_arguments(OTEST2 "" "${one_value_args}" "" ${ARGN})
  
  if(OTEST2_DOMAIN)
    set(domain_arg "-d${OTEST2_DOMAIN}")
  else()
    set(domain_arg "")
  endif()

  foreach(src IN LISTS OTEST2_UNPARSED_ARGUMENTS)
    set(gensrc "${CMAKE_CURRENT_SOURCE_DIR}/${src}.cpp")
    add_custom_command(
        OUTPUT ${gensrc}
        COMMAND otest2
            -I${PROJECT_SOURCE_DIR}/include
            -I/usr/local/lib/clang/7.1.0/include
            ${domain_arg}
            -o ${gensrc} ${CMAKE_CURRENT_SOURCE_DIR}/${src}
        DEPENDS ${src}
    )
    target_sources(${target} PRIVATE ${gensrc})
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${gensrc})
  endforeach()
endfunction()

add_executable(selftest
    main.cpp
    reportermock.cpp
    runtime.cpp
    timesourcemock.cpp
)
target_otest2_sources(selftest
    assertions.ot2
    base64.ot2
    bzip2.ot2
    exceptions.ot2
    fixtures.ot2
    hirschberg.ot2
    mainloop.ot2
    regressions.ot2
    tee.ot2
    testmarks.ot2
)
target_otest2_sources(selftest DOMAIN selftest
    selftests/assertions.ot2
    selftests/exceptions.ot2
    selftests/fixtures.ot2
    selftests/mainloop.ot2
    selftests/regressions.ot2
)
target_link_libraries(selftest PRIVATE libotest2)

add_custom_target(check
    COMMAND selftest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
