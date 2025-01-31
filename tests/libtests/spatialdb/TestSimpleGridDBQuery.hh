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

#include "spatialdata/spatialdb/spatialdbfwd.hh"

#include <memory> // HASA std::unique_ptr
#include <cstddef> // USES size_t

namespace spatialdata {
    namespace spatialdb {
        class TestSimpleGridDBQuery;
        class TestSimpleGridDBQuery_Data;
    } // spatialdb
} // spatialdata

// ------------------------------------------------------------------------------------------------
class spatialdata::spatialdb::TestSimpleGridDBQuery {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor.
    TestSimpleGridDBQuery(TestSimpleGridDBQuery_Data* data);

    /// Destructor.
    ~TestSimpleGridDBQuery(void);

    /// Test constructor.
    static
    void testConstructor(void);

    /// Test accessors.
    static
    void testAccessors(void);

    /// Test setQueryValues()
    void testQueryValues(void);

    /// Test query() using nearest neighbor.
    void testQueryNearest(void);

    /// Test query() using linear interpolation.
    void testQueryLinear(void);

    // PRIVATE METHODS ////////////////////////////////////////////////////////////////////////////
private:

    /// Populate database with data.
    void _initializeDB(void);

    /** Test query method by doing query and checking values returned.
     *
     * @param queryData Data for query.
     * @param flagsE Array of expected return values
     */
    void _checkQuery(const double* queryData,
                     const int* flagsE);

    // PROTECTED MEMBERS //////////////////////////////////////////////////////////////////////////
protected:

    std::unique_ptr<SimpleGridDBData> _data; ///< Database for test subject.
    std::unique_ptr<SimpleGridDBQuery> _query; ///< Test subject.
    std::unique_ptr<TestSimpleGridDBQuery_Data> _testData; ///< Test data.

}; // class TestSimpleGridDBQuery

// ------------------------------------------------------------------------------------------------
class spatialdata::spatialdb::TestSimpleGridDBQuery_Data {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor
    TestSimpleGridDBQuery_Data(void);

    /// Destructor
    ~TestSimpleGridDBQuery_Data(void);

    // PUBLIC MEMBERS ///////////////////////////////////////////////////////
public:

    /// @name Database information
    //@{
    size_t numX; ///< Number of locations in x dimension.
    size_t numY; ///< Number of locations in y dimension.
    size_t numZ; ///< Number of locations in z dimension.
    size_t spaceDim; ///< Spatial dimension for coordinates of locations
    size_t numValues; ///< Number of values per location in database
    size_t dataDim; ///< Spatial dimension of data in database
    const double* dbX; ///< Coordinates along x dimension.
    const double* dbY; ///< Coordinates along x dimension.
    const double* dbZ; ///< Coordinates along x dimension.
    const double* dbData; ///< Database data
    const char** names; ///< Names of values in database
    const char** units; ///< Units of values in database
    const char* description; ///< Description of database.
    //@}

    /// @name Query information
    //@{
    size_t numQueries; ///< Number of queries
    const double* queryNearest; ///< Data for nearest neighbor queries
    const double* queryLinear; ///< Data for linear interpolation queries
    const int* errFlags; ///< Expected return values for queries
    //@}

}; // TestSimpleGridDBQuery

// End of file
