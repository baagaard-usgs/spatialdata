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

#include "spatialdata/spatialdb/CxxFunctionDB.hh" // Test subject

/// Namespace for spatial package
namespace spatialdata {
    namespace spatialdb {
        class TestCxxFunctionDB;
        class TestCxxFunctionDB_Data;
    } // spatialdb
} // spatialdata

// ------------------------------------------------------------------------------------------------
class spatialdata::spatialdb::TestCxxFunctionDB {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor.
    TestCxxFunctionDB(TestCxxFunctionDB_Data* data,
                      CxxFunctionDB* db);

    /// Destructor.
    ~TestCxxFunctionDB(void);

    /// Test constructor
    static
    void testConstructor(void);

    /// Test setDescription()
    static
    void testDescription(void);

    /// Test coordsys()
    static
    void testCoordsys(void);

    /// Test addValue()
    void testAddValue(void);

    /// Test open() and close()
    void testOpenClose(void);

    /// Test getNamesDBValues().
    void testGetNamesDBValues(void);

    /// Test setQueryValues().
    void testQueryValues(void);

    /// Test query().
    void testQuery(void);

protected:

    // PROTECTED MEMBERS //////////////////////////////////////////////////

    std::unique_ptr<CxxFunctionDB> _db; ///< Test subject.
    std::unique_ptr<TestCxxFunctionDB_Data> _data; ///< Test data.

}; // class TestCxxFunctionDB

// ------------------------------------------------------------------------------------------------
class spatialdata::spatialdb::TestCxxFunctionDB_Data {
    // PUBLIC STRUCTS ///////////////////////////////////////////////////////
public:

    /// Structure for holding user data
    struct UserData {
        std::string name; ///< Name of value.
        std::string units; ///< Units for value of user function.
        double scale; ///< Scale to convert to SI units.
    }; // UserData

    // PUBLIC METHODS ///////////////////////////////////////////////////////
public:

    /// Constructor
    TestCxxFunctionDB_Data(void);

    /// Destructor
    ~TestCxxFunctionDB_Data(void);

    // PUBLIC MEMBERS ///////////////////////////////////////////////////////
public:

    size_t numValues; ///< Number of values in spatial database.
    const UserData* values; ///< Names of values in spatial database.
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs; ///< Coordinate system.

    const double* queryXYZ; ///< Coordinate sof points in test queries.
    const double* queryValues; ///< Expected values in test queries.
    int numQueryPoints; ///< Number of points in test queries.

}; // TestCxxFunctionDB_Data

// End of file
