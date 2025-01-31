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

#include "SimpleGridDBQuery.hh" // Implementation of class methods

#include "SimpleGridDBData.hh" // USES SimpleGridDBData
#include "SimpleGridDB.hh" // USES SimpleGridDB::NEAREST

#include "spatialdata/geocoords/CoordSys.hh" // USES CoordSys
#include "spatialdata/geocoords/Converter.hh" // USES Converter

#include <cmath> // USES std::floor()

#include <fstream> // USES std::ifstream
#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::logic_error
#include <cstring> // USES memcpy()
#include <strings.h> // USES strcasecmp()
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
/** Constructor
 *
 * @param[in] data Spatial database data.
 * @param[in] description Spatial database description.
 */
spatialdata::spatialdb::SimpleGridDBQuery::SimpleGridDBQuery(const SimpleGridDBData& data,
                                                             const char* description) :
    _data(data),
    _description(description),
    _queryType(SimpleGridDB::NEAREST),
    _converter(new spatialdata::geocoords::Converter) {}


// ----------------------------------------------------------------------
/// Destructor
spatialdata::spatialdb::SimpleGridDBQuery::~SimpleGridDBQuery(void) {}


// ----------------------------------------------------------------------
// Set query type.
void
spatialdata::spatialdb::SimpleGridDBQuery::setQueryType(const SimpleGridDB::QueryEnum value) {
    _queryType = value;
} // setQueryType


// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
spatialdata::spatialdb::SimpleGridDBQuery::setQueryValues(const std::vector<std::string>& names) {
    if (0 == names.size()) {
        std::ostringstream msg;
        msg
            << "Number of values for query in spatial database " << _description
            << "\n must be positive.\n";
        throw std::invalid_argument(msg.str());
    } // if

    const size_t querySize = names.size();
    _queryIndices.clear();
    _queryIndices.reserve(querySize);
    for (size_t iQuery = 0; iQuery < querySize; ++iQuery) {
        size_t iName = 0;
        const size_t numNames = _data.getNumValues();
        while (iName < numNames) {
            if (0 == strcasecmp(names[iQuery].c_str(), _data.getName(iName))) {
                break;
            } // if
            ++iName;
        } // while
        if (iName >= numNames) {
            std::ostringstream msg;
            msg << "Could not find value '" << names[iQuery] << "' in spatial database '"
                << _description << "'. Available values are:";
            for (size_t iName = 0; iName < numNames; ++iName) {
                msg << "\n  " << _data.getName(iName);
            } // for
            msg << "\n";
            throw std::out_of_range(msg.str());
        } // if
        _queryIndices.emplace_back(iName);
    } // for
} // queryVals


