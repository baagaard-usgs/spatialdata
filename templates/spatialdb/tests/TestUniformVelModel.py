#!/usr/bin/env python
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

class TestUniformVelModel(unittest.TestCase):

  def setUp(self):
    from spatialdata.spatialdb.contrib.UniformVelModel import UniformVelModel
    db = UniformVelModel()
    db.inventory.label = "Uniform properties"
    db._configure()
    self._db = db

    from spatialdata.geocoords.CSGeo import CSGeo
    csQ = CSGeo()
    csQ.inventory.string = "EPSG:4267"
    csQ._configure()
    self._csQ = csQ


  def test_queryVp(self):
    locs = numpy.array( [[-118.520000,  34.120000,  -1400.00],
                         [-116.400000,  32.340000,  -1000.00]],
                        numpy.float64)
    queryValues = ["vp"]
    dataE = numpy.array([[4.5e+3],
                         [4.5e+3]],
                        numpy.float64)
    errE = [0, 0]
    
    db = self._db
    db.open()
    data, err = db.query(locs, self._csQ, queryValues)
    db.close()    

    self.assertEqual(len(errE), len(err))
    for vE, v in zip(errE, err):
      self.assertEqual(vE, v)

    self.assertEqual(len(dataE.shape), len(data.shape))
    for dE, d in zip(dataE.shape, data.shape):
      self.assertEqual(dE, d)
    for vE, v in zip(numpy.reshape(dataE, -1), numpy.reshape(data, -1)):
      self.assertAlmostEqual(vE, v, 6)


  def test_query(self):
    locs = numpy.array( [[-118.520000,  34.120000,  -1400.00],
                         [-116.400000,  32.340000,  -1000.00]],
                        numpy.float64)
    queryValues = ["density", "vs"]
    dataE = numpy.array([[2.5e+3, 2.6e+3],
                         [2.5e+3, 2.6e+3]],
                        numpy.float64)
    errE = [0, 0]
    
    db = self._db
    db.open()
    data, err = db.query(locs, self._csQ, queryValues)
    db.close()    

    self.assertEqual(len(errE), len(err))
    for vE, v in zip(errE, err):
      self.assertEqual(vE, v)

    self.assertEqual(len(dataE.shape), len(data.shape))
    for dE, d in zip(dataE.shape, data.shape):
      self.assertEqual(dE, d)
    for vE, v in zip(numpy.reshape(dataE, -1), numpy.reshape(data, -1)):
      self.assertAlmostEqual(vE, v, 6)


# End of file 
