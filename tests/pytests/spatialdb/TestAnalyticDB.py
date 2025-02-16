# =================================================================================================
# This code is part of SpatialData, developed through the Computational Infrastructure
# for Geodynamics (https://github.com/geodynamics/spatialdata).
#
# Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
# All rights reserved.
#
# See https://mit-license.org/ and LICENSE.md and for license information. 
# =================================================================================================

import unittest

import numpy
from spatialdata.testing.TestCases import make_suite
from pythia.pyre.units.length import m, km, cm


class TestAnalyticDB(unittest.TestCase):

    def setUp(self):
        from spatialdata.spatialdb.AnalyticDB import AnalyticDB
        db = AnalyticDB("TestAnalyticDB")
        db.inventory.label = "test"
        db.inventory.values = ["one", "two", "three"]
        db.inventory.units = ["none", "km", "cm"]
        db.inventory.expressions = ["1.0+2.0*x", "y/z", "1.0+2.0*x+4.0*y"]
        db._configure()
        self._db = db

    def test_database(self):
        locs = numpy.array([[1.0, 2.0, 3.0], [5.6, 4.2, 8.6]], numpy.float64)
        queryValues = ["three", "one"]
        dataE = numpy.array([[(1.0+2.0*x+4.0*y)*0.01, (1.0+2.0*x)*1.0] for x,y,z in locs])
        errE = [0, 0]

        from spatialdata.geocoords.CSCart import CSCart
        cs = CSCart()
        cs._configure()

        db = self._db
        db.open()
        data, err = db.query(locs, cs, queryValues)
        db.close()

        self.assertEqual(len(errE), len(err))
        for vE, v in zip(errE, err):
            self.assertEqual(vE, v)

        self.assertEqual(len(dataE.shape), len(data.shape))
        for dE, d in zip(dataE.shape, data.shape):
            self.assertEqual(dE, d)
        for vE, v in zip(numpy.reshape(dataE, -1), numpy.reshape(data, -1)):
            self.assertAlmostEqual(vE, v, 6)


def load_tests(loader, tests, pattern):
    TEST_CLASSES = [TestAnalyticDB]
    return make_suite(TEST_CLASSES, loader)


if __name__ == "__main__":
    unittest.main(verbosity=2)


# End of file