// ----------------------------------------------------------------------
// Query the database.
int
spatialdata::spatialdb::SimpleGridDBQuery::query(double* values,
                                                 const size_t numValues,
                                                 const double* coordinates,
                                                 const spatialdata::geocoords::CoordSys* csCoordinates) {
    assert(!numValues || values);
    assert(coordinates);
    assert(csCoordinates);

    const size_t querySize = _queryIndices.size();
    const size_t spaceDim = csCoordinates->getSpaceDim();

    if (0 == querySize) {
        std::ostringstream msg;
        msg << "Values to be returned by spatial database " << _description << "\n"
            << "have not been set. Please call setQueryValues() before query().\n";
        throw std::logic_error(msg.str());
    } else if (numValues != querySize) {
        std::ostringstream msg;
        msg << "Number of values to be returned by spatial database "
            << _description << "\n"
            << "(" << querySize << ") does not match size of array provided ("
            << numValues << ").\n";
        throw std::invalid_argument(msg.str());
    } else if (spaceDim != _data.getSpaceDim()) {
        std::ostringstream msg;
        msg << "Spatial dimension (" << spaceDim
            << ") does not match spatial dimension of spatial database (" << _data.getSpaceDim() << ").";
        throw std::invalid_argument(msg.str());
    } // if

    // Convert coordinates
    assert(spaceDim <= 3);
    std::copy(coordinates, coordinates+spaceDim, _xyz);
    _converter->convert(_xyz, 1, spaceDim, _data.getCoordSys(), csCoordinates);

    const bool nearest = SimpleGridDB::NEAREST == _queryType;

    double index0 = 0.0;
    double index1 = 0.0;
    double index2 = 0.0;
    size_t size0 = 0;
    size_t size1 = 0;
    size_t size2 = 0;
    if (3 == spaceDim) {
        index0 = SimpleGridDBData::search(_xyz[0], _data.getX(), nearest);
        index1 = SimpleGridDBData::search(_xyz[1], _data.getY(), nearest);
        index2 = SimpleGridDBData::search(_xyz[2], _data.getZ(), nearest);
        _data.reindex3d(&index0, &size0, &index1, &size1, &index2, &size2);
    } else if (2 == spaceDim) {
        index0 = SimpleGridDBData::search(_xyz[0], _data.getX(), nearest);
        index1 = SimpleGridDBData::search(_xyz[1], _data.getY(), nearest);
        _data.reindex2d(&index0, &size0, &index1, &size1);
    } else { // else
        assert(1 == spaceDim);
        index0 = SimpleGridDBData::search(_xyz[0], _data.getX(), nearest);
        size0 = _data.getNumX();
    } // if/else

    switch (_queryType) {
    case SimpleGridDB::LINEAR:
        if (( index0 < 0.0) || (( index0 > 0) && ( index0 > size0-1.0) ) ||
            ( index1 < 0.0) || (( index1 > 0) && ( index1 > size1-1.0) ) ||
            ( index2 < 0.0) || (( index2 > 0) && ( index2 > size2-1.0) )) {
            queryFlag = 1;
            return queryFlag;
        } // if

        switch (_data.getDataDim()) {
        case 1:
            _interpolate1D(values, numValues, index0, size0);
            break;
        case 2:
            _interpolate2D(values, numValues, index0, size0, index1, size1);
            break;
        case 3:
            _interpolate3D(values, numValues, index0, index1, index2);
            break;
        default:
            assert(false);
            throw std::logic_error("Unsupported data dimension in SimpleGridDB::query().");
        } // switch
        break;
    case SimpleGridDB::NEAREST: {
        index0 = std::min(index0, size0-1.0);
        index0 = std::max(index0, 0.0);
        index1 = std::min(index1, size1-1.0);
        index1 = std::max(index1, 0.0);
        index2 = std::min(index2, size2-1.0);
        index2 = std::max(index2, 0.0);
        const size_t indexNearest0 = size_t(std::floor(index0+0.5));
        const size_t indexNearest1 = size_t(std::floor(index1+0.5));
        const size_t indexNearest2 = size_t(std::floor(index2+0.5));
        const size_t indexData = _data.getDataIndex(indexNearest0, size0, indexNearest1, size1, indexNearest2, size2);

        const double* dataLoc = _data.getData(indexData);assert(dataLoc);
        for (size_t iValue = 0; iValue < querySize; ++iValue) {
            values[iValue] = dataLoc[_queryIndices[iValue]];
#if 0 // DEBUGGING
            std::cout << "val["<<iValue<<"]: " << values[iValue]
                      << ", indexData: " << indexData
                      << ", index0: " << index0
                      << ", index1: " << index1
                      << ", index2: " << index2
                      << std::endl;
#endif
        } // for
        break;
    } // NEAREST
    default:
        assert(false);
        throw std::logic_error("Unsupported query type in SimpleGridDB::query().");
    } // switch

    return queryFlag;
} // query


// ----------------------------------------------------------------------
// Interpolate to get values at target location defined by indices in 1-D.
void
spatialdata::spatialdb::SimpleGridDBQuery::_interpolate1D(double* values,
                                                          const size_t numValues,
                                                          const double indexX,
                                                          const size_t numX) const {
    assert(numX >= 2);
    const size_t indexX0 = std::min(numX-2, size_t(std::floor(indexX)));
    const double wtX0 = 1.0 - (indexX - indexX0);
    const size_t indexX1 = indexX0 + 1;
    const double wtX1 = 1.0 - wtX0;
    assert(0 <= indexX0 && indexX0 < numX);
    assert(0 <= indexX1 && indexX1 < numX);

    const double wt000 = wtX0;
    const size_t index000 = _data.getDataIndex(indexX0, numX, 0, 0, 0, 0);
    const double* data000 = _data.getData(index000);assert(data000);

    const double wt100 = wtX1;
    const size_t index100 = _data.getDataIndex(indexX1, numX, 0, 0, 0, 0);
    const double* data100 = _data.getData(index100);assert(data100);

    const size_t querySize = _queryIndices.size();
    for (size_t iValue = 0; iValue < querySize; ++iValue) {
        const size_t qVal = _queryIndices[iValue];
        values[iValue] =
            wt000 * data000[qVal] +
            wt100 * data100[qVal];
#if 0 // DEBUGGING
        std::cout << "val["<<iValue<<"]: " << values[iValue]
                  << ", wt000: " << wt000 << ", data: " << data000[qVal]
                  << ", wt100: " << wt100 << ", data: " << data100[qVal]
                  << std::endl;
#endif
    } // for

} // _interpolate1D


