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

#include "SpatialDB.hh" // ISA Spatialdb

#include <string> // HASA std::string

class spatialdata::spatialdb::SimpleDB : public SpatialDB {
    friend class TestSimpleDB; // unit testing

public:

    // PUBLIC ENUM ////////////////////////////////////////////////////////

    /** Topology of spatial data */
    enum TopoEnum {
        POINT=0,
        LINE=1,
        AREA=2,
        VOLUME=3
    };

    /** Type of query */
    enum QueryEnum {
        NEAREST=0,
        LINEAR=1
    };

public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Constructor with description.
     *
     * @param description Description of database
     */
    SimpleDB(const char* description);

    /// Default destructor.
    ~SimpleDB(void);

    /** Set query type.
     *
     * @pre Must call Open() before QueryType()
     *
     * @param queryType Set type of query
     */
    void setQueryType(const SimpleDB::QueryEnum queryType);

    /** Set filename for database.
     *
     * @param filename Filename of database
     */
    void setFilename(const char* filename);

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
     * @param[in] names Names of values to be returned in queries
     */
    void setQueryValues(const std::vector<std::string>& names) override;

    /** Query the database.
     *
     * @pre Must call open() before query()
     *
     * @param values Array for computed values (output from query), values
     *   must be allocated BEFORE calling query().
     * @param numValues Number of values expected (size of values array)
     * @param coordinates Coordinates of point for query
     * @param csCoordinates Coordinate system of coordinates
     *
     * @returns 0 on success, 1 on failure (i.e., could not interpolate
     *   so values set to 0)
     */
    int query(double* values,
              const size_t numValues,
              const double* coordinates,
              const spatialdata::geocoords::CoordSys* csCoordinates) override;

private:

    // PRIVATE METHODS ////////////////////////////////////////////////////

    SimpleDB(void); ///< Not implemented
    SimpleDB(const SimpleDB& data); ///< Not implemented
    const SimpleDB& operator=(const SimpleDB& data); ///< Not implemented

private:

    // PRIVATE MEMBERS /////////////////////////////////////////////////////

    std::unique_ptr<SimpleDBData> _data; ///< Pointer to data
    std::unique_ptr<SimpleDBQuery> _query; ///< Query handler
    std::string _filename;

}; // class SimpleDB

// End of file
