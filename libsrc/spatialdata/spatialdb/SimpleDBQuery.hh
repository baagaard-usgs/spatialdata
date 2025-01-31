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

#include "spatialdbfwd.hh" // forward declarations
#include "SimpleDB.hh" // USES SimpleDB

#include "spatialdata/geocoords/geocoordsfwd.hh" // HOLDSA Converter

#include <vector> // USES std::vector

// ----------------------------------------------------------------------
class spatialdata::spatialdb::SimpleDBQuery {
    friend class TestSimpleDBQuery;

public:

    // PUBLIC METHODS /////////////////////////////////////////////////////

    /** Constructor
     *
     * @param[in] data Spatial database data.
     * @param[in] description Spatial database description.
     */
    SimpleDBQuery(const SimpleDBData& data,
                  const char* description);

    /// Default destructor.
    ~SimpleDBQuery(void);

    /// Dellocate data structures.
    void deallocate(void);

    /** Set query type.
     *
     * @param value Set type of query
     */
    void setQueryType(const SimpleDB::QueryEnum value);

    /** Set values to be returned by queries.
     *
     * @param names Names of values to be returned in queries
     */
    void setQueryValues(const std::vector<std::string>& names);

    /** Query the database.
     *
     * @param values Array for computed values (output from query)
     * @param numValues Number of values expected (size of values array)
     * @param coordinates Coordinates of point to query
     * @param csCoordinates Coordinate system of coordinates
     */
    void query(double* values,
               const size_t numValues,
               const double* coordinates,
               const spatialdata::geocoords::CoordSys* csCoordinates);

private:

    // PRIVATE STRUCT /////////////////////////////////////////////////////

    /** Interpolation weighting information */
    struct Weighting {
        double wt; ///< Weight for location
        size_t i_near; ///< Index into nearest
    }; // struct Weighting

private:

    // PRIVATE METHODS ////////////////////////////////////////////////////

    /** Query database using nearest neighbor algorithm.
     *
     * Values at location are equal to values at nearest location in
     * database.
     *
     * @param values Array for computed values (output from query)
     * @param numValues Number of values expected (size of values array)
     */
    void _queryNearest(double* values,
                       const size_t numValues);

    /** Query database using linear interpolation algorithm.
     *
     * Values at location are interpolation from locations in database.
     *
     * @param values Array for computed values (output from query)
     * @param numValues Number of values expected (size of values array)
     */
    void _queryLinear(double* values,
                      const size_t numValues);

    /// Find locations in database nearest query location.
    void _findNearest(void);

    /** Get interpolation weighting functions for query.
     *
     * @param weights Pointer to array of interpolation weights
     */
    void _getWeights(std::vector<Weighting>* weights);

    /** Get interpolation weighting functions for point interpolation.
     *
     * This routine is not actually used by a point query since it is
     * trivial. Instead it is used with the other topologies to build up
     * interpolation to the higher dimensions.
     *
     * @param weights Pointer to array of interpolation weights
     */
    void _findPointPoint(std::vector<Weighting>* weights);

    /** Get interpolation weighting functions for linear interpolation.
     *
     * @param weights Pointer to array of interpolation weights
     */
    void _findLinePoint(std::vector<Weighting>* weights);

    /** Get interpolation weighting functions for areal interpolation.
     *
     * @param weights Pointer to array of interpolation weights
     */
    void _findAreaPoint(std::vector<Weighting>* weights);

    /** Get interpolation weighting functions for volumetric interpolation.
     *
     * @param weights Pointer to array of interpolation weights
     */
    void _findVolumePoint(std::vector<Weighting>* weights);

    /** Set coordinates of point in 3-D space using coordinates in
     * current coordinate system.
     *
     * @param point3 Coordinates of point in 3D space [output].
     * @param point Coordinates of point in current coordinate system.
     * @param spaceDim Spatial dimension of current coordinate system.
     */
    static
    void _setPoint3(double* const point3,
                    const double* point,
                    const size_t spaceDim);

    /** Compute square of distance between points A and B.
     *
     * @param a Coordinates of point A
     * @param b Coordinates of point B
     */
    static double _distSquared(const double a[3],
                               const double b[3]);

    /** Compute area and "direction" of triangle abc. Direction is vector
     * normal to triangular surface.
     *
     * @param a Coordinates of point A
     * @param b Coordinates of point B
     * @param c Coordinates of point C
     */
    static void _area(double* area,
                      double dir[3],
                      const double* a,
                      const double* b,
                      const double* c);

    /** Compute volume of tetrahedral defined by four points.
     *
     * @param a Coordinates of point A
     * @param b Coordinates of point B
     * @param c Coordinates of point C
     * @param d Coordinates of point D
     */
    static double _volume(const double a[3],
                          const double b[3],
                          const double c[3],
                          const double d[3]);

private:

    // PRIVATE MEMBERS ////////////////////////////////////////////////////

    const SimpleDBData& _data; ///< Reference to simple database.
    std::string _description; ///< Reference to simple database description.

    double _queryPoint[3]; ///< Location of query.
    SimpleDB::QueryEnum _queryType; ///< Query type.
    std::vector<size_t> _nearest; ///< Index of nearest points in database to location.
    std::vector<size_t> _queryIndices; ///< Indices of values to be returned in queries.
    std::unique_ptr<spatialdata::geocoords::Converter> _converter; ///< Covert query points to local coordinate system.

}; // class SimpleDBQuery

// End of file