// ----------------------------------------------------------------------
// Interpolate to get values at target location defined by indices in 2-D.
void
spatialdata::spatialdb::SimpleGridDBQuery::_interpolate2D(double* values,
                                                          const size_t numValues,
                                                          const double indexX,
                                                          const size_t numX,
                                                          const double indexY,
                                                          const size_t numY) const {
    assert(numX >= 2);
    const size_t indexX0 = std::min(numX-2, size_t(std::floor(indexX)));
    const double wtX0 = 1.0 - (indexX - indexX0);
    const size_t indexX1 = indexX0 + 1;
    const double wtX1 = 1.0 - wtX0;
    assert(0 <= indexX0 && indexX0 < numX);
    assert(0 <= indexX1 && indexX1 < numX);

    assert(numY >= 2);
    const size_t indexY0 = std::min(numY-2, size_t(std::floor(indexY)));
    const double wtY0 = 1.0 - (indexY - indexY0);
    const size_t indexY1 = indexY0 + 1;
    const double wtY1 = 1.0 - wtY0;
    assert(0 <= indexY0 && indexY0 < numY);
    assert(0 <= indexY1 && indexY1 < numY);

    const double wt000 = wtX0 * wtY0;
    const size_t index000 = _data.getDataIndex(indexX0, numX, indexY0, numY, 0, 0);
    const double* data000 = _data.getData(index000);assert(data000);

    const double wt010 = wtX0 * wtY1;
    const size_t index010 = _data.getDataIndex(indexX0, numX, indexY1, numY, 0, 0);
    const double* data010 = _data.getData(index010);assert(data010);

    const double wt100 = wtX1 * wtY0;
    const size_t index100 = _data.getDataIndex(indexX1, numX, indexY0, numY, 0, 0);
    const double* data100 = _data.getData(index100);assert(data100);

    const double wt110 = wtX1 * wtY1;
    const size_t index110 = _data.getDataIndex(indexX1, numX, indexY1, numY, 0, 0);
    const double* data110 = _data.getData(index110);assert(data110);

    const size_t querySize = _queryIndices.size();
    for (size_t iValue = 0; iValue < querySize; ++iValue) {
        const size_t qVal = _queryIndices[iValue];
        values[iValue] =
            wt000 * data000[qVal] +
            wt010 * data010[qVal] +
            wt100 * data100[qVal] +
            wt110 * data110[qVal];
#if 0 // DEBUGGING
        std::cout << "val["<<iValue<<"]: " << values[iValue]
                  << ", wt000: " << wt000 << ", data: " << data000[qVal]
                  << ", wt010: " << wt010 << ", data: " << data010[qVal]
                  << ", wt100: " << wt100 << ", data: " << data100[qVal]
                  << ", wt110: " << wt110 << ", data: " << data110[qVal]
                  << std::endl;
#endif
    } // for

} // _interpolate2D


