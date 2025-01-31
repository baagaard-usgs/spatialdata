// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================
#pragma once

#include "spatialdata/spatialdb/SpatialDB.hh"

#include <list> // HASA std::list
#include <string> // HASA std::string

/// C++ manager for simple spatial database.
class spatialdata::spatialdb::CompositeDB : public spatialdata::spatialdb::SpatialDB {
    friend class TestCompositeDB; // unit testing

public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Constructor with description.
     *
     * @param description Description of database
     */
    CompositeDB(const char* description);

    /// Default destructor.
    ~CompositeDB(void);

    /** Add spatial database.
     *
     * @param db Spatial database.
     * @param namesValues Names of values to use with database.
     */
    void addDB(std::shared_ptr<spatialdata::spatialdb::SpatialDB>& db,
               const std::vector<std::string>& namesValues);

    /// Open the database and prepare for querying.
    void open(void) override;

    /// Close the database.
    void close(void) override;

    /** Get names of values in spatial database.
     *
     * @returns Array of names of values.
     */
    const std::vector<std::string>& getNamesDBValues(void) const override;

    /** Set values to be returned by queries.
     *
     * @pre Must call open() before setQueryValues()
     *
     * @param namesQuery Names of values to be returned in queries
     */
    void setQueryValues(const std::vector<std::string>& namesQuery) override;

    /** Query the database.
     *
     * @note values should be preallocated to accommodate numValues values.
     *
     * @pre Must call open() before query().
     *
     * @param values Array for computed values (output from query), must be
     *   allocated BEFORE calling query().
     * @param numValues Number of values expected (size of pVals array)
     * @param coordinates Coordinates of point for query [numDims].
     * @param csCoordinates Coordinate system of coordinates.
     *
     * @returns 0 on success, 1 on failure (i.e., could not interpolate)
     */
    int query(double* values,
              const size_t numValues,
              const double* coordinates,
              const spatialdata::geocoords::CoordSys* csCoordinates) override;

private:

    // NOT IMPLEMENTED ////////////////////////////////////////////////////

    CompositeDB(void); ///< Not implemented
    CompositeDB(const CompositeDB& data); ///< Not implemented
    const CompositeDB& operator=(const CompositeDB& data); ///< Not implemented

private:

    // PRIVATE STRUCTS ////////////////////////////////////////////////////

    struct DBEntry {
        std::shared_ptr<spatialdata::spatialdb::SpatialDB> db;
        std::vector<double> queryBuffer;
        std::vector<size_t> queryIndices;
        std::vector<std::string> namesValues;
    }; // DBInfo

private:

    // PRIVATE MEMBERS /////////////////////////////////////////////////////

    std::list<DBEntry> _dbs; ///< Queue of spatial databases.
    std::vector<std::string> _namesValues; ///< Names of values in databases.

}; // class CompositeDB

// End of file
