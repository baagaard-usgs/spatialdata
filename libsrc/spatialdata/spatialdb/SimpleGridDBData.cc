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

#include "SimpleGridDBData.hh" // Implementation of class methods

#include "spatialdata/geocoords/CoordSys.hh" // HASA CoordSys
#include "spatialdata/units/Parser.hh" // HASA Parser

#include <algorithm> // USES std:copy()

#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::logic_error
#include <cassert> // USES assert()

// ----------------------------------------------------------------------
// Constructor
spatialdata::spatialdb::SimpleGridDBData::SimpleGridDBData(void) :
    _numX(0),
    _numY(0),
    _numZ(0),
    _dataDim(0),
    _numValues(0) {}


// ----------------------------------------------------------------------
// Destructor
spatialdata::spatialdb::SimpleGridDBData::~SimpleGridDBData(void) {}


// ----------------------------------------------------------------------
// Allocate data.
void
spatialdata::spatialdb::SimpleGridDBData::allocate(const size_t numX,
                                                   const size_t numY,
                                                   const size_t numZ,
                                                   const size_t numValues,
                                                   const size_t spaceDim,
                                                   const size_t dataDim) {
    _numX = numX;
    _numY = numY;
    _numZ = numZ;
    _numValues = numValues;
    _dataDim = dataDim;

    const size_t numLocs = (3 == spaceDim) ? _numX * _numY * _numZ : (2 == spaceDim) ? _numX * _numY : _numX;
    _data.resize(numLocs*numValues);
    _x.resize(numX);
    _y.resize(numY);
    _z.resize(numZ);
} // allocate


// ----------------------------------------------------------------------
// Deallocate data.
void
spatialdata::spatialdb::SimpleGridDBData::deallocate(void) {
    _data.clear();_data.shrink_to_fit();
    _x.clear();_x.shrink_to_fit();
    _y.clear();_y.shrink_to_fit();
    _z.clear();_z.shrink_to_fit();
    _names.clear();_names.shrink_to_fit();
    _units.clear();_units.shrink_to_fit();
    _numX = 0;
    _numY = 0;
    _numZ = 0;
    _numValues = 0;
    _dataDim = 0;
    _cs.reset();
} // deallocate


// ----------------------------------------------------------------------
// Get spatial dimension.
size_t
spatialdata::spatialdb::SimpleGridDBData::getSpaceDim(void) const {
    assert(_cs);
    return _cs->getSpaceDim();
}


// ----------------------------------------------------------------------
// Set names of data values.
void
spatialdata::spatialdb::SimpleGridDBData::setNames(const std::vector<std::string>& names) {
    if (names.size() != _numValues) {
        std::ostringstream msg;
        msg << "Mismatch in number of values (" << _numValues << " != " << names.size()
            << ") for names of values in SimpleGrid spatial database.";
        throw std::invalid_argument(msg.str());
    } // if
    _names = names;
} // setNames


// ----------------------------------------------------------------------
// Set units of data values.
void
spatialdata::spatialdb::SimpleGridDBData::setUnits(const std::vector<std::string>& units) {
    if (units.size() != _numValues) {
        std::ostringstream msg;
        msg << "Mismatch in number of values (" << _numValues << " != " << units.size()
            << ") for units of values in SimpleGrid spatial database.";
        throw std::invalid_argument(msg.str());
    } // if
    _units = units;
} // setUnits


// ----------------------------------------------------------------------
// Set coordinate system for spatial database.
void
spatialdata::spatialdb::SimpleGridDBData::setCoordSys(std::shared_ptr<spatialdata::geocoords::CoordSys>& cs) {
    _cs = std::move(cs);
}


// ----------------------------------------------------------------------
// Check compatibility of spatial database parameters.
void
spatialdata::spatialdb::SimpleGridDBData::checkCompatibility(void) const {
    assert(_cs);
    const size_t spaceDim = _cs->getSpaceDim();
    const size_t dataDim = _dataDim;
    std::ostringstream msg;

    const size_t numX = _numX;
    const size_t numY = _numY;
    const size_t numZ = _numZ;

    int count1 = 0;
    if (( 0 == numX) || ( 1 == numX) ) {
        count1 += 1;
    } // if
    if (( 0 == numY) || ( 1 == numY) ) {
        count1 += 1;
    } // if
    if (( 0 == numZ) || ( 1 == numZ) ) {
        count1 += 1;
    } // if

    if (( 0 == dataDim) && ( 3 != count1) ) {
        msg << "Dimension of data in spatial distribution (" << dataDim
            << ") is incompatible with dimensions of a 0-D grid  ("
            << numX << "," << numY << "," << numZ << ").";
        throw std::domain_error(msg.str());

    } else if (( 1 == dataDim) && ( 2 != count1) ) {
        msg << "Dimension of data in spatial distribution (" << dataDim
            << ") is incompatible with dimensions of a 1-D grid  ("
            << numX << "," << numY << "," << numZ << ").";
        throw std::domain_error(msg.str());

    } else if (( 2 == dataDim) && ( 1 != count1) ) {
        msg << "Dimension of data in spatial distribution (" << dataDim
            << ") is incompatible with dimensions of a 2-D grid  ("
            << numX << "," << numY << "," << numZ << ").";
        throw std::domain_error(msg.str());

    } else if (( 3 == dataDim) && ( 0 != count1) ) {
        msg << "Dimension of data in spatial distribution (" << dataDim
            << ") is incompatible with dimensions of a 3-D grid  ("
            << numX << "," << numY << "," << numZ << ").";
        throw std::domain_error(msg.str());
    } // if/else

    if (dataDim > spaceDim) {
        msg << "Dimension of data in spatial distribution (" << dataDim
            << ") exceeds the number of dimensions of the coordinates ("
            << spaceDim << ").";
        throw std::domain_error(msg.str());
    } // if

    if (dataDim > spaceDim) {
        msg << "Dimension of data in spatial distribution (" << dataDim
            << ") exceeds the number of dimensions of the coordinates ("
            << spaceDim << ").";
        throw std::domain_error(msg.str());
    } // if
} // _checkCompatibility


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::SimpleGridDBData::toSI(void) {
    spatialdata::units::Parser::toSI(_data.data(), _units, _numX*_numY*_numZ, _numValues);
} // convertToSI