// ----------------------------------------------------------------------
// Interpolate to get values at target location defined by indices in 3-D.
void
spatialdata::spatialdb::SimpleGridDBQuery::_interpolate3D(double* values,
                                                          const size_t numValues,
                                                          const double indexX,
                                                          const double indexY,
                                                          const double indexZ) const {
    const size_t numX = _data.getNumX();
    const size_t numY = _data.getNumY();
    const size_t numZ = _data.getNumZ();

    assert(numX >= 2);
    const size_t indexX0 = std::min(numX-2, size_t(std::floor(indexX)));
    const double wtX0 = 1.0 - (indexX - indexX0);
    const size_t indexX1 = indexX0 + 1;
    const double wtX1 = 1.0 - wtX0;
    assert(0 <= indexX0 && indexX0 < numX);
    assert(0 <= indexX1 && indexX1 < numX);

    assert(numY >= 2);
    const size_t indexY0 = std::min(numY-2, size_t(std::floor(indexY)));
    const double wtY0 = 1.0 - (indexY - indexY0);
    const size_t indexY1 = indexY0 + 1;
    const double wtY1 = 1.0 - wtY0;
    assert(0 <= indexY0 && indexY0 < numY);
    assert(0 <= indexY1 && indexY1 < numY);

    assert(numZ >= 2);
    const size_t indexZ0 = std::min(numZ-2, size_t(std::floor(indexZ)));
    const double wtZ0 = 1.0 - (indexZ - indexZ0);
    const size_t indexZ1 = indexZ0 + 1;
    const double wtZ1 = 1.0 - wtZ0;
    assert(0 <= indexZ0 && indexZ0 < numZ);
    assert(0 <= indexZ1 && indexZ1 < numZ);

    const double wt000 = wtX0 * wtY0 * wtZ0;
    const size_t index000 = _data.getDataIndex(indexX0, numX, indexY0, numY, indexZ0, numZ);
    const double* data000 = _data.getData(index000);assert(data000);

    const double wt001 = wtX0 * wtY0 * wtZ1;
    const size_t index001 = _data.getDataIndex(indexX0, numX, indexY0, numY, indexZ1, numZ);
    const double* data001 = _data.getData(index001);assert(data001);

    const double wt010 = wtX0 * wtY1 * wtZ0;
    const size_t index010 = _data.getDataIndex(indexX0, numX, indexY1, numY, indexZ0, numZ);
    const double* data010 = _data.getData(index010);assert(data010);

    const double wt011 = wtX0 * wtY1 * wtZ1;
    const size_t index011 = _data.getDataIndex(indexX0, numX, indexY1, numY, indexZ1, numZ);
    const double* data011 = _data.getData(index011);assert(data011);

    const double wt100 = wtX1 * wtY0 * wtZ0;
    const size_t index100 = _data.getDataIndex(indexX1, numX, indexY0, numY, indexZ0, numZ);
    const double* data100 = _data.getData(index100);assert(data100);

    const double wt101 = wtX1 * wtY0 * wtZ1;
    const size_t index101 = _data.getDataIndex(indexX1, numX, indexY0, numY, indexZ1, numZ);
    const double* data101 = _data.getData(index101);assert(data101);

    const double wt110 = wtX1 * wtY1 * wtZ0;
    const size_t index110 = _data.getDataIndex(indexX1, numX, indexY1, numY, indexZ0, numZ);
    const double* data110 = _data.getData(index110);assert(data110);

    const double wt111 = wtX1 * wtY1 * wtZ1;
    const size_t index111 = _data.getDataIndex(indexX1, numX, indexY1, numY, indexZ1, numZ);
    const double* data111 = _data.getData(index111);assert(data111);

    const size_t querySize = _queryIndices.size();
    for (size_t iValue = 0; iValue < querySize; ++iValue) {
        const size_t qVal = _queryIndices[iValue];
        values[iValue] =
            wt000 * data000[qVal] +
            wt001 * data001[qVal] +
            wt010 * data010[qVal] +
            wt011 * data011[qVal] +
            wt100 * data100[qVal] +
            wt101 * data101[qVal] +
            wt110 * data110[qVal] +
            wt111 * data111[qVal];
#if 0 // DEBUGGING
        std::cout << "val["<<iValue<<"]: " << values[iValue]
                  << ", wt000: " << wt000 << ", data: " << data000[qVal]
                  << ", wt001: " << wt001 << ", data: " << data001[qVal]
                  << ", wt010: " << wt010 << ", data: " << data010[qVal]
                  << ", wt011: " << wt011 << ", data: " << data011[qVal]
                  << ", wt100: " << wt100 << ", data: " << data100[qVal]
                  << ", wt101: " << wt101 << ", data: " << data101[qVal]
                  << ", wt110: " << wt110 << ", data: " << data110[qVal]
                  << ", wt111: " << wt111 << ", data: " << data111[qVal]
                  << std::endl;
#endif
    } // for

} // _interpolate3D


// End of file
