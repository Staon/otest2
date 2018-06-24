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
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/include/otest2
AM_CXXFLAGS =

bin_PROGRAMS = otest2

otest2_SOURCES = \
    error.cpp \
    error.h \
    excparser.cpp \
    excparser.h \
    excreader.cpp \
    excreader.h \
    filereader.cpp \
    filereader.h \
    formatting.cpp \
    formatting.h \
    generator.cpp \
    generator.h \
    generatorprinter.cpp \
    generatorprinter.h \
    generatorstd.cpp \
    generatorstd.h \
    location.cpp \
    location.h \
    main.cpp \
    parser.cpp \
    parser.h \
    vartable.cpp \
    vartable.h
    
otest2_LDFLAGS = \
    -L$(top_srcdir)/lib \
    -lotest2 \
    -lclangTooling \
    -lclangFrontend \
    -lclangSerialization \
    -lclangDriver \
    -lclangRewriteFrontend \
    -lclangRewrite \
    -lclangParse \
    -lclangSema \
    -lclangAnalysis \
    -lclangAST \
    -lclangASTMatchers \
    -lclangEdit \
    -lclangLex \
    -lclangBasic
