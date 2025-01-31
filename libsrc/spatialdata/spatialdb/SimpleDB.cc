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

#include "SimpleDB.hh" // Implementation of class methods

#include "SimpleDBData.hh" // USES SimpleDBData
#include "SimpleDBQuery.hh" // USES SimpleDBQuery
#include "SimpleDBIO.hh" // USES SimpleDBIO

#include "spatialdata/spatialdb/Exception.hh" // USES OutOfBounds
#include "spatialdata/geocoords/CoordSys.hh" // USES CoordSys

#include <sstream> // USES std::ostringsgream
#include <cassert> // USES assert()
#include <stdexcept> // USES std::runtime_error

// ----------------------------------------------------------------------
/// Default constructor
spatialdata::spatialdb::SimpleDB::SimpleDB(const char* description) :
    SpatialDB(description ? description : ":UNKNOWN SimpleDB:") {
    _data = std::make_unique<SimpleDBData>();
    _query = std::make_unique<SimpleDBQuery>(*_data.get(), this->getDescription());
}


// ----------------------------------------------------------------------
/// Default destructor
spatialdata::spatialdb::SimpleDB::~SimpleDB(void) {}


// ----------------------------------------------------------------------
// Set query type.
void
spatialdata::spatialdb::SimpleDB::setQueryType(const SimpleDB::QueryEnum queryType) {
    assert(_query);
    _query->setQueryType(queryType);
} // setQueryType


// ----------------------------------------------------------------------
// Set filename for database.
void
spatialdata::spatialdb::SimpleDB::setFilename(const char* filename) {
    if (!filename) {
        throw std::logic_error("Null argument to SimpleDB::setFilename().");
    } // if
    _filename = filename;
}


// ----------------------------------------------------------------------
/// Open the database and prepare for querying.
void
spatialdata::spatialdb::SimpleDB::open(void) {
    SimpleDBIO::read(_data.get(), _filename.c_str());

    // Set default query values to all values in database
    _query->setQueryValues(_data->getNames());
} // open


// ----------------------------------------------------------------------
/// Close the database.
void
spatialdata::spatialdb::SimpleDB::close(void) {
    _data->deallocate();
} // close


// ----------------------------------------------------------------------
// Get names of values in spatial database.
const std::vector<std::string>&
spatialdata::spatialdb::SimpleDB::getNamesDBValues(void) const {
    if (!_data) {
        std::ostringstream msg;
        msg << "Spatial database " << getDescription() << " has not been opened.\n"
            << "Please call open() before calling getNamesDBValues().";
        throw std::logic_error(msg.str());
    } // if
    return _data->getNames();
} // getNamesDBValues


// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
spatialdata::spatialdb::SimpleDB::setQueryValues(const std::vector<std::string>& names) {
    if (!_query) {
        std::ostringstream msg;
        msg << "Spatial database " << getDescription() << " has not been opened.\n"
            << "Please call open() before calling setQueryValues().";
        throw std::logic_error(msg.str());
    } // if
    _query->setQueryValues(names);
} // queryVals


// ----------------------------------------------------------------------
// Query the database.
int
spatialdata::spatialdb::SimpleDB::query(double* values,
                                        const size_t numValues,
                                        const double* coordinates,
                                        const spatialdata::geocoords::CoordSys* csCoordinates) {
    try {
        assert(_query);
        _query->query(values, numValues, coordinates, csCoordinates);
    } catch (const OutOfBounds& err) {
        std::fill(values, values+numValues, 0);
        return 1;
    } catch (const std::exception& err) {
        throw;
    } catch (...) {
        throw std::runtime_error("Unknown error in SimpleDB::query()");
    } // catch
    return 0;
} // query


// End of file
