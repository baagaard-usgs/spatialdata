#!/usr/bin/env nemesis
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

import unittest

import numpy
from spatialdata.testing.TestCases import make_suite

class TestTimeHistory(unittest.TestCase):

    def test_timehistory(self):
        timeQ = numpy.array([0.5, 0.0, 0.6, 2.0, 5.0, 20.0, 8.0], dtype=numpy.float64)
        amplitudeE = numpy.array([1.0, 0.0, 1.2, 4.0, 2.5, 0.0, 1.0], dtype=numpy.float64)
        errE = numpy.array([0, 0, 0, 0, 0, 1, 0], dtype=numpy.float64)

        from spatialdata.spatialdb.TimeHistory import TimeHistory
        th = TimeHistory()
        th.inventory.label = "test"
        th.inventory.filename = "data/timehistory.timedb"
        th._configure()

        th.open()
        amplitude, err = th.query(timeQ)
        th.close()

        self.assertEqual(len(errE), len(err))
        for vE, v in zip(errE, err):
            self.assertEqual(vE, v)

        self.assertEqual(len(amplitudeE.shape), len(amplitude.shape))
        for dE, d in zip(amplitudeE.shape, amplitude.shape):
            self.assertEqual(dE, d)
        for vE, v in zip(numpy.reshape(amplitudeE, -1), numpy.reshape(amplitude, -1)):
            self.assertAlmostEqual(vE, v, 6)

    def test_io(self):
        """
        Test write().
        """
        filename = "data/test.timedb"
        time = numpy.array([0.0, 2.0, 6.0, 7.0, 10.0], dtype=numpy.float64)
        amplitude = numpy.array([0.0, 0.2, 0.1, -0.1, 2.0], dtype=numpy.float64)
        units = "year"

        # Write database
        from spatialdata.spatialdb import TimeHistoryIO
        TimeHistoryIO.write(time, amplitude, units, filename)

        # Test write
        filenameE = "data/test_okay.timedb"

        fin = open(filenameE, "r", encoding="utf-8")
        linesE = fin.readlines()
        fin.close()

        fin = open(filename, "r", encoding="utf-8")
        lines = fin.readlines()
        fin.close()

        self.assertEqual(len(linesE), len(lines))
        iline = 0
        for (lineE, line) in zip(linesE, lines):
            if lineE != line:
                print("Error found in line %d in file '%s' is incorrect." % (iline, filename))
                self.assertTrue(False)
            iline += 1


def load_tests(loader, tests, pattern):
    TEST_CLASSES = [TestTimeHistory]
    return make_suite(TEST_CLASSES, loader)


if __name__ == "__main__":
    unittest.main(verbosity=2)


# End of file
