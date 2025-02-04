# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

subpackage = units
include $(top_srcdir)/subpackage.am
include $(top_srcdir)/modulesrc/module.am

subpkgpython_LTLIBRARIES = _units.la

_units_la_SOURCES = \
	Nondimensional_wrap.cc \
	units.cc

_units_la_LDFLAGS = -module -avoid-version \
	$(AM_LDFLAGS) $(PYTHON_LA_LDFLAGS)

_units_la_LIBADD = \
	$(top_builddir)/libsrc/spatialdata/libspatialdata.la \
	$(PYTHON_BLDLIBRARY) $(PYTHON_LIBS) $(PYTHON_SYSLIBS)


# End of file