// ----------------------------------------------------------------------
// Bilinear search for coordinate.
double
spatialdata::spatialdb::SimpleGridDBData::search(const double target,
                                                 const std::vector<double>& coordinate,
                                                 const bool nearest) {
    const size_t numLocs = coordinate.size();
    if (numLocs <= 1) {
        return 0.0;
    } // if

    assert(numLocs > 0);

    double index = -1.0;
    size_t indexL = 0;
    size_t indexR = numLocs - 1;
    const double tolerance = 1.0e-6;
    if (( target >= coordinate[indexL]-tolerance) && ( target <= coordinate[indexR]+tolerance) ) {
        while (indexR - indexL > 1) {
            size_t indexM = indexL + (indexR-indexL) / 2;
            if (target < coordinate[indexM]) {
                indexR = indexM;
            } else {
                indexL = indexM;
            } // if/else
        } // while
        assert(target >= coordinate[indexL]-tolerance);
        assert(coordinate[indexR] > coordinate[indexL]);
        index = double(indexL) + (target - coordinate[indexL]) / (coordinate[indexR] - coordinate[indexL]);
    } else if (nearest) {
        if (target <= coordinate[indexL]) {
            index = 0.0;
        } else {
            index = double(numLocs-1);
        } // if/else
    } // if/else

    return index;
} // search


// ----------------------------------------------------------------------
// Adjust indices to account for optimizations for lower dimension
// distribution.
void
spatialdata::spatialdb::SimpleGridDBData::reindex2d(double* const index0,
                                                    size_t* const size0,
                                                    double* const index1,
                                                    size_t* const size1) const {
    assert(index0);
    assert(index1);
    assert(size0);
    assert(size1);

    *size0 = _numX;
    *size1 = _numY;
    if (( 1 == _dataDim) && ( 1 == _numX) ) {
        *index0 = *index1;
        *size0 = *size1;
        *index1 = 0;
        *size1 = 1;
    } // if
} // reindex2d


// ----------------------------------------------------------------------
// Adjust indices to account for optimizations for lower dimension
// distribution.
void
spatialdata::spatialdb::SimpleGridDBData::reindex3d(double* const index0,
                                                    size_t* const size0,
                                                    double* const index1,
                                                    size_t* const size1,
                                                    double* const index2,
                                                    size_t* const size2) const {
    assert(index0);
    assert(index1);
    assert(index2);
    assert(size0);
    assert(size1);
    assert(size2);

    *size0 = _numX;
    *size1 = _numY;
    *size2 = _numZ;
    if (2 == _dataDim) {
        if (1 == _numX) {
            *index0 = *index1;
            *size0 = *size1;
            *index1 = *index2;
            *size1 = *size2;
            *index2 = 0;
            *size2 = 1;
        } else if (1 == _numY) {
            *index1 = *index2;
            *size1 = *size2;
            *index2 = 0;
            *size2 = 1;
        } // if/else
    } else if (1 == _dataDim) {
        if (_numY > 1) {
            *index0 = *index1;
            *size0 = *size1;
            *index1 = 0;
            *size1 = 1;
            *index2 = 0;
            *size2 = 1;
        } else if (_numZ > 1) {
            *index0 = *index2;
            *size0 = *size2;
            *index1 = 0;
            *size1 = 1;
            *index2 = 0;
            *size2 = 1;
        } // if
    } // if/else
} // reindex3d


// ----------------------------------------------------------------------
// Get index into data array.
size_t
spatialdata::spatialdb::SimpleGridDBData::getDataIndex(const double* const coords,
                                                       const size_t spaceDim) const {
    assert(coords);
    const bool nearest = false;

    double index0 = 0;
    double index1 = 0;
    double index2 = 0;
    size_t size0 = 0;
    size_t size1 = 0;
    size_t size2 = 0;
    if (spaceDim > 2) {
        index0 = std::floor(SimpleGridDBData::search(coords[0], _x, nearest)+0.5);
        index1 = std::floor(SimpleGridDBData::search(coords[1], _y, nearest)+0.5);
        index2 = std::floor(SimpleGridDBData::search(coords[2], _z, nearest)+0.5);
        reindex3d(&index0, &size0, &index1, &size1, &index2, &size2);
    } else if (spaceDim > 1) {
        index0 = std::floor(SimpleGridDBData::search(coords[0], _x, nearest)+0.5);
        index1 = std::floor(SimpleGridDBData::search(coords[1], _y, nearest)+0.5);
        reindex2d(&index0, &size0, &index1, &size1);
    } else {
        assert(1 == spaceDim);
        index0 = std::floor(SimpleGridDBData::search(coords[0], _x, nearest)+0.5);
    } // if

    const size_t indexData = getDataIndex(size_t(index0), size0, size_t(index1), size1, size_t(index2), size2);
    return indexData;
} // _getDataIndex


// End of file
