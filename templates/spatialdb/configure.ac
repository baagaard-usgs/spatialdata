# -*- autoconf -*-
#
# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

AC_PREREQ(2.59)
AC_INIT([spatialdbcontrib], [0.1.0], [baagaard@usgs.gov])
AC_CONFIG_HEADER([portinfo])
AC_CONFIG_AUX_DIR([./aux-config])
AC_CONFIG_MACRO_DIR([./m4])
AM_INIT_AUTOMAKE([foreign])

# ----------------------------------------------------------------------
# C/C++/libtool/install
AC_PROG_CXX
AC_PROG_CC
AC_DISABLE_STATIC
AM_PROG_CC_C_O

AC_PROG_LIBTOOL
AC_PROG_INSTALL

dnl Require C++-14
AX_CXX_COMPILE_STDCXX(14)

dnl ----------------------------------------------------------------------
dnl PROJ
CIT_PROJ6_HEADER
CIT_PROJ6_LIB

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

# SPATIALDATA
AC_LANG(C++)
AC_CHECK_HEADER([spatialdata/spatialdb/SpatialDB.hh], [], [
  AC_MSG_ERROR([SpatialDB header not found; try CPPFLAGS="-I<Spatialdata include dir>"])
])
AC_MSG_CHECKING([for spatialdb::SimpleDB in -lspatialdata])
AC_REQUIRE_CPP
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[#include <spatialdata/spatialdb/SpatialDB.hh>]
                    [#include <spatialdata/spatialdb/SimpleDB.hh>]],
                   [[spatialdata::spatialdb::SimpleDB db("SimpleDB");]])],
  [AC_MSG_RESULT(yes)],
  [AC_MSG_RESULT(no)
   AC_MSG_ERROR([Spatialdata library not found; try LDFLAGS="-L<Spatialdata lib dir>"])
])


# ENDIANNESS
AC_C_BIGENDIAN

# ----------------------------------------------------------------------
AC_CONFIG_FILES([Makefile
	         tests/Makefile])

AC_OUTPUT


# End of file
