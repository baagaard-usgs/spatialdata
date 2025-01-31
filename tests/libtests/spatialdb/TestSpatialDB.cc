// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include <portinfo>

#include "spatialdata/spatialdb/SpatialDB.hh" // Test subject

#include "spatialdata/spatialdb/SimpleDB.hh" // USES SimpleDB
#include "spatialdata/spatialdb/SimpleDBIO.hh" // USES SimpleDBIO

#include "spatialdata/geocoords/CSCart.hh" // USES CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()
#include <cassert>

// ------------------------------------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class TestSpatialDB;
    } // spatialdb
} // spatialdata

class spatialdata::spatialdb::TestSpatialDB {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor.
    TestSpatialDB(void);

    /// Destructor.
    ~TestSpatialDB(void);

    /// Test SpatialDB
    void testDB(void);

    /// Test SpatialDB queries w/multiple points.
    void testDBmulti(void);

    // PRIVATE MEMBERS ////////////////////////////////////////////////////////////////////////////
private:

    std::unique_ptr<SpatialDB> _db; ///< Test subject

}; // class TestSpatialDB

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestSpatialDB::testDB", "[TestSpatialDB]") {
    spatialdata::spatialdb::TestSpatialDB().testDB();
}
TEST_CASE("TestSpatialDB::testDBmulti", "[TestSpatialDB]") {
    spatialdata::spatialdb::TestSpatialDB().testDBmulti();
}

// ----------------------------------------------------------------------
// Constructor.
spatialdata::spatialdb::TestSpatialDB::TestSpatialDB(void) {
    std::unique_ptr<spatialdata::spatialdb::SimpleDB> db(new spatialdata::spatialdb::SimpleDB("TestSpatialDB"));assert(db);

    const char* filename = "data/spatialdb.dat";
    db->setFilename(filename);
    db->open();
    db->setQueryType(spatialdata::spatialdb::SimpleDB::NEAREST);

    _db = std::move(db);
} // setUp


// ----------------------------------------------------------------------
// Destructor.
spatialdata::spatialdb::TestSpatialDB::~TestSpatialDB(void) {}


// ----------------------------------------------------------------------
// Test SpatialDB
void
spatialdata::spatialdb::TestSpatialDB::testDB(void) {
    assert(_db);

    const std::string& description = "testDB";
    _db->setDescription(description.c_str());
    CHECK(description == std::string(_db->getDescription()));

    const std::vector<std::string> names({"two", "one", "four", "three"});
    const size_t numValues = 4;
    const double queryLoc[3] = { 0.6, 0.1, 0.2 };
    const double vals[4] = { 6.3e+3, 4.7, 0.8, 1.2e+6 };
    const int errFlags[1] = { 0 };

    _db->setQueryValues(names);

    double* valsQ = (0 < numValues) ? new double[numValues] : NULL;
    spatialdata::geocoords::CSCart csCart;
    const int err = _db->query(valsQ, numValues, queryLoc, &csCart);
    REQUIRE(err == errFlags[0]);

    const double tolerance = 1.0e-06;
    for (size_t iVal = 0; iVal < numValues; ++iVal) {
        const double toleranceV = fabs(vals[iVal]) * tolerance;
        CHECK_THAT(valsQ[iVal], Catch::Matchers::WithinAbs(vals[iVal], toleranceV));
    } // for

    delete[] valsQ;valsQ = NULL;
    _db->close();
} // testDB


// ----------------------------------------------------------------------
// Test SpatialDB queries w/multiple points.
void
spatialdata::spatialdb::TestSpatialDB::testDBmulti(void) {
    assert(_db);

    const size_t numValues = 4;
    const size_t numLocs = 2;
    const size_t spaceDim = 3;
    const std::vector<std::string> names({"two", "one", "four", "three"});
    const double queryLocs[numLocs*spaceDim] = {
        0.6, 0.1, 0.2,
        0.1, 0.6, 0.3,
    };
    const double vals[numLocs*numValues] = {
        6.3e+3, 4.7, 0.8, 1.2e+6,
        3.6e+3, 7.4, 8.0, 2.1e+6,
    };
    const int errFlags[numLocs] = { 0, 0 };

    _db->setQueryValues(names);

    size_t size = numLocs * numValues;
    double valsQ[size];
    int errQ[size];
    spatialdata::geocoords::CSCart csCart;

    _db->multiquery(valsQ, errQ, queryLocs, numLocs, numValues, &csCart);

    const double tolerance = 1.0e-06;
    for (size_t iLoc = 0; iLoc < numLocs; ++iLoc) {
        REQUIRE(errFlags[iLoc] == errQ[iLoc]);
        for (size_t iVal = 0, index = 0; iVal < numValues; ++iVal, index++) {
            const double toleranceV = fabs(vals[index]) * tolerance;
            CHECK_THAT(valsQ[index], Catch::Matchers::WithinAbs(vals[index], toleranceV));
        }
    } // for

    _db->close();
} // testDBmulti


// End of file
