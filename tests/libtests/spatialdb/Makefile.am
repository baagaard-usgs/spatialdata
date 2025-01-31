# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

include $(top_srcdir)/tests/check.am

SUBDIRS = data

TESTS = libtest_spatialdb

check_PROGRAMS = libtest_spatialdb

libtest_spatialdb_SOURCES = \
	TestSpatialDB.cc \
	TestUniformDB.cc \
    TestAnalyticDB.cc \
	TestCxxFunctionDB.cc \
	TestCxxFunctionDB_Cases.cc \
	TestGravityField.cc \
	TestGravityField_Cases.cc \
	TestSimpleDBData.cc \
	TestSimpleDBIO.cc \
	TestSimpleDBQuery.cc \
	TestSimpleDBQuery_Cases.cc \
	TestSimpleDB.cc \
	TestSimpleDB_Cases.cc \
	TestSimpleGridDBData.cc \
	TestSimpleGridDBIO.cc \
	TestSimpleGridDBQuery.cc \
	TestSimpleGridDBQuery_Cases.cc \
	TestSimpleGridDB.cc \
	TestSimpleGridDB_Cases.cc \
	TestCompositeDB.cc \
	TestTimeHistory.cc \
	TestTimeHistoryIO.cc \
	$(top_srcdir)/tests/src/driver_catch2.cc


noinst_HEADERS = \
	TestGravityField.hh \
	TestSimpleDBQuery.hh \
	TestSimpleDB.hh \
	TestSimpleGridDBQuery.hh \
	TestSimpleGridDB.hh \
	TestCxxFunctionDB.hh

# End of file
