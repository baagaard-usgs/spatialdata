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

#include "SimpleGridDB.hh" // Implementation of class methods

#include "SimpleGridDBData.hh" // USES SimpleGridData
#include "SimpleGridDBQuery.hh" // USES SimpleGridQuery
#include "SimpleGridDBIO.hh" // USES SimpleGridIO
#include "Exception.hh" // USES OutOfBounds

#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::logic_error
#include <cassert> // USES assert()

// ----------------------------------------------------------------------
/// Default constructor
spatialdata::spatialdb::SimpleGridDB::SimpleGridDB(const char* description) :
    SpatialDB(description ? description : ":UNKNOWN SimpleGridDB:") {
    _data = std::make_unique<SimpleGridDBData>();
    _query = std::make_unique<SimpleGridDBQuery>(*_data.get(), this->getDescription());
}


// ----------------------------------------------------------------------
// Destructor
spatialdata::spatialdb::SimpleGridDB::~SimpleGridDB(void) {}


// ----------------------------------------------------------------------
// Set query type.
void
spatialdata::spatialdb::SimpleGridDB::setQueryType(const SimpleGridDB::QueryEnum queryType) {
    assert(_query);
    _query->setQueryType(queryType);
} // setQueryType


// ----------------------------------------------------------------------
// Set filename containing data.
void
spatialdata::spatialdb::SimpleGridDB::setFilename(const char* filename) {
    if (!filename) {
        throw std::logic_error("Null argument to SimpleGridDB::setFilename().");
    } // if
    _filename = filename;
} // setFilename


// ----------------------------------------------------------------------
// Open the database and prepare for querying.
void
spatialdata::spatialdb::SimpleGridDB::open(void) {
    SimpleGridDBIO::read(_data.get(), _filename.c_str());

    // Set default query values to all values in database
    _query->setQueryValues(_data->getNames());
} // open


// ----------------------------------------------------------------------
// Close the database.
void
spatialdata::spatialdb::SimpleGridDB::close(void) {
    _data->deallocate();
} // close


// ----------------------------------------------------------------------
// Get names of values in spatial database.
const std::vector<std::string>&
spatialdata::spatialdb::SimpleGridDB::getNamesDBValues(void) const {
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
spatialdata::spatialdb::SimpleGridDB::setQueryValues(const std::vector<std::string>& names) {
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
spatialdata::spatialdb::SimpleGridDB::query(double* values,
                                            const size_t numVals,
                                            const double* coordinates,
                                            const spatialdata::geocoords::CoordSys* csCoordinates) {
    try {
        assert(_query);
        _query->query(values, numVals, coordinates, csCoordinates);
    } catch (const OutOfBounds& err) {
        std::fill(values, values+numVals, 0);
        return 1;
    } catch (const std::exception& err) {
        throw;
    } catch (...) {
        throw std::runtime_error("Unknown error in SimpleDB::query()");
    } // catch
    return 0;
} // query


// End of file
