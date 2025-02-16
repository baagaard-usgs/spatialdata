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

#include "spatialdata/spatialdb/SpatialDB.hh" // ISA SpatialDB

// Forward declaration of the C++ UniformVelModel object in the
// contrib/spatialdb namespace.
namespace contrib {
    namespace spatialdb {
        class UniformVelModel;
    } // spatialdb
} // contrib

class contrib::spatialdb::UniformVelModel :
    public spatialdata::spatialdb::SpatialDB // inheritance from SpatialDB
{
    // Allow the unit testing TestUniformVelModel object access to
    // private class methods and members.
    friend class TestUniformVelModel;

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Constructor
    UniformVelModel(const char* description);

    /// Destructor
    ~UniformVelModel(void);

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    // Methods required to satisfy the SpatialDB interface.

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
     * @param names Names of values to be returned in queries
     */
    void setQueryValues(const std::vector<std::string>& names) override;

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

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    // Functions specific to this type of spatial database.

    /** Set the P wave speed.
     *
     * @param value P wave speed in m/s.
     */
    void setVp(const double value);

    /** Set the S wave speed.
     *
     * @param value S wave speed in m/s.
     */
    void setVs(const double value);

    /** Set the density.
     *
     * @param value Density in kg/m**3.
     */
    void setDensity(const double value);

    // PRIVATE MEMBERS //////////////////////////////////////////////////////
private:

    // Physical property information (hardwired in this case). Replace
    // with data structures for holding the physical property
    // information.

    double _vp; ///< P wave speed
    double _vs; ///< S wave speed
    double _density; ///< Density

    /// Coordinate system for velocity model. When querying the velocity
    /// model for physical properties, the coordinates of the query are
    /// transformed into this coordinate system.
    std::shared_ptr<spatialdata::geocoords::CSGeo> _cs; ///< Coordinate system
    double _xyz[3]; ///< Array used in conversion of coordinates.

    std::vector<std::string> _names; ///< Names of values in database
    std::vector<size_t> _queryIndices; ///< Indices of values to be returned in queries.

    // NOT IMPLEMENTED //////////////////////////////////////////////////////
private:

    UniformVelModel(void); ///< Not implemented
    UniformVelModel(const UniformVelModel&); ///< Not implemented
    const UniformVelModel& operator=(const UniformVelModel&); ///< Not implemented

}; // UniformVelModel

// End of file
