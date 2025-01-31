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

#include "SimpleDBData.hh" // Implementation of class methods

#include "SpatialDB.hh" // USES SpatialDB::convertToSI()
#include "spatialdata/geocoords/CoordSys.hh" // HOLDSA CoordSys
#include "spatialdata/units/Parser.hh" // HOLDSA Parser

#include <algorithm> // USES std:copy()

#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringsgream

// ----------------------------------------------------------------------
// Default constructor
spatialdata::spatialdb::SimpleDBData::SimpleDBData(void) :
    _numLocs(0),
    _numValues(0),
    _dataDim(0) {}


// ----------------------------------------------------------------------
// Default destructor
spatialdata::spatialdb::SimpleDBData::~SimpleDBData(void) {
    _numLocs = 0;
    _numValues = 0;
    _dataDim = 0;
} // destructor


// ----------------------------------------------------------------------
// Allocate data structures.
void
spatialdata::spatialdb::SimpleDBData::allocate(const size_t numLocs,
                                               const size_t numValues,
                                               const size_t spaceDim,
                                               const size_t dataDim) {
    if (0 == numLocs) {
        std::ostringstream msg;
        msg << "Number of locations (" << numLocs << ") must be positive.";
        throw std::invalid_argument(msg.str());
    } // if
    if (0 == numValues) {
        std::ostringstream msg;
        msg << "Number of values (" << numValues << ") must be positive.";
        throw std::invalid_argument(msg.str());
    } // if
    if (0 == spaceDim) {
        std::ostringstream msg;
        msg << "Number of spatial dimensions (" << spaceDim << ") must be positive.";
        throw std::invalid_argument(msg.str());
    } // if
    if (dataDim > 3) {
        std::ostringstream msg;
        msg << "Spatial dimension of data (" << dataDim << ") must be in the range [0,3].";
        throw std::out_of_range(msg.str());
    } // if

    size_t size = numLocs*numValues;
    _data.resize(size);

    size = numLocs*spaceDim;
    _coordinates.resize(size);

    size = numValues;
    _names.resize(size);
    _units.resize(size);

    _numLocs = numLocs;
    _numValues = numValues;
    _dataDim = dataDim;
} // allocate


// ----------------------------------------------------------------------
// Deallocate data.
void
spatialdata::spatialdb::SimpleDBData::deallocate(void) {
    _data.clear();_data.shrink_to_fit();
    _coordinates.clear();_coordinates.shrink_to_fit();
    _names.clear();_names.shrink_to_fit();
    _units.clear();_units.shrink_to_fit();
    _numLocs = 0;
    _numValues = 0;
    _dataDim = 0;
    _cs.reset();
} // deallocate


// ----------------------------------------------------------------------
// Get spatial dimension of locations.
size_t
spatialdata::spatialdb::SimpleDBData::getSpaceDim(void) const {
    assert(_cs);
    return _cs->getSpaceDim();
}


// ----------------------------------------------------------------------
// Set names of values.
void
spatialdata::spatialdb::SimpleDBData::setNames(const std::vector<std::string>& names) {
    if (names.size() != _numValues) {
        std::ostringstream msg;
        msg << "Mismatch in number of values (" << _numValues << " != " << names.size()
            << ") for names of values in SimpleDB spatial database.";
        throw std::invalid_argument(msg.str());
    } // if
    _names = names;
} // names


// ----------------------------------------------------------------------
// Set units of values.
void
spatialdata::spatialdb::SimpleDBData::setUnits(const std::vector<std::string>& units) {
    if (units.size() != _numValues) {
        std::ostringstream msg;
        msg << "Mismatch in number of values (" << _numValues << " != " << units.size()
            << ") for units of values in SimpleDB spatial database.";
        throw std::invalid_argument(msg.str());
    } // if
    _units = units;
} // units


// ----------------------------------------------------------------------
// Set coordinate system for spatial database.
void
spatialdata::spatialdb::SimpleDBData::setCoordSys(std::shared_ptr<spatialdata::geocoords::CoordSys>& cs) {
    _cs = std::move(cs);
}


// ----------------------------------------------------------------------
// Get coordinates of location in database.
const double*
spatialdata::spatialdb::SimpleDBData::getCoordinates(const size_t index) const {
    assert(index >= 0 && index < _numLocs);
    assert(_cs);
    return &_coordinates[index*_cs->getSpaceDim()];
}


// ----------------------------------------------------------------------
// Get coordinates of location in database.
double*
spatialdata::spatialdb::SimpleDBData::getCoordinates(const size_t index) {
    assert(index >= 0 && index < _numLocs);
    assert(_cs);
    return &_coordinates[index*_cs->getSpaceDim()];
}


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::SimpleDBData::checkCompatibility(void) const {
    assert(_cs);

    const size_t numLocs = _numLocs;
    const size_t spaceDim = _cs->getSpaceDim();
    const size_t dataDim = _dataDim;
    std::ostringstream msg;
    if (numLocs < 1 + dataDim) {
        msg << "Spatial distribution with data dimensions of " << dataDim
            << " must have at least " << 1+dataDim << " points.\n"
            << "Found " << numLocs << " points in distribution.";
        throw std::domain_error(msg.str());
    } // if
    if (( 0 == dataDim) && ( numLocs > 1) ) {
        msg << "Spatial distribution with data dimensions of " << dataDim
            << " cannot have more than one point.\n"
            << "Found " << numLocs << " points in distribution.";
        throw std::domain_error(msg.str());
    } // if
    if (dataDim > spaceDim) {
        msg << "Dimension of data in spatial distribution (" << dataDim
            << ") exceeds the number of dimensions of the coordinates ("
            << spaceDim << ").";
        throw std::domain_error(msg.str());
    } // if
} // checkCompatibility


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::SimpleDBData::toSI(void) {
    spatialdata::units::Parser::toSI(_data.data(), _units, _numLocs, _numValues);
} // convertToSI


// End of file
