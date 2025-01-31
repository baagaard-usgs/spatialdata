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

#include "UniformDB.hh" // Implementation of class methods

// Include ios here to avoid some Python/gcc issues
#include <ios>

#include "spatialdata/units/Parser.hh" // USES Parser

#include <vector> // USES std::vector
#include <stdexcept> // USES std::runtime_error

#include <sstream> // USES std::ostringsgream
#include <strings.h> // USES strcasecmp()
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
/// Constructor with description
spatialdata::spatialdb::UniformDB::UniformDB(const char* description) :
    SpatialDB(description ? description : ":UNKNOWN UniformDB:") {}


// ----------------------------------------------------------------------
/// Default destructor
spatialdata::spatialdb::UniformDB::~UniformDB(void) {}


// ----------------------------------------------------------------------
// Set values in database.
void
spatialdata::spatialdb::UniformDB::setData(const std::vector<std::string>& names,
                                           const std::vector<std::string>& units,
                                           const std::vector<double>& values) {
    const size_t numNames = names.size();
    const size_t numUnits = units.size();
    const size_t numValues = values.size();
    if ((numNames != numValues) || (numUnits != numValues)) {
        std::ostringstream msg;
        msg << "Mismatch in number of names (" << numNames << ")"
            << "number of units (" << numUnits << ")"
            << "and number of expressions (" << numValues << ") for UniformDB "
            << this->getDescription() << ".";
        throw std::range_error(msg.str());
    } // if
    _names = names;

    spatialdata::units::Parser parser;

    if (numValues > 0) {
        std::vector<double> scales(numValues);
        for (size_t i = 0; i < numValues; ++i) {
            if (strcasecmp(units[i].c_str(), "none") != 0) {
                scales[i] = parser.parse(units[i].c_str());
            } else {
                scales[i] = 1.0;
            } // if/else
        } // for

        _values.clear();
        _values.reserve(numValues);
        for (size_t i = 0; i < numValues; ++i) {
            _values.emplace_back(values[i]*scales[i]);
        } // for

        // Default query values is all values.
        _queryIndices.clear();
        _queryIndices.reserve(numValues);
        for (size_t i = 0; i < numValues; ++i) {
            _queryIndices.emplace_back(i);
        } // for
    } else {
        _values.clear();_values.shrink_to_fit();
        _names.clear();_names.shrink_to_fit();
        _queryIndices.clear();_queryIndices.shrink_to_fit();
    } // if/else
} // setData


// ----------------------------------------------------------------------
// Get names of values in spatial database.
const std::vector<std::string>&
spatialdata::spatialdb::UniformDB::getNamesDBValues(void) const {
    return _names;
}


// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
spatialdata::spatialdb::UniformDB::setQueryValues(const std::vector<std::string>& names) {
    const size_t querySize = names.size();
    if (0 == querySize) {
        std::ostringstream msg;
        msg << "Number of values for query in spatial database " << getDescription()
            << " must be positive.\n";
        throw std::invalid_argument(msg.str());
    } // if

    _queryIndices.clear();
    _queryIndices.reserve(querySize);
    const size_t numValues = _names.size();
    for (size_t iValue = 0; iValue < querySize; ++iValue) {
        size_t iName = 0;
        while (iName < numValues) {
            if (0 == strcasecmp(names[iValue].c_str(), _names[iName].c_str())) {
                break;
            }
            ++iName;
        } // while
        if (iName >= numValues) {
            std::ostringstream msg;
            msg << "Could not find value '" << names[iValue] << "' in spatial database '"
                << getDescription() << "'. Available values are:";
            for (size_t iName = 0; iName < numValues; ++iName) {
                msg << "\n  " << _names[iName];
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
spatialdata::spatialdb::UniformDB::query(double* values,
                                         const size_t numValues,
                                         const double* coordinates,
                                         const spatialdata::geocoords::CoordSys* csCoordinates) {
    assert(!numValues || values);

    const size_t querySize = _queryIndices.size();
    if (0 == querySize) {
        std::ostringstream msg;
        msg << "Values to be returned by spatial database " << getDescription() << "\n"
            << "have not been set. Please call setQueryValues() before query().\n";
        throw std::logic_error(msg.str());
    } // if
    else if (numValues != querySize) {
        std::ostringstream msg;
        msg << "Number of values to be returned by spatial database "
            << getDescription() << "\n"
            << "(" << querySize << ") does not match size of array provided ("
            << numValues << ").\n";
        throw std::invalid_argument(msg.str());
    } // if

    for (size_t iValue = 0; iValue < querySize; ++iValue) {
        values[iValue] = _values[_queryIndices[iValue]];
    } // for

    return 0;
} // query


// End of file
