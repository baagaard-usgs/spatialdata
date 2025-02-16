# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

check-local: check-TESTS
	for f in $(TEST_LOGS); do printf "\n=== $$f ===\n\n"; cat $$f; done


TESTS = test_contrib.py
dist_check_SCRIPTS = test_contrib.py

noinst_PYTHON = \
	TestUniformVelModel.py 


# End of file 
