# Copyright (C) 2018 Ondrej Starek
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
#
include_flags = -I$(top_srcdir)/include -I/usr/lib/gcc/x86_64-linux-gnu/5/include/
AM_CPPFLAGS = $(include_flags)
AM_CXXFLAGS = -Wall -Wno-comment -Wno-unused-variable -Werror

OTEST2 = $(top_srcdir)/otest2/otest2
SUFFIXES = .ot2
.ot2.cpp:
	$(OTEST2) -o $@ $(include_flags) $<

check_PROGRAMS = selftest

selftest_SOURCES = \
	assertions.ot2 \
	main.cpp \
	runtime.cpp

selftest_LDADD = \
	../lib/.libs/libotest2.la \
	./selftests/.libs/libselftest.la \
	-l:libtinfo.a
	
TESTS = $(check_PROGRAMS)

.PHONY = test
test: $(check_PROGRAMS)

SUBDIRS = selftests
