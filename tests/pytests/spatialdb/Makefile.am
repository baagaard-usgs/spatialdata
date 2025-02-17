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

TESTS = test_spatialdb.py
dist_check_SCRIPTS = test_spatialdb.py

noinst_PYTHON = \
	TestCompositeDB.py \
	TestGenSimpleDBApp.py \
	TestGravityField.py \
	TestSimpleIOAscii.py \
	TestSimpleDB.py \
	TestUniformDB.py \
	TestSimpleGridDB.py \
	TestUserFunctionDB.py \
	TestAnalyticDB.py \
	TestTimeHistory.py \
	TestTimeHistoryIO.py

dist_noinst_DATA = \
	gensimpledb.cfg \
	testgen1din2d.odb

# 'export' the input files by performing a mock install
export_datadir = $(top_builddir)/tests/pytests/spatialdb
export-data: $(data_DATA)
	if [ "X$(top_srcdir)" != "X$(top_builddir)" ]; then for f in $(dist_noinst_DATA); do $(install_sh_DATA) $(srcdir)/$$f $(export_datadir); done; fi

clean-data:
	if [ "X$(top_srcdir)" != "X$(top_builddir)" ]; then for f in $(dist_noinst_DATA); do $(RM) $(RM_FLAGS) $(export_datadir)/$$f; done; fi

BUILT_SOURCES = export-data
clean-local: clean-data

# End of file
