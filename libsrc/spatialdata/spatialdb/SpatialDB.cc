// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include <portinfo>

#include "SpatialDB.hh" // Implementation of class methods

#include "spatialdata/geocoords/CoordSys.hh" // USES CoordSys

#include <cassert> // USES assert()
#include <vector> // USES std::vector

// Include ios here to avoid some Python/gcc issues
#include <ios>

#include "spatialdata/units/Parser.hh" // USES Parser

// ----------------------------------------------------------------------
/// Constructor with description
spatialdata::spatialdb::SpatialDB::SpatialDB(const char* description) :
    _description(description ? description : ":UNKNOWN:") {}


// ----------------------------------------------------------------------
/// Default destructor
spatialdata::spatialdb::SpatialDB::~SpatialDB(void) {}


// ----------------------------------------------------------------------
// Get description of spatial database.
void
spatialdata::spatialdb::SpatialDB::setDescription(const char* description) {
    _description = description ? description : ":UNKNOWN:";
}


// ----------------------------------------------------------------------
// Get description of spatial database.
const char*
spatialdata::spatialdb::SpatialDB::getDescription(void) const {
    return _description.c_str();
}


// ----------------------------------------------------------------------
// Perform multiple queries of the database.
void
spatialdata::spatialdb::SpatialDB::multiquery(double* values,
                                              int* err,
                                              const double* coordinates,
                                              const size_t numLocs,
                                              const size_t numValues,
                                              const spatialdata::geocoords::CoordSys* csQuery) {
    assert(csQuery);
    const size_t spaceDim = csQuery->getSpaceDim();
    assert( (!values && 0 == numValues && 0 == numValues) ||
            (values && numValues > 0 && numValues > 0) );
    assert( (!err && 0 == numLocs) || (err && numLocs > 0) );
    assert( (!coordinates && 0 == numLocs && 0 == spaceDim) ||
            (coordinates && numLocs > 0 && spaceDim > 0) );

    for (size_t i = 0, indexV = 0, indexC = 0;
         i < numValues;
         ++i, indexV += numValues, indexC += spaceDim) {
        err[i] = query(&values[indexV], numValues, &coordinates[indexC], csQuery);
    } // for
} // multiquery


// End of file
