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

include(${PROJECT_SOURCE_DIR}/cmake/target-sources.cmake)

add_executable(selftest EXCLUDE_FROM_ALL
    reportermock.cpp
    runtime.cpp
    timesourcemock.cpp
)
target_otest2_sources(selftest
    assertions.ot2
    base64.ot2
    bzip2.ot2
    dsl.ot2
    exceptions.ot2
    fixtures.ot2
    fixtureobject.ot2
    hirschberg.ot2
    itemwise.ot2
    lexicographical.ot2
    longtexts.ot2
    mainloop.ot2
    maps.ot2
    regressions.ot2
    repeaters.ot2
#    tags.ot2
    tee.ot2
    testmarks.ot2
    userdata.ot2
)
target_otest2_sources(selftest DOMAIN selftest
    selftests/assertions.ot2
    selftests/dsl.ot2
    selftests/exceptions.ot2
    selftests/fixtures.ot2
    selftests/fixtureobject.ot2
    selftests/itemwise.ot2
    selftests/lexicographical.ot2
    selftests/longtexts.ot2
    selftests/mainloop.ot2
    selftests/maps.ot2
    selftests/regressions.ot2
    selftests/repeaters.ot2
#    selftests/tags.ot2
    selftests/userdata.ot2
)
target_otest2_main(selftest)
target_link_libraries(selftest PRIVATE libotest2)

# -- make check 
add_custom_target(check
    COMMAND selftest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# -- CircleCI tests - the JUnit reporting files
add_custom_target(check-circleci
    COMMAND selftest --junit=../test-results/selftest/result.xml
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
