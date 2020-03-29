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

# -- the otest2 library
add_library(libotest2
    assertbean.cpp
    assertcontext.cpp
    assertions.cpp
    base64istream.cpp
    base64ostream.cpp
    bzip2istream.cpp
    bzip2ostream.cpp
    case.cpp
    casefactory.cpp
    casegenerated.cpp
    caseordinary.cpp
    caseregistry.cpp
    cmddestroycase.cpp
    cmddestroysuite.cpp
    cmddummy.cpp
    cmdfirststate.cpp
    cmdnextcase.cpp
    cmdnextsuite.cpp
    cmdrunstate.cpp
    cmdstartcase.cpp
    cmdstartsuite.cpp
    cmdstarttest.cpp
    cmdstate.cpp
    cmdstoptest.cpp
    command.cpp
    commandstack.cpp
    context.cpp
    contextobject.cpp
    dfltenvironment.cpp
    dfltloop.cpp
    difflogarray.cpp
    difflogblock.cpp
    difflogbuilder.cpp
    difflogreverse.cpp
    exccatcher.cpp
    exccatcherordinary.cpp
    exctestmarkin.cpp
    internalerror.cpp
    object.cpp
    objectpath.cpp
    registry.cpp
    regressions.cpp
    reporter.cpp
    reporterconsole.cpp
    reporterdebug.cpp
    reporterjunit.cpp
    reporterstatistics.cpp
    reportertee.cpp
    runcode.cpp
    runcode.h
    runner.cpp
    runnerfilter.cpp
    runnerfilterentire.cpp
    runnerfilterone.cpp
    runnerordinary.cpp
    semanticstack.cpp
    state.cpp
    stategenerated.cpp
    stateordinary.cpp
    stateregistry.cpp
    suite.cpp
    suitefactory.cpp
    suitegenerated.cpp
    suiteordinary.cpp
    teeostream.cpp
    terminaldriver.cpp
    testmark.cpp
    testmarkbool.cpp
    testmarkbuilder.cpp
    testmarkdiffprinter.cpp
    testmarkfactory.cpp
    testmarkfloat.cpp
    testmarkhash.cpp
    testmarkin.cpp
    testmarkinbin.cpp
    testmarkinbinios.cpp
    testmarkint.cpp
    testmarklist.cpp
    testmarkmap.cpp
    testmarknull.cpp
    testmarkout.cpp
    testmarkoutbin.cpp
    testmarkoutbinios.cpp
    testmarkprefix.cpp
    testmarkprinter.cpp
    testmarkstorage.cpp
    testmarkstring.cpp
    timesource.cpp
    timesourcesys.cpp
)
set_target_properties(libotest2 PROPERTIES OUTPUT_NAME otest2)
target_include_directories(libotest2 PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(libotest2 PRIVATE ${PROJECT_SOURCE_DIR}/include/otest2)
target_link_libraries(libotest2 INTERFACE tinfo bz2 pugixml)

# -- library installation
install(TARGETS libotest2 DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/otest2 DESTINATION include)