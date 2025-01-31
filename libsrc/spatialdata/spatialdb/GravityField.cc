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

#include "GravityField.hh" // Implementation of class methods

#include "spatialdata/geocoords/CoordSys.hh" // USES CoordSys
#include "spatialdata/geocoords/CSGeo.hh" // USES CSGeo

#include <cmath> // USES sqrt()
#include <strings.h> // USES strcasecmp()
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringsgream
#include <assert.h> // USES assert()

namespace spatialdata {
    namespace spatialdb {
        class _GravityField;
    } // spatialdb
} // spatialdata

class spatialdata::spatialdb::_GravityField {
public:

    static const size_t numValues;
    static const std::vector<std::string> valueNames;
};
const size_t spatialdata::spatialdb::_GravityField::numValues = 3;
const std::vector<std::string> spatialdata::spatialdb::_GravityField::valueNames = {
    "gravity_field_x",
    "gravity_field_y",
    "gravity_field_z",
};

// ----------------------------------------------------------------------
/// Default constructor
spatialdata::spatialdb::GravityField::GravityField(void) :
    SpatialDB("Gravity field"),
    _acceleration(9.80665), // m/s^2
    _querySize(3) {
    _gravityDir[0] = +0.0;
    _gravityDir[1] = +0.0;
    _gravityDir[2] = -1.0;

    _queryValues[0] = 0;
    _queryValues[1] = 1;
    _queryValues[2] = 2;
} // constructor


// ----------------------------------------------------------------------
/// Default destructor
spatialdata::spatialdb::GravityField::~GravityField(void) {
    _querySize = 0;
} // destructor


// ----------------------------------------------------------------------
// Set direction of gravitational body force.
void
spatialdata::spatialdb::GravityField::setGravityDir(const double x,
                                                    const double y,
                                                    const double z) {
    const double mag = std::max(1.0e-8, sqrt(x*x + y*y + z*z));
    _gravityDir[0] = x / mag;
    _gravityDir[1] = y / mag;
    _gravityDir[2] = z / mag;
} // setGravityDir


// ----------------------------------------------------------------------
// Set gravitational acceleration.
void
spatialdata::spatialdb::GravityField::setGravityAcc(const double acceleration) {
    _acceleration = acceleration;
} // setGravityAcc


// ----------------------------------------------------------------------
// Open the database and prepare for querying.
void
spatialdata::spatialdb::GravityField::open(void) {
}


// ----------------------------------------------------------------------
// Close the database.
void
spatialdata::spatialdb::GravityField::close(void) {
}


// ----------------------------------------------------------------------
// Get names of values in spatial database.
const std::vector<std::string>&
spatialdata::spatialdb::GravityField::getNamesDBValues(void) const {
    return _GravityField::valueNames;
} // getNamesDBValues


// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
spatialdata::spatialdb::GravityField::setQueryValues(const std::vector<std::string>& names) {
    const size_t numNames = names.size();
    if (0 == numNames) {
        std::ostringstream msg;
        msg << "Number of values (" << numNames << ") for query of gravity field spatial database " << getDescription()
            << " must be positive.\n";
        throw std::invalid_argument(msg.str());
    } else if (numNames > 3) {
        std::ostringstream msg;
        msg << "Number of values (" << numNames << ") for query of gravity field spatial database " << getDescription()
            << "\n must be 1, 2, or 3.\n";
        throw std::invalid_argument(msg.str());
    } // if/else

    _querySize = numNames;
    for (size_t iVal = 0; iVal < numNames; ++iVal) {
        if (0 == strcasecmp(names[iVal].c_str(), _GravityField::valueNames[0].c_str())) {
            _queryValues[iVal] = 0;
        } else if (0 == strcasecmp(names[iVal].c_str(), _GravityField::valueNames[1].c_str())) {
            _queryValues[iVal] = 1;
        } else if (0 == strcasecmp(names[iVal].c_str(), _GravityField::valueNames[2].c_str())) {
            _queryValues[iVal] = 2;
        } else {
            std::ostringstream msg;
            msg << "Could not find value '" << names[iVal] << "' in spatial database '"
                << getDescription() << "'. Available values are: "
                << "'" << _GravityField::valueNames[0] << "', "
                << "'" << _GravityField::valueNames[1] << "', "
                << "'" << _GravityField::valueNames[2] << "'.";
            throw std::out_of_range(msg.str());
        } // if
    } // for
} // queryVals


// ----------------------------------------------------------------------
// Query the database.
int
spatialdata::spatialdb::GravityField::query(double* values,
                                            const size_t numValues,
                                            const double* coordinates,
                                            const spatialdata::geocoords::CoordSys* csCoordinates) {
    assert(!numValues || values);
    assert(coordinates);
    assert(csCoordinates);
    const size_t spaceDim = csCoordinates->getSpaceDim();

    if (0 == _querySize) {
        std::ostringstream msg;
        msg << "Values to be returned by spatial database " << getDescription() << "\n"
            << "have not been set. Please call setQueryValues() before query().\n";
        throw std::logic_error(msg.str());
    } else if (numValues != _querySize) {
        std::ostringstream msg;
        msg << "Number of values to be returned by spatial database "
            << getDescription() << "\n"
            << "(" << _querySize << ") does not match size of array provided ("
            << numValues << ").\n";
        throw std::logic_error(msg.str());
    } // if

    if (geocoords::CoordSys::CARTESIAN == csCoordinates->getCSType()) {
        for (size_t i = 0; i < _querySize; ++i) {
            values[i] = _acceleration*_gravityDir[_queryValues[i]];
        } // for
    } else {
        const geocoords::CSGeo* csGeo = dynamic_cast<const geocoords::CSGeo*>(csCoordinates);
        double surfaceNormal[3];
        const int numLocs = 1;
        csGeo->computeSurfaceNormal(surfaceNormal, coordinates, numLocs, spaceDim);
        for (size_t i = 0; i < _querySize; ++i) {
            values[i] = -_acceleration * surfaceNormal[_queryValues[i]];
        } // for
    } // if/else

    return 0;
} // query


// End of file
