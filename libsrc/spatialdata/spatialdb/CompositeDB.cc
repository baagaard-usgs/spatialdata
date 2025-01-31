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

#include "CompositeDB.hh" // Implementation of class methods

#include "Utilities.hh" // USES search()
#include "Exception.hh" // USES ValueNotFound

#include <set> // USES std::set
#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringsgream
#include <strings.h> // USES strcasecmp()
#include <cassert> // USES assert()

// ----------------------------------------------------------------------
/// Constructor with description.
spatialdata::spatialdb::CompositeDB::CompositeDB(const char* description) :
    SpatialDB(description) {}


// ----------------------------------------------------------------------
/// Default destructor
spatialdata::spatialdb::CompositeDB::~CompositeDB(void) {}


// ----------------------------------------------------------------------
// Set database A.
void
spatialdata::spatialdb::CompositeDB::addDB(std::shared_ptr<SpatialDB>& db,
                                           const std::vector<std::string>& namesValues) {
    assert(db);
    assert(namesValues.size() > 0);

    DBEntry entry;
    entry.db = db;
    entry.namesValues = namesValues;
    _dbs.push_back(entry);

    std::set<std::string> namesCurrent;
    for (const std::string& name : _namesValues) {
        namesCurrent.insert(name);
    } // for
    const size_t numNamesAdd = namesValues.size();
    _namesValues.reserve(_namesValues.size() + numNamesAdd);
    for (const std::string& name : namesValues) {
        if (namesCurrent.count(name)) {
            std::ostringstream msg;
            msg << "Value '" << name << "' for spatial database '" << db->getDescription() << "' "
                << "already set by another spatial database in CompositeDB '" << this->getDescription() << "'.";
            throw std::runtime_error(msg.str());
        } // if
        _namesValues.emplace_back(name);
    } // for
} // addDB


// ----------------------------------------------------------------------
// Open the database and prepare for querying.
void
spatialdata::spatialdb::CompositeDB::open(void) {
    for (DBEntry& entry : _dbs) {
        entry.db->open();

        // Default is to query designated values in each spatial database.
        const size_t querySize = entry.namesValues.size();
        entry.queryIndices.clear();
        entry.queryIndices.reserve(querySize);
        entry.queryBuffer.resize(querySize);
        for (size_t iQuery = 0; iQuery < querySize; ++iQuery) {
            const size_t index = Utilities::search(entry.db->getNamesDBValues(), entry.namesValues[iQuery].c_str(), entry.db->getDescription());
            entry.queryIndices.emplace_back(index);
            entry.db->setQueryValues(entry.namesValues);
        } // for
    } // for
} // open


// ----------------------------------------------------------------------
// Close the database.
void
spatialdata::spatialdb::CompositeDB::close(void) {
    for (DBEntry& entry : _dbs) {
        entry.db->close();
    } // for
} // close


// ----------------------------------------------------------------------
// Get names of values in spatial database.
const std::vector<std::string>&
spatialdata::spatialdb::CompositeDB::getNamesDBValues(void) const {
    return _namesValues;
} // getNamesDBValues


// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
spatialdata::spatialdb::CompositeDB::setQueryValues(const std::vector<std::string>& namesQuery) {
    for (DBEntry& entry : _dbs) {
        entry.queryIndices.clear();
        entry.queryIndices.reserve(entry.namesValues.size());
    } // for

    const size_t querySize = namesQuery.size();
    for (size_t iQuery = 0; iQuery < querySize; ++iQuery) {
        bool found = false;
        for (DBEntry& entry : _dbs) {
            try {
                Utilities::search(entry.namesValues, namesQuery[iQuery].c_str(), getDescription());
                entry.queryIndices.emplace_back(iQuery);
                found = true;
                break;
            } catch (const ValueNotFound& err) {
                continue;
            } catch (...) {
                throw;
            } // try/catch
        } // for
        if (!found) {
            std::ostringstream msg;
            msg << "Could not find value '" << namesQuery[iQuery] << "' in spatial database '"
                << getDescription() << "'. Available values are:";
            for (const std::string& name : _namesValues) {
                msg << "\n  " << name;
            }
            msg << "\n";
            throw ValueNotFound(msg.str());
        } // if
    } // for

    for (DBEntry& entry : _dbs) {
        std::vector<std::string> namesDB;
        namesDB.reserve(entry.queryIndices.size());

        const size_t dbQuerySize = entry.queryIndices.size();
        for (size_t iQuery = 0; iQuery < dbQuerySize; ++iQuery) {
            namesDB.emplace_back(namesQuery[entry.queryIndices[iQuery]]);
        } // for
        if (dbQuerySize > 0) {
            entry.db->setQueryValues(namesDB);
            entry.queryBuffer.resize(dbQuerySize);
        } // if
    } // for

} // setQueryValues


// ----------------------------------------------------------------------
// Query the database.
int
spatialdata::spatialdb::CompositeDB::query(double* values,
                                           const size_t numValues,
                                           const double* coordinates,
                                           const spatialdata::geocoords::CoordSys* csCoordinates) {
    int err = 0;
    for (DBEntry& entry : _dbs) {
        if (entry.queryIndices.size() > 0) {
            const size_t querySize = entry.queryIndices.size();
            assert(entry.queryBuffer.size() == querySize);
            const int ierr = entry.db->query(entry.queryBuffer.data(), querySize, coordinates, csCoordinates);
            err |= ierr;
            for (size_t iValue = 0; iValue < querySize; ++iValue) {
                values[entry.queryIndices[iValue]] = entry.queryBuffer[iValue];
            } // for
        } // if
    } // for

    return err;
} // query


// End of file
