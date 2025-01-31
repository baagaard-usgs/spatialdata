// ================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// ================================================================================================

#include <portinfo>

#include "spatialdata/spatialdb/CxxFunctionDB.hh" // Implementation of class methods

// Include ios here to avoid some Python/gcc issues
#include <ios>

#include "spatialdata/geocoords/CoordSys.hh" // USES CoordSys
#include "spatialdata/geocoords/Converter.hh" // USES Converter
#include "spatialdata/units/Parser.hh" // USES Parser

#include <string> // USES std::string
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <cassert> // USES assert()
#include <typeinfo> // USES typeid()

// ----------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class CxxFunctionDB::CxxFn1D : public CxxFunctionDB::CxxFn {
public:

            CxxFn1D(CxxFunctionDB::cxxfn1D_type fn) : _fn(fn) {}


            int query(double* value,
                      const double* coordinates,
                      const size_t dim) {
                if (!value || !coordinates || ( 1 != dim) ) { return 1; }
                *value = _fn(coordinates[0]);
                return 0;
            }

private:

            CxxFunctionDB::cxxfn1D_type _fn;
        };

        class CxxFunctionDB::CxxFn2D : public CxxFunctionDB::CxxFn {
public:

            CxxFn2D(CxxFunctionDB::cxxfn2D_type fn) : _fn(fn) {}


            int query(double* value,
                      const double* coordinates,
                      const size_t dim) {
                if (!value || !coordinates || ( 2 != dim) ) { return 1; }
                *value = _fn(coordinates[0], coordinates[1]);
                return 0;
            }

private:

            CxxFunctionDB::cxxfn2D_type _fn;
        };

        class CxxFunctionDB::CxxFn3D : public CxxFunctionDB::CxxFn {
public:

            CxxFn3D(CxxFunctionDB::cxxfn3D_type fn) : _fn(fn) {}


            int query(double* value,
                      const double* coordinates,
                      const size_t dim) {
                if (!value || !coordinates || ( 3 != dim) ) { return 1; }
                *value = _fn(coordinates[0], coordinates[1], coordinates[2]);
                return 0;
            }

private:

            CxxFunctionDB::cxxfn3D_type _fn;
        };

    } // namespace spatialdb
} // namespace spatialdata

// ----------------------------------------------------------------------
// Constructor
spatialdata::spatialdb::CxxFunctionDB::CxxFunctionDB(const char* description) :
    SpatialDB(description ? description : ":UNKNOWN CxxFunctionDB:"),
    _converter(new spatialdata::geocoords::Converter) {}


// ----------------------------------------------------------------------
// Destructor
spatialdata::spatialdb::CxxFunctionDB::~CxxFunctionDB(void) {}


// ----------------------------------------------------------------------
// Add function/value to database in 1-D.
void
spatialdata::spatialdb::CxxFunctionDB::addValue(const char* name,
                                                cxxfn1D_type fn,
                                                const char* units) {
    _checkAdd(name, (void*)fn, units);

    QueryFn data;
    data.fn = std::make_shared<CxxFn1D>(fn);
    data.units = units;
    data.scale = 0.0;
    _functions.emplace_back(data);
    _names.push_back(name);
} // addValue


// ----------------------------------------------------------------------
// Add function/value to database in 2-D.
void
spatialdata::spatialdb::CxxFunctionDB::addValue(const char* name,
                                                cxxfn2D_type fn,
                                                const char* units) {
    _checkAdd(name, (void*)fn, units);

    QueryFn data;
    data.fn = std::make_shared<CxxFn2D>(fn);
    data.units = units;
    data.scale = 0.0;
    _functions.emplace_back(data);
    _names.push_back(name);
} // addValue


// ----------------------------------------------------------------------
// Add function/value to database in 3-D.
void
spatialdata::spatialdb::CxxFunctionDB::addValue(const char* name,
                                                cxxfn3D_type fn,
                                                const char* units) {
    _checkAdd(name, (void*)fn, units);

    QueryFn data;
    data.fn = std::make_shared<CxxFn3D>(fn);
    data.units = units;
    data.scale = 0.0;
    _functions.emplace_back(data);
    _names.push_back(name);
} // addValue


// ----------------------------------------------------------------------
// Open the database and prepare for querying.
void
spatialdata::spatialdb::CxxFunctionDB::open(void) {
    // Compute conversion to SI units.
    spatialdata::units::Parser parser;

    const std::string& none = "none";
    for (QueryFn& function : _functions) {
        if (strcasecmp(none.c_str(),  function.units.c_str()) != 0) {
            function.scale = parser.parse(function.units.c_str());
        } else {
            function.scale = 1.0;
        } // if/else
    } // for

    _checkCompatibility();

    // Default query values is all values.
    const size_t querySize = _functions.size();
    _queryIndices.reserve(querySize);
    for (size_t i = 0; i < querySize; ++i) {
        _queryIndices.emplace_back(i);
    } // for
} // open


// ----------------------------------------------------------------------
// Close the database.
void
spatialdata::spatialdb::CxxFunctionDB::close(void) {
    _queryIndices.clear();
} // close


// ----------------------------------------------------------------------
// Get names of values in spatial database.
const std::vector<std::string>&
spatialdata::spatialdb::CxxFunctionDB::getNamesDBValues(void) const {
    return _names;
} // getNamesDBValues


// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
spatialdata::spatialdb::CxxFunctionDB::setQueryValues(const std::vector<std::string>& names) {
    const size_t querySize = names.size();
    if (0 == querySize) {
        std::ostringstream msg;
        msg << "Number of values for query in spatial database " << getDescription()
            << "\n must be positive.\n";
        throw std::invalid_argument(msg.str());
    } // if

    _queryIndices.clear();
    _queryIndices.reserve(querySize);

    for (size_t iQuery = 0; iQuery < querySize; ++iQuery) {
        size_t iName = 0;
        const size_t numNames = _names.size();
        for (; iName < numNames; ++iName) {
            if (0 == strcasecmp(names[iQuery].c_str(), _names[iName].c_str())) {
                break;
            } // if
        } // for
        if (iName >= numNames) {
            std::ostringstream msg;
            msg << "Could not find value '" << names[iQuery] << "' in spatial database '"
                << getDescription() << "'. Available values are:";
            for (size_t iName = 0; iName < numNames; ++iName) {
                msg << "\n  " << _names[iName];
            }
            msg << "\n";
            throw std::out_of_range(msg.str());
        } // if
        _queryIndices.emplace_back(iName);
    } // for
} // setQueryValues


// ----------------------------------------------------------------------
// Query the database.
int
spatialdata::spatialdb::CxxFunctionDB::query(double* values,
                                             const size_t numValues,
                                             const double* coordinates,
                                             const spatialdata::geocoords::CoordSys* csCoordinates) {
    assert(!numValues || values);
    assert(coordinates);
    assert(csCoordinates);

    const size_t querySize = _queryIndices.size();
    const size_t spaceDim = csCoordinates->getSpaceDim();

    assert(_cs);
    if (0 == querySize) {
        std::ostringstream msg;
        msg << "Values to be returned by spatial database " << getDescription()
            << " have not been set. Please call setQueryValues() before query().\n";
        throw std::logic_error(msg.str());
    } else if (numValues != querySize) {
        std::ostringstream msg;
        msg << "Number of values to be returned by spatial database "
            << getDescription() << " (" << querySize << ") does not match size of array provided ("
            << numValues << ").\n";
        throw std::invalid_argument(msg.str());
    } else if (spaceDim != _cs->getSpaceDim()) {
        std::ostringstream msg;
        msg << "Spatial dimension (" << spaceDim
            << ") does not match spatial dimension of spatial database (" << _cs->getSpaceDim() << ").";
        throw std::invalid_argument(msg.str());
    } // if

    // Convert coordinates
    assert(spaceDim <= 3);
    double xyz[3];
    std::copy(coordinates, coordinates+spaceDim, xyz);
    assert(_converter);
    _converter->convert(xyz, 1, spaceDim, _cs.get(), csCoordinates);

    int queryFlag = 0;
    for (size_t iValue = 0; iValue < querySize; ++iValue) {
        queryFlag = _functions[_queryIndices[iValue]].fn->query(&values[iValue], xyz, spaceDim);
        if (queryFlag) { break; }
        values[iValue] *= _functions[_queryIndices[iValue]].scale; // Convert to SI units.
    } // for

    return queryFlag;
} // query


// ----------------------------------------------------------------------
// Set filename containing data.
void
spatialdata::spatialdb::CxxFunctionDB::setCoordSys(const std::shared_ptr<geocoords::CoordSys>& cs) {
    _cs = cs;
} // setCoordSys


// ----------------------------------------------------------------------
void
spatialdata::spatialdb::CxxFunctionDB::_checkAdd(const char* name,
                                                 void* fn,
                                                 const char* units) const {
    if (!name) {
        std::ostringstream msg;
        msg << "NULL name passed to addValue() for spatial database " << getDescription() << ".";
        throw std::logic_error(msg.str());
    } // if

    if (!units) {
        std::ostringstream msg;
        msg << "NULL units passed to addValue() for spatial database " << getDescription() << ".";
        throw std::logic_error(msg.str());
    } // if

    // Verify user function for value does not already exist.
    bool found = false;
    const size_t numValues = _functions.size();
    for (size_t i = 0; i < numValues; ++i) {
        if (0 == strcasecmp(_names[i].c_str(), name)) {
            found = true;
            break;
        } // if
    } // for
    if (found) {
        std::ostringstream msg;
        msg << "Cannot add user function for value " << name << " to spatial database " << getDescription()
            << ". User function for value already exists.";
        throw std::logic_error(msg.str());
    } // if

    if (!fn) {
        std::ostringstream msg;
        msg << "Cannot add NULL user function for value " << name << " to spatial database " << getDescription() << ".";
        throw std::invalid_argument(msg.str());
    } // if
} // _checkAdd


// ----------------------------------------------------------------------
// Check compatibility of spatial database parameters.
void
spatialdata::spatialdb::CxxFunctionDB::_checkCompatibility(void) const {
    // Verify that we can call all user functions for given spatial dimension.

    if (!_cs) {
        std::ostringstream msg;
        msg << "Coordinate system has not been set for spatial database " << getDescription() << ".";
        throw std::logic_error(msg.str());
    } // if

    double coordinates[3] = { 0.0, 0.0, 0.0 };
    const int spaceDim = _cs->getSpaceDim();
    assert(0 < spaceDim && spaceDim <= 3);

    double value;
    const size_t numValues = _functions.size();
    for (size_t iValue = 0; iValue < numValues; ++iValue) {
        assert(_functions[iValue].fn);
        const int flag = _functions[iValue].fn->query(&value, coordinates, spaceDim);
        if (flag) {
            std::ostringstream msg;
            msg << "Error encountered in verifying compatibility for user function " << typeid(_functions[iValue].fn).name()
                << " for value '" << _names[iValue] << "' in spatial database " << getDescription() << ".";
            throw std::runtime_error(msg.str());
        } // if
    } // for
} // _checkCompatibility


// End of file
