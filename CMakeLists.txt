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

cmake_minimum_required(VERSION 3.7)

project(OTest2 VERSION 1.0.0)

# -- disable warning of the deprecated istrstream
add_compile_options(-Wno-deprecated)

add_subdirectory(lib)
add_subdirectory(otest2)
add_subdirectory(test)

# -- declare the cmake package
include(CMakePackageConfigHelpers)

# -- generate the package's configuration file
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/Config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION lib/cmake/otest2
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
# -- generate the version file
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Version.cmake
    VERSION "${OTest2_VERSION_MAJOR}.${OTest2_VERSION_MINOR}.${OTest2_VERSION_PATCH}"
    COMPATIBILITY SameMajorVersion
)
# -- install the generated files
install(EXPORT otest2 FILE otest2.cmake DESTINATION lib/cmake/otest2)
install(
    FILES
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Version.cmake
        ${PROJECT_SOURCE_DIR}/cmake/target-sources.cmake
        ${PROJECT_SOURCE_DIR}/cmake/template-main.cpp
    DESTINATION lib/cmake/otest2
)

# -- I want to use the package in the example projects. Copy requested files
#    into the binary directory and register the package into the user's
#    registry so that the package might not be installed.
export(EXPORT otest2 FILE "${CMAKE_CURRENT_BINARY_DIR}/otest2.cmake")
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/target-sources.cmake
    ${PROJECT_BINARY_DIR}
    COPYONLY)
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/template-main.cpp
    ${PROJECT_BINARY_DIR}
    COPYONLY)
export(PACKAGE OTest2)

# -- building of binary packages
include(InstallRequiredSystemLibraries)

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${OTest2_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${OTest2_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${OTest2_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "Ondrej Starek <stareko@email.cz>")

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libtinfo-dev, libbz2-dev, libpugixml-dev, libboost-dev")

include(CPack)
