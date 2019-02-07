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
AM_CXXFLAGS = -fno-rtti -pthread

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
    lcstream.cpp \
    lcstream.h \
    location.cpp \
    location.h \
    main.cpp \
    options.cpp \
    options.h \
    parser.cpp \
    parser.h \
    vartable.cpp \
    vartable.h
    
otest2_LDFLAGS = \
    -L$(top_srcdir)/lib \
    -lotest2 \
    -lclangTooling \
    -lclangFrontend \
    -lclangFrontendTool \
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
    -lclangBasic \
    -lLLVMAArch64AsmParser \
    -lLLVMAArch64AsmPrinter \
    -lLLVMAArch64CodeGen \
    -lLLVMAArch64Desc \
    -lLLVMAArch64Disassembler \
    -lLLVMAArch64Info \
    -lLLVMAArch64Utils \
    -lLLVMAggressiveInstCombine \
    -lLLVMAMDGPUAsmParser \
    -lLLVMAMDGPUAsmPrinter \
    -lLLVMAMDGPUCodeGen \
    -lLLVMAMDGPUDesc \
    -lLLVMAMDGPUDisassembler \
    -lLLVMAMDGPUInfo \
    -lLLVMAMDGPUUtils \
    -lLLVMAnalysis \
    -lLLVMARMAsmParser \
    -lLLVMARMAsmPrinter \
    -lLLVMARMCodeGen \
    -lLLVMARMDesc \
    -lLLVMARMDisassembler \
    -lLLVMARMInfo \
    -lLLVMARMUtils \
    -lLLVMAsmParser \
    -lLLVMAsmPrinter \
    -lLLVMBitReader \
    -lLLVMBitWriter \
    -lLLVMBPFAsmParser \
    -lLLVMBPFAsmPrinter \
    -lLLVMBPFCodeGen \
    -lLLVMBPFDesc \
    -lLLVMBPFDisassembler \
    -lLLVMBPFInfo \
    -lLLVMCodeGen \
    -lLLVMCore \
    -lLLVMCoroutines \
    -lLLVMCoverage \
    -lLLVMDebugInfoCodeView \
    -lLLVMDebugInfoDWARF \
    -lLLVMDebugInfoMSF \
    -lLLVMDebugInfoPDB \
    -lLLVMDemangle \
    -lLLVMDlltoolDriver \
    -lLLVMExecutionEngine \
    -lLLVMFuzzMutate \
    -lLLVMGlobalISel \
    -lLLVMHexagonAsmParser \
    -lLLVMHexagonCodeGen \
    -lLLVMHexagonDesc \
    -lLLVMHexagonDisassembler \
    -lLLVMHexagonInfo \
    -lLLVMInstCombine \
    -lLLVMInstrumentation \
    -lLLVMInterpreter \
    -lLLVMipo \
    -lLLVMLanaiAsmParser \
    -lLLVMLanaiAsmPrinter \
    -lLLVMLanaiCodeGen \
    -lLLVMLanaiDesc \
    -lLLVMLanaiDisassembler \
    -lLLVMLanaiInfo \
    -lLLVMLibDriver \
    -lLLVMLineEditor \
    -lLLVMLinker \
    -lLLVMLTO \
    -lLLVMMCDisassembler \
    -lLLVMMCJIT \
    -lLLVMMCParser \
    -lLLVMMipsAsmParser \
    -lLLVMMipsAsmPrinter \
    -lLLVMMipsCodeGen \
    -lLLVMMipsDesc \
    -lLLVMMipsDisassembler \
    -lLLVMMipsInfo \
    -lLLVMMIRParser \
    -lLLVMMSP430AsmPrinter \
    -lLLVMMSP430CodeGen \
    -lLLVMMSP430Desc \
    -lLLVMMSP430Info \
    -lLLVMNVPTXAsmPrinter \
    -lLLVMNVPTXCodeGen \
    -lLLVMNVPTXDesc \
    -lLLVMNVPTXInfo \
    -lLLVMObjCARCOpts \
    -lLLVMObject \
    -lLLVMObjectYAML \
    -lLLVMOption \
    -lLLVMOrcJIT \
    -lLLVMPasses \
    -lLLVMPowerPCAsmParser \
    -lLLVMPowerPCAsmPrinter \
    -lLLVMPowerPCCodeGen \
    -lLLVMPowerPCDesc \
    -lLLVMPowerPCDisassembler \
    -lLLVMPowerPCInfo \
    -lLLVMProfileData \
    -lLLVMRuntimeDyld \
    -lLLVMScalarOpts \
    -lLLVMSelectionDAG \
    -lLLVMSparcAsmParser \
    -lLLVMSparcAsmPrinter \
    -lLLVMSparcCodeGen \
    -lLLVMSparcDesc \
    -lLLVMSparcDisassembler \
    -lLLVMSparcInfo \
    -lLLVMSymbolize \
    -lLLVMSystemZAsmParser \
    -lLLVMSystemZAsmPrinter \
    -lLLVMSystemZCodeGen \
    -lLLVMSystemZDesc \
    -lLLVMSystemZDisassembler \
    -lLLVMSystemZInfo \
    -lLLVMTableGen \
    -lLLVMTarget \
    -lLLVMTransformUtils \
    -lLLVMVectorize \
    -lLLVMWindowsManifest \
    -lLLVMXCoreAsmPrinter \
    -lLLVMXCoreCodeGen \
    -lLLVMXCoreDesc \
    -lLLVMXCoreDisassembler \
    -lLLVMXCoreInfo \
    -lLLVMXRay \
    -lLLVMX86AsmParser \
    -lLLVMX86AsmPrinter \
    -lLLVMX86CodeGen \
    -lLLVMX86Desc \
    -lLLVMX86Disassembler \
    -lLLVMX86Info \
    -lLLVMX86Utils \
    -lLLVMMC \
    -lLLVMBinaryFormat \
    -lLLVMSupport \
    -lz \
    -ltinfo
