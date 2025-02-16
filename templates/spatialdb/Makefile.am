# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

# This files uses autoconf patterns in variables names so that the GNU
# build system can automatically construct the dependencies and build
# the library using the appropriate flags.

# m4 macros for GNU build system
ACLOCAL_AMFLAGS = -I ./m4

SUBDIRS = tests

# Define path where python and module will be installed
subpkgpythondir = $(pythondir)/spatialdata/spatialdb/contrib


# C++ LIBRARY -------------------------------------------------------------------------------------
lib_LTLIBRARIES = libspatialdbcontrib.la

# Source files for library
libspatialdbcontrib_la_SOURCES = \
	UniformVelModel.cc

# C++ Header files
noinst_HEADERS = \
	UniformVelModel.hh


libspatialdbcontrib_la_LDFLAGS = $(AM_LDFLAGS) $(PYTHON_LDFLAGS) $(PYTHON_LA_LDFLAGS)
libspatialdbcontrib_la_LIBADD = \
	-lproj \
	-lspatialdata \
	$(PYTHON_BLDLIBRARY) $(PYTHON_LIBS) $(PYTHON_SYSLIBS)

AM_CPPFLAGS = -I$(top_srcdir)/libsrc
AM_CPPFLAGS += -DDATADIR=$(pkgdatadir)/geocoords -I$(PYTHON_INCDIR)

# pybind11 ----------------------------------------------------------------------------------------
subpkgpython_LTLIBRARIES = _spatialdbcontrib.la

_spatialdbcontrib_la_SOURCES = \
	UniformVelModel_wrap.cc \
	spatialdbcontrib.cc

_spatialdbcontrib_la_CPPFLAGS = \
	-I. \
	$(PYBIND11_CPPFLAGS)

_spatialdbcontrib_la_LDFLAGS = -module -avoid-version \
	$(AM_LDFLAGS) $(PYTHON_LA_LDFLAGS)

_spatialdbcontrib_la_LIBADD = \
	$(top_builddir)/libspatialdbcontrib.la \
	$(PYTHON_BLDLIBRARY) $(PYTHON_LIBS) $(PYTHON_SYSLIBS)

# Python API --------------------------------------------------------------------------------------
subpkgpython_PYTHON = \
	UniformVelModel.py \
	__init__.py


# End of file
