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
#include "SimpleGridDBData.hh" // HASA SimpleGridDBData
#include "SimpleGridDB.hh" // USES SimpleGridDB::QueryTy[e

#include "spatialdata/geocoords/geocoordsfwd.hh" // USES CoordSys

#include <string> // HASA std::string

class spatialdata::spatialdb::SimpleGridDBQuery {
    friend class TestSimpleGridDBQuery; // unit testing

    // PUBLIC MEMBERS ///////////////////////////////////////////////////////
public:

    /** Constructor
     *
     * @param[in] data Spatial database data.
     * @param[in] description Spatial database description.
     */
    SimpleGridDBQuery(const SimpleGridDBData& data,
                      const char* description);

    /// Destructor
    ~SimpleGridDBQuery(void);

    /** Set query type.
     *
     * @pre Must call Open() before QueryType()
     *
     * @param queryType Set type of query
     */
    void setQueryType(const SimpleGridDB::QueryEnum queryType);

    /** Set values to be returned by queries.
     *
     * @pre Must call open() before setQueryValues()
     *
     * @param names Names of values to be returned in queries
     */
    void setQueryValues(const std::vector<std::string>& names);

    /** Query the database.
     *
     * @pre Must call open() before query()
     *
     * @param values Array for computed values (output from query), vals
     *   must be allocated BEFORE calling query().
     * @param numValues Number of values expected (size of values array)
     * @param coordinates Coordinates of point for query
     * @param csCoordinates Coordinate system of coordinates
     */
    void query(double* values,
               const size_t numValues,
               const double* coordinates,
               const spatialdata::geocoords::CoordSys* csCoordinates);

    // PRIVATE METHODS //////////////////////////////////////////////////////
private:

    /** Interpolate in 1-D to get values at target location defined by
     * indices.
     *
     * @param values Array for computed values (output from query), must be
     *   allocated BEFORE calling query().
     * @param numValues Number of values expected (size of values array)
     * @param indexX Index along x dimension.
     * @param numX Number of coordinates along x dimension.
     */
    void _interpolate1D(double* values,
                        const size_t numValues,
                        const double indexX,
                        const size_t numX) const;

    /** Interpolate in 2-D to get values at target location defined by
     * indices.
     *
     * @param values Array for computed values (output from query), must be
     *   allocated BEFORE calling query().
     * @param numValues Number of values expected (size of pVals array)
     * @param indexX Index along x dimension.
     * @param numX Number of coordinates along x dimension.
     * @param indexY Index along y dimension.
     * @param numY Number of coordinates along y dimension.
     */
    void _interpolate2D(double* values,
                        const size_t numValues,
                        const double indexX,
                        const size_t numX,
                        const double indexY,
                        const size_t numY) const;

    /** Interpolate in 3-D to get values at target location defined by
     * indices.
     *
     * @param values Array for computed values (output from query), must be
     *   allocated BEFORE calling query().
     * @param numValues Number of values expected (size of pVals array)
     * @param indexX Index along x dimension.
     * @param indexY Index along y dimension.
     * @param indexZ Index along z dimension.
     */
    void _interpolate3D(double* values,
                        const size_t numValues,
                        const double indexX,
                        const double indexY,
                        const double indexZ) const;

    // PRIVATE MEMBERS //////////////////////////////////////////////////////
private:

    const SimpleGridDBData& _data; ///< Data in spatial database.
    std::string _description; ///< Spatial database description.

    double _xyz[3];

    SimpleGridDB::QueryEnum _queryType; ///< Query type
    std::vector<size_t> _queryIndices; ///< Indices of values to be returned in queries.
    std::unique_ptr<spatialdata::geocoords::Converter> _converter; /// Convert query points to local coordinate system.

    // NOT IMPLEMENTED //////////////////////////////////////////////////////
private:

    SimpleGridDBQuery(const SimpleGridDBQuery&); ///< Not implemented
    const SimpleGridDBQuery& operator=(const SimpleGridDBQuery&); ///< Not implemented

}; // SimpleGridDBQuery

// End of file
