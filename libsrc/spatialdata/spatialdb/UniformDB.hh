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

#include "SpatialDB.hh"

#include <string> // HASA std::string

/// C++ manager for simple spatial database.
class spatialdata::spatialdb::UniformDB : public SpatialDB { // class UniformDB
    friend class TestUniformDB; // unit testing

public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Constructor with description.
     *
     * @param description Description of database
     */
    UniformDB(const char* description);

    /// Default destructor.
    ~UniformDB(void);

    /** Set values in database.
     *
     * @param names Array of names of values in database.
     * @param units Array of units for values in database.
     * @param values Array of values in database.
     */
    void setData(const std::vector<std::string>& names,
                 const std::vector<std::string>& units,
                 const std::vector<double>& values);

    /// Open the database and prepare for querying.
    void open(void) override;

    /// Close the database.
    void close(void) override;

    /** Get names of values in spatial database.
     *
     * @param[out] valueNames Array of names of values.
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

    UniformDB(void); ///< Not implemented
    UniformDB(const UniformDB& data); ///< Not implemented
    const UniformDB& operator=(const UniformDB& data); ///< Not implemented

private:

    // PRIVATE MEMBERS /////////////////////////////////////////////////////

    std::vector<double> _values; ///< Values in database
    std::vector<std::string> _names; ///< Names of values in database
    std::vector<size_t> _queryIndices; ///< Indices of values to be returned in queries.
}; // class UniformDB

#include "UniformDB.icc"

// End of file
