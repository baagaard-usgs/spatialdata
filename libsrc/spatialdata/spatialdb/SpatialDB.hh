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

#include "spatialdbfwd.hh"
#include "spatialdata/geocoords/geocoordsfwd.hh"

#include <string> // USES std::string
#include <memory> // USES std::shared_ptr

/// C++ manager for spatial database.
class spatialdata::spatialdb::SpatialDB {
public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Constructor with description.
     *
     * @param description Description for database
     */
    SpatialDB(const char* description);

    /// Default destructor.
    virtual ~SpatialDB(void);

    /** Set description of spatial database.
     *
     * @param[in] description Description of database.
     */
    void setDescription(const char* description);

    /** Get description of spatial database.
     *
     * @returns Description of database
     */
    const char* getDescription(void) const;

    /// Open the database and prepare for querying.
    virtual
    void open(void) = 0;

    /// Close the database.
    virtual
    void close(void) = 0;

    /** Get names of values in spatial database.
     *
     * @returns Array of names of values.
     */
    virtual
    const std::vector<std::string>& getNamesDBValues(void) const = 0;

    /** Set values to be returned by queries.
     *
     * @pre Must call open() before setQueryValues()
     *
     * @param names Names of values to be returned in queries
     */
    virtual
    void setQueryValues(const std::vector<std::string>& names) = 0;

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
    virtual
    int query(double* values,
              const size_t numValues,
              const double* coordinates,
              const spatialdata::geocoords::CoordSys* csCoordinates) = 0;

    /** Perform multiple queries of the database.
     *
     * @note values should be preallocated to accommodate numValues values
     * at numLocs locations.
     *
     * @note err should be preallocated to accommodate numLocs values.
     *
     * @pre Must call open() before query().
     *
     * @param values Array for computed values (output from query), must be
     *   allocated BEFORE calling query() [numLocs*numValues].
     * @param err Array for error flag values (output from query), must be
     *   allocated BEFORE calling query() [numLocs].
     * @param coordinates Coordinates of point for query [numLocs*numDims].
     * @param numLocs Number of locations.
     * @param numValues Number of values expected.
     * @param csCoordinates Coordinate system of coordinates.
     */
    void multiquery(double* values,
                    int* err,
                    const double* coordinates,
                    const size_t numLocs,
                    const size_t numValues,
                    const spatialdata::geocoords::CoordSys* csCoordinates);

    // PRIVATE METHODS ////////////////////////////////////////////////////
private:

    SpatialDB(void); ///< Not implemented
    SpatialDB(const SpatialDB& data); ///< Not implemented
    const SpatialDB& operator=(const SpatialDB& data); ///< Not implemented

private:

    // PRIVATE MEMBERS ////////////////////////////////////////////////////

    std::string _description; ///< Description of spatial database.

}; // class SpatialDB

// End of file
