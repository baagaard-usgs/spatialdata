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

#include "spatialdata/muparser/muparserfwd.hh" // HOLDSA mu::parser

#include <string> // HASA std::string

/// C++ manager for analytic spatial database.
class spatialdata::spatialdb::AnalyticDB : public SpatialDB {
    friend class TestAnalyticDB; // unit testing

public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Constructor with description.
     *
     * @param description Description of database
     */
    AnalyticDB(const char* description);

    /// Default destructor.
    ~AnalyticDB(void);

    /// Clear values from database.
    void clear(void);

    /** Set expressions in database.
     *
     * @param names Array of names of expressions in database.
     * @param units Array of units for expressions in database.
     * @param expressions Array of expressions in database.
     */
    void setData(const std::vector<std::string>& names,
                 const std::vector<std::string>& units,
                 const std::vector<std::string>& expressions);

    /** Set coordinate system associated with user functions.
     *
     * @param cs Coordinate system.
     */
    void setCoordSys(const std::shared_ptr<spatialdata::geocoords::CoordSys>& cs);

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

    AnalyticDB(void); ///< Not implemented
    AnalyticDB(const AnalyticDB& data); ///< Not implemented
    const AnalyticDB& operator=(const AnalyticDB& data); ///< Not implemented

private:

    // PRIVATE MEMBERS /////////////////////////////////////////////////////

    std::vector<mu::Parser> _parsers; ///< Handle for muParser context
    std::vector<std::string> _names; ///< Names of values in database
    std::vector<double> _scales; ///< Scales to convert expressions to SI units.
    std::vector<std::string> _expressions; ///< Expressions in database
    std::vector<size_t> _queryIndices; ///< Indices of values to be returned in queries.
    mu::value_type _expressionVars[3]; ///< Storage for default expression variables
    std::shared_ptr<spatialdata::geocoords::CoordSys> _cs; ///< Coordinate system
    std::unique_ptr<spatialdata::geocoords::Converter> _converter; /// Convert query points to local coordinate system.

}; // class AnalyticDB

#include "AnalyticDB.icc"

// End of file
