dnl ================================================================================================
dnl This code is part of SpatialData, developed through the Computational Infrastructure
dnl for Geodynamics (https://github.com/geodynamics/spatialdata).
dnl
dnl Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
dnl All rights reserved.
dnl
dnl See https://mit-license.org/ and LICENSE.md and for license information. 
dnl ================================================================================================

AC_PREREQ(2.59)
AC_INIT([spatialdata], [3.1.3], [baagaard@usgs.gov])
AC_CONFIG_HEADER([portinfo])
AC_CONFIG_AUX_DIR([./aux-config])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign subdir-objects tar-pax])

dnl ----------------------------------------------------------------------
dnl TESTING w/Catch2 and Python unittest
AC_ARG_ENABLE([testing],
	[  --enable-testing        Enable unit testing with Catch2 (requires Catch2) [[default=no]]],
	[if test "$enableval" = yes ; then enable_testing=yes; else enable_testing=no; fi],
	[enable_testing=no])
AM_CONDITIONAL([ENABLE_TESTING], [test "$enable_testing" = yes])

dnl TEST COVERAGE w/locv and python-coverage
AC_ARG_ENABLE([test-coverage],
	[  --enable-test-coverage  Enable test coverage with lcov and python-coverage [[default=no]]],
	[if test "$enableval" = yes ; then enable_test_coverage=yes; else enable_test_coverage=no; fi],
	[enable_test_coverage=no])
AM_CONDITIONAL([ENABLE_TEST_COVERAGE], [test "$enable_test_coverage" = yes])
AC_ARG_WITH([python-coverage],
    [AC_HELP_STRING([--with-python-coverage],
        [set executable for python-coverage @<:@default=coverage2@:>@])],
	[python_coverage=$withval],
	[python_coverage="coverage2"])
AC_SUBST(python_coverage)

dnl ----------------------------------------------------------------------
AC_PROG_CXX
AC_PROG_CC
AC_DISABLE_STATIC
AM_PROG_CC_C_O

AC_PROG_LIBTOOL
AC_PROG_INSTALL

dnl Require C++-14 (Catch2)
AX_CXX_COMPILE_STDCXX(14)

dnl ----------------------------------------------------------------------
dnl PROJ
CIT_PROJ6_HEADER
CIT_PROJ6_LIB

dnl CATCH2
if test "$enable_testing" = "yes" ; then
  CIT_CATCH2_HEADER
  CIT_CATCH2_LIB
fi

dnl Check for Python modules and packages.
AM_PATH_PYTHON([3.8])
AC_ARG_VAR(PYTHON, [Python interpreter])
AC_ARG_VAR(PYTHONPATH, [Python module search path])
CIT_PYTHON_SYSCONFIG
CIT_CHECK_PYTHON_HEADER
CIT_CHECK_PYTHON_SHARED

dnl PYTHIA/PYRE
CIT_PYTHON_MODULE([pythia],[1.1.0])

dnl NUMPY
CIT_NUMPY_PYTHON_MODULE
CIT_PYTHON_MODULE([numpy], [1.20.0])

dnl pybind11
CIT_PYTHON_MODULE([pybind11],[2.0.0])
CIT_PYBIND11_INCLUDES

dnl TEST COVERAGE
if test "$enable_test_coverage" = "yes" ; then
  AC_PATH_PROG(LCOV, lcov)
  if test -z "$LCOV"; then
    AC_MSG_FAILURE([cannot find 'lcov' program for C++ test coverage.])
  fi
  AC_PATH_PROG(PYTHON_COVERAGE, $python_coverage)
  if test -z "$PYTHON_COVERAGE"; then
    AC_MSG_FAILURE([cannot find '$python_coverage' program for Python test coverage.])
  fi
fi

dnl VERSION
CIG_PKG_GIT(SPATIALDATA)
AC_DEFINE_UNQUOTED([SPATIALDATA_VERSION], ["$PACKAGE_VERSION"], [Define Spatialdata version])

dnl ENDIANNESS
AC_C_BIGENDIAN

dnl ----------------------------------------------------------------------
AC_CONFIG_FILES([Makefile
	libsrc/Makefile
	libsrc/spatialdata/Makefile
	libsrc/spatialdata/geocoords/Makefile
	libsrc/spatialdata/spatialdb/Makefile
	libsrc/spatialdata/units/Makefile
	libsrc/spatialdata/utils/Makefile
	libsrc/spatialdata/muparser/Makefile
	modulesrc/Makefile
	modulesrc/include/Makefile
	modulesrc/geocoords/Makefile
	modulesrc/spatialdb/Makefile
	modulesrc/units/Makefile
	modulesrc/utils/Makefile
	spatialdata/Makefile
	tests/Makefile
	tests/src/Makefile
	tests/libtests/Makefile
	tests/libtests/geocoords/Makefile
	tests/libtests/spatialdb/Makefile
	tests/libtests/spatialdb/data/Makefile
	tests/libtests/units/Makefile
	tests/libtests/utils/Makefile
	tests/pytests/Makefile
	tests/pytests/geocoords/Makefile
	tests/pytests/spatialdb/Makefile
	tests/pytests/spatialdb/data/Makefile
	tests/pytests/units/Makefile
	tests/pytests/utils/Makefile
	templates/Makefile
	docs/Makefile])

AC_OUTPUT


dnl End of file
