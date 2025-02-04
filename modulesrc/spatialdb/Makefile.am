# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

subpackage = spatialdb
include $(top_srcdir)/subpackage.am
include $(top_srcdir)/modulesrc/module.am

subpkgpython_LTLIBRARIES = _spatialdb.la

_spatialdb_la_SOURCES = \
	SpatialDB_wrap.cc \
	AnalyticDB_wrap.cc \
	UniformDB_wrap.cc \
	SimpleDB_wrap.cc \
	SimpleDBData_wrap.cc \
	SimpleDBIO_wrap.cc \
	SimpleGridDB_wrap.cc \
	SimpleGridDBData_wrap.cc \
	SimpleGridDBIO_wrap.cc \
	CompositeDB_wrap.cc \
	GravityField_wrap.cc \
	TimeHistory_wrap.cc \
	TimeHistoryIO_wrap.cc \
	spatialdb.cc


_spatialdb_la_LDFLAGS = -module -avoid-version \
	$(AM_LDFLAGS) $(PYTHON_LA_LDFLAGS)

_spatialdb_la_LIBADD = \
	$(top_builddir)/libsrc/spatialdata/libspatialdata.la \
	$(PYTHON_BLDLIBRARY) $(PYTHON_LIBS) $(PYTHON_SYSLIBS)


# End of file
