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

# -- code shared by the public library and by the generator
add_library(libotest2common
    exc.cpp
)
set_target_properties(libotest2common PROPERTIES OUTPUT_NAME otest2common)
target_include_directories(libotest2common PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(libotest2common PRIVATE ${PROJECT_SOURCE_DIR}/include/otest2)

# -- the otest2 library
add_library(libotest2
    assertbean.cpp
    assertbuffer.cpp
    assertbufferbase.cpp
    assertbufferstr.cpp
    assertcontext.cpp
    assertions.cpp
    assertionstext.cpp
    assertstream.cpp
    base64istream.cpp
    base64ostream.cpp
    bzip2istream.cpp
    bzip2ostream.cpp
    case.cpp
    casegenerated.cpp
    caseordinary.cpp
    cmddummy.cpp
    cmdfirststate.cpp
    cmdleaveobject.cpp
    cmdnextobject.cpp
    cmdrepeatobject.cpp
    cmdrunstate.cpp
    cmdstartupobject.cpp
    cmdstate.cpp
    cmdteardownobject.cpp
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
    fcemarshaler.cpp
    internalerror.cpp
    object.cpp
    objectpath.cpp
    objectrepeater.cpp
    objectrepeaterfactory.cpp
    objectrepeateronce.cpp
    objectscenario.cpp
    parameters.cpp
    printutils.cpp
    registry.cpp
    regressions.cpp
    repeater.cpp
    reporter.cpp
    reporterconsole.cpp
    reporterjunit.cpp
    reporterstatistics.cpp
    reportertee.cpp
    runcode.cpp
    runcode.h
    runner.cpp
    runnerfilter.cpp
    runnerfilterentire.cpp
    runnerfilteruntagged.cpp
    runnerfiltertags.cpp
    runnerordinary.cpp
    scenario.cpp
    scenariocase.cpp
    scenariocontainer.cpp
    scenarioiter.cpp
    scenarioitercontainer.cpp
    scenarioitercontainer.h
    scenarioroot.cpp
    scenariosuite.cpp
    semanticstack.cpp
    state.cpp
    stategenerated.cpp
    stateordinary.cpp
    stateregistry.cpp
    suite.cpp
    suitegenerated.cpp
    tags.cpp
    tagsstack.cpp
    teeostream.cpp
    terminaldriver.cpp
    testmark.cpp
    testmarkbool.cpp
    testmarkbuilder.cpp
    testmarkdiffprinter.cpp
    testmarkfactory.cpp
    testmarkformatter.cpp
    testmarkformatterassert.cpp
    testmarkformatterios.cpp
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
    testroot.cpp
    timesource.cpp
    timesourcesys.cpp
    userdata.cpp
)
set_target_properties(libotest2 PROPERTIES OUTPUT_NAME otest2)
target_include_directories(libotest2 PRIVATE ${PROJECT_SOURCE_DIR}/include/otest2)
target_link_libraries(libotest2 PUBLIC libotest2common)
target_link_libraries(libotest2 INTERFACE tinfo bz2 pugixml)

# -- library installation
install(TARGETS libotest2common DESTINATION lib EXPORT otest2)
install(TARGETS libotest2 DESTINATION lib EXPORT otest2)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/otest2 DESTINATION include)
