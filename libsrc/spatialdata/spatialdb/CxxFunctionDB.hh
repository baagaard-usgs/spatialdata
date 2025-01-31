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
#include <map> // HASA std::map

class spatialdata::spatialdb::CxxFunctionDB : public SpatialDB {
    friend class TestCxxFunctionDB; // unit testing

public:

    // PUBLIC TYPEDEF//////////////////////////////////////////////////////

    /** User function prototype in 1-D.
     *
     * @param x X coordinate.
     * @returns Value of user-defined function.
     */
    typedef double (*cxxfn1D_type)(const double x);

    /** User function prototype in 2-D.
     *
     * @param x X coordinate.
     * @param y Y coordinate.
     * @returns Value of user-defined function.
     */
    typedef double (*cxxfn2D_type)(const double x,
                                   const double y);

    /** User function prototype in 3-D.
     *
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     * @returns Value of user-defined function.
     */
    typedef double (*cxxfn3D_type)(const double x,
                                   const double y,
                                   const double z);

    // PUBLIC MEMBERS ///////////////////////////////////////////////////////
public:

    /** Constructor with description.
     *
     * @param description Description of database
     */
    CxxFunctionDB(const char* description);

    /// Destructor
    ~CxxFunctionDB(void);

    /** Add function/value to database in 1-D.
     *
     * @param name Name of value for function.
     * @param fn User function for value.
     * @param units Units associated with function value.
     */
    void addValue(const char* name,
                  cxxfn1D_type fn,
                  const char* units);

    /** Add function/value to database in 2-D.
     *
     * @param name Name of value for function.
     * @param fn User function for value.
     * @param units Units associated with function value.
     */
    void addValue(const char* name,
                  cxxfn2D_type fn,
                  const char* units);

    /** Add function/value to database in 3-D.
     *
     * @param name Name of value for function.
     * @param fn User function for value.
     * @param units Units associated with function value.
     */
    void addValue(const char* name,
                  cxxfn3D_type fn,
                  const char* units);

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
     * @param numValues Number of values to be returned in queries
     */
    void setQueryValues(const std::vector<std::string>& names) override;

    /** Query the database.
     *
     * @note pVals should be preallocated to accommodate numValues values.
     *
     * @pre Must call open() before query()
     *
     * @param values Array for computed values (output from query), must be
     *   allocated BEFORE calling query().
     * @param numValues Number of values expected (size of values array)
     * @param coordinates Coordinates of point for query
     * @param csCoordinates Coordinate system of coordinates
     *
     * @returns 0 on success, 1 on failure.
     */
    int query(double* values,
              const size_t numValues,
              const double* coordinates,
              const spatialdata::geocoords::CoordSys* csCoordinates) override;

    /** Set coordinate system associated with user functions.
     *
     * @param cs Coordinate system.
     */
    void setCoordSys(const std::shared_ptr<geocoords::CoordSys>& cs);

private:

    // PRIVATE TYPEDEF//////////////////////////////////////////////////////

    /** User function prototype.
     *
     * @param value Return value of function.
     * @param coords Coordinates of point.
     * @param dim Spatial dimension of coordinate system.
     * @returns 0 if successful, 1 on failure.
     */
    typedef int (*queryfn_type)(double* value,
                                const double* coords,
                                const size_t dim);

    // PRIVATE METHODS //////////////////////////////////////////////////////
private:

    /** Check suitability of arguments for adding user function.
     *
     * @parma[in] name Name of value.
     * @param[in] fn User-defined function for value.
     * @param[in] units Units of value.
     */
    void _checkAdd(const char* name,
                   void* fn,
                   const char* units) const;

    /// Check compatibility of spatial database parameters.
    void _checkCompatibility(void) const;

    // PRIVATE STRUCTS //////////////////////////////////////////////////////
private:

    class CxxFn {
public:

        CxxFn(void) {}


        virtual ~CxxFn(void) {}


        virtual int query(double* value,
                          const double* coords,
                          const size_t dim) = 0;

    };
    class CxxFn1D;
    class CxxFn2D;
    class CxxFn3D;

    /// Structure for holding user data
    struct QueryFn {
        std::shared_ptr<CxxFn> fn; ///< User-defined function for query.
        std::string units; ///< Units for value of user function.
        double scale; ///< Scale to convert to SI units.
    }; // QueryFn

    typedef std::map<std::string, QueryFn> function_map;

    // PRIVATE MEMBERS //////////////////////////////////////////////////////
private:

    std::vector<QueryFn> _functions; ///< Query functions for each value.
    std::vector<size_t> _queryIndices; ///< Indices for queries
    std::vector<std::string> _names; ///< Names of values in spatial database.
    std::shared_ptr<spatialdata::geocoords::CoordSys> _cs; ///< Coordinate system
    std::unique_ptr<spatialdata::geocoords::Converter> _converter; ///< Convert query points to local coordinate
                                                                   ///< system.U

    // NOT IMPLEMENTED //////////////////////////////////////////////////////
private:

    CxxFunctionDB(void); ///< Not implemented
    CxxFunctionDB(const CxxFunctionDB&); ///< Not implemented
    const CxxFunctionDB& operator=(const CxxFunctionDB&); ///< Not implemented

}; // CxxFunctionDB

// End of file
