#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(otest2, 1.0, foo@foo)
AM_INIT_AUTOMAKE
LT_INIT

AC_CONFIG_MACRO_DIRS([m4])

AC_CONFIG_SRCDIR([lib/exc.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET

# Check support for C++11 flag
AX_CXX_COMPILE_STDCXX(11, noext)

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lclang':
AC_CHECK_LIB([clang], [clang_createIndex],,[AC_MSG_ERROR(The libclang is not detected!)])

# Checks for header files.
AC_CHECK_HEADERS

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

# Checks libclang headers
AC_CHECK_HEADER(clang-c/Index.h,,[AC_MSG_ERROR(The header clang-c/Index.h is not detected!)])

AC_CONFIG_FILES([Makefile lib/Makefile otest2/Makefile])
AC_OUTPUT
