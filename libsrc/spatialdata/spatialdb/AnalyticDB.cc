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

#include "AnalyticDB.hh" // Implementation of class methods

// Include ios here to avoid some Python/gcc issues
#include <ios>

#include "spatialdata/units/Parser.hh" // USES Parser
#include "spatialdata/geocoords/CSCart.hh" // HASA CoordSys
#include "spatialdata/geocoords/Converter.hh" // USES Converter
#include "spatialdata/muparser/muParser.h" // USES mu::parser

#include <vector> // USES std::vector
#include <stdexcept> // USES std::runtime_error

#include <sstream> // USES std::ostringsgream
#include <strings.h> // USES strcasecmp()
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
/// Constructor with description
spatialdata::spatialdb::AnalyticDB::AnalyticDB(const char* description) :
    SpatialDB(description ? description : ":UNKNOWN AnalyticDB:"),
    _cs(new spatialdata::geocoords::CSCart),
    _converter(new spatialdata::geocoords::Converter) {}


// ----------------------------------------------------------------------
/// Default destructor
spatialdata::spatialdb::AnalyticDB::~AnalyticDB(void) {
    clear();
    _cs.reset();
    _converter.reset();
} // destructor


// ----------------------------------------------------------------------
/// Clear database values.
void
spatialdata::spatialdb::AnalyticDB::clear(void) {
    _names.clear();_names.shrink_to_fit();
    _scales.clear();_names.shrink_to_fit();
    _expressions.clear();_names.shrink_to_fit();
    _parsers.clear();_names.shrink_to_fit();
    _queryIndices.clear();_names.shrink_to_fit();
} // destructor


// ----------------------------------------------------------------------
// Set values in database.
void
spatialdata::spatialdb::AnalyticDB::setData(const std::vector<std::string>& names,
                                            const std::vector<std::string>& units,
                                            const std::vector<std::string>& expressions) {
    const size_t numNames = names.size();
    const size_t numUnits = units.size();
    const size_t numValues = expressions.size();
    if ((numNames != numValues) || (numUnits != numValues)) {
        std::ostringstream msg;
        msg << "Mismatch in number of names (" << numNames << ")"
            << "number of units (" << numUnits << ")"
            << "and number of expressions (" << numValues << ") for AnalyticDB "
            << this->getDescription() << ".";
        throw std::range_error(msg.str());
    } // if
    clear();

    if (0 == numValues) {
        return;
    } // if

    spatialdata::units::Parser parser;

    _names = names;
    _scales.resize(numValues);
    for (size_t i = 0; i < numValues; ++i) {
        if (strcasecmp(units[i].c_str(), "none") != 0) {
            _scales[i] = parser.parse(units[i].c_str());
        } else {
            _scales[i] = 1.0;
        } // if/else
    } // for

    _expressions.resize(numValues);
    _parsers.resize(numValues);
    for (size_t i = 0; i < numValues; ++i) {
        _expressions[i] = expressions[i];
        _parsers[i].SetArgSep(',');
        _parsers[i].SetDecSep('.');
        // Only allow built-in variables mupSetVarFactory(_parsers, AddVariable, NULL);
        _parsers[i].DefineConst("pi", M_PI);
        _parsers[i].DefineVar("x", &_expressionVars[0]);
        _parsers[i].DefineVar("y", &_expressionVars[1]);
        _parsers[i].DefineVar("z", &_expressionVars[2]);
        _parsers[i].SetExpr(_expressions[i]);
    } // for

    // Default query values is all values.
    _queryIndices.clear();
    _queryIndices.reserve(numValues);
    for (size_t i = 0; i < numValues; ++i) {
        _queryIndices.emplace_back(i);
    } // for

} // setData


// ----------------------------------------------------------------------
// Set filename containing data.
void
spatialdata::spatialdb::AnalyticDB::setCoordSys(const std::shared_ptr<geocoords::CoordSys>& cs) {
    _cs = cs;assert(_cs);
} // setCoordSys


// ----------------------------------------------------------------------
// Get names of values in spatial database.
const std::vector<std::string>&
spatialdata::spatialdb::AnalyticDB::getNamesDBValues(void) const {
    return _names;
} // getNamesDBValues


// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
spatialdata::spatialdb::AnalyticDB::setQueryValues(const std::vector<std::string>& names) {
    const size_t querySize = names.size();
    if (0 == querySize) {
        std::ostringstream msg;
        msg << "Number of values for query in spatial database " << getDescription()
            << " must be positive.\n";
        throw std::invalid_argument(msg.str());
    } // if

    const size_t numValues = _names.size();
    _queryIndices.clear();
    _queryIndices.reserve(querySize);
    for (size_t iValue = 0; iValue < querySize; ++iValue) {
        size_t iName = 0;
        while (iName < numValues) {
            if (0 == strcasecmp(names[iValue].c_str(), _names[iName].c_str())) {
                break;
            } //
            ++iName;
        } // while
        if (iName >= numValues) {
            std::ostringstream msg;
            msg << "Could not find value '" << names[iValue] << "' in spatial database '"
                << getDescription() << "'. Available values are:";
            for (size_t iName = 0; iName < numValues; ++iName) {
                msg << "\n  " << _names[iName];
                msg << "\n";
            } // for
            throw std::out_of_range(msg.str());
        } // if
        _queryIndices.emplace_back(iName);
    } // for
} // setQueryValues


// ----------------------------------------------------------------------
// Query the database.
int
spatialdata::spatialdb::AnalyticDB::query(double* values,
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
    } else if (spaceDim != _cs->getSpaceDim()) {
        std::ostringstream msg;
        msg << "Spatial dimension (" << spaceDim
            << ") does not match spatial dimension of spatial database (" << _cs->getSpaceDim() << ").";
        throw std::invalid_argument(msg.str());
    } // if

    // Convert coordinates
    assert(spaceDim <= 3);
    for (size_t d = 0; d < spaceDim; ++d) {
        _expressionVars[d] = coordinates[d];
    }
    assert(_converter);
    _converter->convert(_expressionVars, 1, spaceDim, _cs.get(), csCoordinates);

    try {
        for (size_t iValue = 0; iValue < querySize; ++iValue) {
            const size_t index = _queryIndices[iValue];
            values[iValue] = _scales[index] * _parsers[index].Eval();
        } // for
    } catch (const mu::Parser::exception_type& exception) {
        throw std::runtime_error(exception.GetMsg());
    }

    return 0;
} // query


// End of file
