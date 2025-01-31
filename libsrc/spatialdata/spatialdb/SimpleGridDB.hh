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

#include "SpatialDB.hh" // ISA SpatialDB

#include <string> // HASA std::string

class spatialdata::spatialdb::SimpleGridDB : public SpatialDB {
    friend class TestSimpleGridDB; // unit testing

public:

    // PUBLIC ENUM ////////////////////////////////////////////////////////

    /** Type of query */
    enum QueryEnum {
        NEAREST=0, ///< Nearest interpolation.
        LINEAR=1, ///< Linear interpolation.
    };

    // PUBLIC MEMBERS ///////////////////////////////////////////////////////
public:

    /** Constructor with description.
     *
     * @param description Description of database
     */
    SimpleGridDB(const char* description);

    /// Destructor
    ~SimpleGridDB(void);

    /** Set query type.
     *
     * @pre Must call Open() before QueryType()
     *
     * @param queryType Set type of query
     */
    void setQueryType(const QueryEnum queryType);

    /** Set filename containing data.
     *
     * @param filename Name of data file.
     */
    void setFilename(const char* filename);

    /// Open the database and prepare for querying.
    void open(void) override;

    /// Close the database.
    void close(void) override;

    /** Get names of values in spatial database.
     *
     * @returns Names of values.
     */
    const std::vector<std::string>& getNamesDBValues(void) const override;

    /** Set values to be returned by queries.
     *
     * @pre Must call open() before setQueryValues()
     *
     * @param names Names of values to be returned in queries
     */
    void setQueryValues(const std::vector<std::string>& names) override;

    /** Query the database.
     *
     * @pre Must call open() before query()
     *
     * @param values Array for computed values (output from query), vals
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

    // PRIVATE MEMBERS //////////////////////////////////////////////////////
private:

    std::unique_ptr<SimpleGridDBData> _data; ///< Pointer to data
    std::unique_ptr<SimpleGridDBQuery> _query; ///< Query handler
    std::string _filename; ///< Filename of data file

    // NOT IMPLEMENTED //////////////////////////////////////////////////////
private:

    SimpleGridDB(void); ///< Not implemented
    SimpleGridDB(const SimpleGridDB&); ///< Not implemented
    const SimpleGridDB& operator=(const SimpleGridDB&); ///< Not implemented

}; // SimpleGridDB

// End of file
