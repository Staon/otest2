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
