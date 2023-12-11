#!/usr/bin/env nemesis
#
# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2023, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================
import os
import sys

current = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.dirname(current))
from UnitTestApp import UnitTestApp

import unittest


class TestApp(UnitTestApp):
    """Test application.
    """

    def _suite(self):
        """Setup the test suite.
        """
        suite = unittest.TestSuite()

        from TestNondimensional import TestNondimensional
        suite.addTest(unittest.makeSuite(TestNondimensional))

        from TestNondimElasticQuasistatic import TestNondimElasticQuasistatic
        suite.addTest(unittest.makeSuite(TestNondimElasticQuasistatic))

        from TestNondimElasticDynamic import TestNondimElasticDynamic
        suite.addTest(unittest.makeSuite(TestNondimElasticDynamic))

        return suite


# ----------------------------------------------------------------------
if __name__ == '__main__':
    app = TestApp()
    app.main()


# End of file
