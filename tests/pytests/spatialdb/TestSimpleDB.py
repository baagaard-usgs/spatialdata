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
from spatialdata.spatialdb.SimpleDB import SimpleDB
from spatialdata.geocoords.CSCart import CSCart


class TestSimpleDB(unittest.TestCase):

    def setUp(self):
        from spatialdata.spatialdb.SimpleDB import SimpleDB
        db = SimpleDB()
        db.inventory.label = "test"
        db.inventory.queryType = "nearest"
        db.inventory.filename = "data/simple.spatialdb"
        db._configure()
        self._db = db

    def test_database(self):
        locs = numpy.array([[1.0, 2.0, 3.0], [5.6, 4.2, 8.6]], numpy.float64)
        queryValues = ["two", "one"]
        dataE = numpy.array([[4.7, 6.3]] * 2, numpy.float64)
        errE = [0, 0]

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

    def test_io(self):
        """
        Test write().
        """
        # Database info
        cs = CSCart()
        cs._configure()

        filename = "data/test.spatialdb"
        dbData = {
            'points': numpy.array(
                [[1.0, 2.0, 3.0],
                [0.5, 3.0, -3.0]],
                numpy.float64),
            'coordsys': cs,
            'data_dim': 1,
            'values': [
                {
                    'name': "One",
                    'units': "m",
                    'data': numpy.array([2.0, 8.0], numpy.float64),
                },
                {
                    'name': "Two",
                    'units': "m",
                    'data': numpy.array([-2.0, 3.0], numpy.float64),
                }
            ]
        }
        dataDim = 1

        qlocs = numpy.array([
            [0.875, 2.25, 1.5],
            [0.6, 2.8, -1.8],
            [1.0, 2.0, 3.0]],
            numpy.float64)
        valuesE = numpy.array([
            [-0.75, 3.5],
            [2.0, 6.8],
            [-2.0, 2.0]],
            numpy.float64)
        errE = [0, 0, 0]

        # Write database
        from spatialdata.spatialdb import SimpleDBIO
        SimpleDBIO.write(dbData, filename)

        # Test write using query
        db = SimpleDB()
        db.inventory.label = "test"
        db.inventory.queryType = "linear"
        db.inventory.filename = filename
        db._configure()

        db.open()
        values, err = db.query(qlocs, cs, ["two", "one"])
        db.close()

        self.assertEqual(len(valuesE.shape), len(values.shape))
        for dE, d in zip(valuesE.shape, values.shape):
            self.assertEqual(dE, d)
        for vE, v in zip(numpy.reshape(valuesE, -1), numpy.reshape(values, -1)):
            self.assertAlmostEqual(vE, v, 6)


def load_tests(loader, tests, pattern):
    TEST_CLASSES = [TestSimpleDB]
    return make_suite(TEST_CLASSES, loader)


if __name__ == "__main__":
    unittest.main(verbosity=2)


# End of file
