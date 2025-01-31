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

#include "spatialdata/spatialdb/CompositeDB.hh" // USES CompositeDB

#include "spatialdata/spatialdb/UniformDB.hh" // USES UniformDB
#include "spatialdata/geocoords/CSCart.hh" // USES CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ------------------------------------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class TestCompositeDB;
    } // spatialdb
} // spatialdata

class spatialdata::spatialdb::TestCompositeDB {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Constructor.
    TestCompositeDB(void);

    /// Test constructors
    static
    void testConstructors(void);

    /// Test accessors.
    void testAccessors(void);

    /// Test getNamesDBValues().
    void testGetNamesDBValues(void);

    /// Test setQueryValues() with values in two databases.
    void testQueryValuesAB(void);

    /// Test setQueryValues() with values in first database
    void testQueryValuesA(void);

    /// Test setQueryValues() with values in second dataset.
    void testQueryValuesB(void);

    /// Test query() with values in both databases.
    void testQueryAB(void);

    /// Test query() with values in first database.
    void testQueryA(void);

    /// Test query() with values in second database.
    void testQueryB(void);

private:

    std::shared_ptr<SpatialDB> _dbA; ///< Spatial database A.
    std::shared_ptr<SpatialDB> _dbB; ///< Spatial database B.

}; // class TestCompositeDB

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestCompositeDB::testConstructors", "[TestCompositeDB]") {
    spatialdata::spatialdb::TestCompositeDB::testConstructors();
}

TEST_CASE("TestCompositeDB::testAccessors", "[TestCompositeDB]") {
    spatialdata::spatialdb::TestCompositeDB().testAccessors();
}
TEST_CASE("TestCompositeDB::testGetNamesDBValues", "[TestCompositeDB]") {
    spatialdata::spatialdb::TestCompositeDB().testGetNamesDBValues();
}
TEST_CASE("TestCompositeDB::testQueryValuesAB", "[TestCompositeDB]") {
    spatialdata::spatialdb::TestCompositeDB().testQueryValuesAB();
}
TEST_CASE("TestCompositeDB::testQueryValuesA", "[TestCompositeDB]") {
    spatialdata::spatialdb::TestCompositeDB().testQueryValuesA();
}
TEST_CASE("TestCompositeDB::testQueryValuesB", "[TestCompositeDB]") {
    spatialdata::spatialdb::TestCompositeDB().testQueryValuesB();
}
TEST_CASE("TestCompositeDB::testQueryAB", "[TestCompositeDB]") {
    spatialdata::spatialdb::TestCompositeDB().testQueryAB();
}
TEST_CASE("TestCompositeDB::testQueryA", "[TestCompositeDB]") {
    spatialdata::spatialdb::TestCompositeDB().testQueryA();
}
TEST_CASE("TestCompositeDB::testQueryB", "[TestCompositeDB]") {
    spatialdata::spatialdb::TestCompositeDB().testQueryB();
}

// ------------------------------------------------------------------------------------------------
spatialdata::spatialdb::TestCompositeDB::TestCompositeDB(void) {
    { // initialize db A
        std::shared_ptr<UniformDB> db(new UniformDB("dbA"));REQUIRE(db);
        const std::vector<std::string> names({ "one", "two", "three" });
        const std::vector<std::string> units({ "none", "none", "none" });
        const std::vector<double> values({ 1.1, 2.2, 3.3 });
        db->setData(names, units, values);
        _dbA = db;
    } // initialize db A

    { // initialize db B
        std::shared_ptr<UniformDB> db(new UniformDB("dbA"));REQUIRE(db);
        const std::vector<std::string> names({ "four", "five" });
        const std::vector<std::string> units({ "none", "none" });
        const std::vector<double> values({ 4.4, 5.5 });
        db->setData(names, units, values);
        _dbB = db;
    } // initialize db B

} // constructor


// ----------------------------------------------------------------------
// Test constructors.
void
spatialdata::spatialdb::TestCompositeDB::testConstructors(void) {
    const std::string& description = "database A";
    CompositeDB db(description.c_str());
    CHECK(description == std::string(db.getDescription()));
} // testConstructors


// ----------------------------------------------------------------------
// Test accessors.
void
spatialdata::spatialdb::TestCompositeDB::testAccessors(void) {
    const std::string& description = "testAccessors";

    CompositeDB db(description.c_str());
    CHECK(description == std::string(db.getDescription()));

    // Set database A
    const std::vector<std::string> namesA({ "three", "one" });
    db.addDB(_dbA, namesA);
    CHECK(1 == db._dbs.size());

    // Set database B
    const std::vector<std::string> namesB({ "five" });
    db.addDB(_dbB, namesB);
    CHECK(2 == db._dbs.size());
} // testAccessors


// ----------------------------------------------------------------------
// Test getNamesDBValues().
void
spatialdata::spatialdb::TestCompositeDB::testGetNamesDBValues(void) {
    CompositeDB db("testGetNamesDBValues");

    // Set database A
    const std::vector<std::string> namesA({ "three", "one" });
    db.addDB(_dbA, namesA);

    // Set database B
    const std::vector<std::string> namesB({ "five" });
    db.addDB(_dbB, namesB);

    const std::vector<std::string>& namesDB = db.getNamesDBValues();
    REQUIRE(namesDB.size() == namesA.size() + namesB.size());

    size_t iAB = 0;
    for (size_t iA = 0; iA < namesA.size(); ++iA, ++iAB) {
        CHECK(namesA[iA] == namesDB[iAB]);
    } // for
    for (size_t iB = 0; iB < namesB.size(); ++iB, ++iAB) {
        CHECK(namesB[iB] == namesDB[iAB]);
    } // for
} // testGetDBValues


// ----------------------------------------------------------------------
// Test setQueryValues() with values in dbA and dbB.
void
spatialdata::spatialdb::TestCompositeDB::testQueryValuesAB(void) {
    CompositeDB db("testQueryValuesAB");

    const std::vector<std::string> namesA({ "three", "one" });
    db.addDB(_dbA, namesA);

    const std::vector<std::string> namesB({ "five" });
    db.addDB(_dbB, namesB);

    db.open();

    { // Check defaults (all values in A and then all values in B).
        const std::vector<std::string> namesE({"three", "one", "five"});
        REQUIRE(namesE == db.getNamesDBValues());
    } // Check defaults

    const std::vector<std::string> queryValues({ "one", "five", "three" });
    db.setQueryValues(queryValues);
    db.close();

    { // Check values
        const size_t querySizes[2] = {2, 1};
        const size_t queryIndices[3] = {
            0, 2, // dbA
            1, // dbB
        };
        size_t indicesIndex = 0;
        size_t sizeIndex = 0;
        for (const CompositeDB::DBEntry& entry : db._dbs) {
            const size_t querySizeDB = entry.queryIndices.size();
            REQUIRE(querySizes[sizeIndex++] == querySizeDB);
            for (size_t iQuery = 0; iQuery < querySizeDB; ++iQuery) {
                CHECK(queryIndices[indicesIndex++] == entry.queryIndices[iQuery]);
            } // for
        } // for
    } // Check values
} // testQueryValuesAB


// ----------------------------------------------------------------------
// Test setQueryValues() with values in dbA.
void
spatialdata::spatialdb::TestCompositeDB::testQueryValuesA(void) {
    CompositeDB db("testQueryValuesA");

    const std::vector<std::string> namesA({ "three", "one" });
    db.addDB(_dbA, namesA);

    const std::vector<std::string> namesB({ "five" });
    db.addDB(_dbB, namesB);

    db.open();

    const std::vector<std::string> queryValues({ "one", "three" });
    db.setQueryValues(queryValues);
    db.close();

    { // Check values
        const size_t querySizes[2] = {2, 0};
        const size_t queryIndices[2] = {
            0, 1, // dbA
        };
        size_t indicesIndex = 0;
        size_t sizeIndex = 0;
        for (const CompositeDB::DBEntry& entry : db._dbs) {
            const size_t querySizeDB = entry.queryIndices.size();
            REQUIRE(querySizes[sizeIndex++] == querySizeDB);
            for (size_t iQuery = 0; iQuery < querySizeDB; ++iQuery) {
                CHECK(queryIndices[indicesIndex++] == entry.queryIndices[iQuery]);
            } // for
        } // for
    } // Check values
} // testQueryValuesA


// ----------------------------------------------------------------------
// Test setQueryValues() with values in dbB.
void
spatialdata::spatialdb::TestCompositeDB::testQueryValuesB(void) {
    CompositeDB db("testQueryValuesB");

    const std::vector<std::string> namesA({ "three", "one" });
    db.addDB(_dbA, namesA);

    const std::vector<std::string> namesB({ "five" });
    db.addDB(_dbB, namesB);

    db.open();

    const std::vector<std::string> queryValues({ "five" });
    db.setQueryValues(queryValues);
    db.close();

    { // Check values
        const size_t querySizes[2] = {0, 1};
        const size_t queryIndices[1] = {
            0, // dbB
        };
        size_t indicesIndex = 0;
        size_t sizeIndex = 0;
        for (const CompositeDB::DBEntry& entry : db._dbs) {
            const size_t querySizeDB = entry.queryIndices.size();
            REQUIRE(querySizes[sizeIndex++] == querySizeDB);
            for (size_t iQuery = 0; iQuery < querySizeDB; ++iQuery) {
                CHECK(queryIndices[indicesIndex++] == entry.queryIndices[iQuery]);
            } // for
        } // for
    } // Check values
} // testQueryValuesB


// ----------------------------------------------------------------------
// Test query() with values in both dbA and dbB.
void
spatialdata::spatialdb::TestCompositeDB::testQueryAB(void) {
    CompositeDB db("testQueryValuesB");

    const std::vector<std::string> namesA({ "three", "one" });
    db.addDB(_dbA, namesA);

    const std::vector<std::string> namesB({ "five" });
    db.addDB(_dbB, namesB);

    db.open();

    const std::vector<std::string> queryValues({ "five", "one" });
    const size_t querySize = queryValues.size();

    const size_t spaceDim = 2;
    spatialdata::geocoords::CSCart cs;
    cs.setSpaceDim(spaceDim);
    const double coordinates[2] = { 2.3, 5.6 };
    std::vector<double> values(querySize);
    const double valuesE[2] = { 5.5, 1.1 };

    db.open();
    db.setQueryValues(queryValues);
    db.query(values.data(), querySize, coordinates, &cs);
    db.close();

    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < querySize; ++i) {
        const double toleranceV = fabs(valuesE[i]) > 0.0 ? tolerance*valuesE[i] : tolerance;
        CHECK_THAT(values[i], Catch::Matchers::WithinAbs(valuesE[i], toleranceV));
    } // for
} // testQueryAB


// ----------------------------------------------------------------------
// Test query() with values in dbA.
void
spatialdata::spatialdb::TestCompositeDB::testQueryA(void) { // testQueryA
    CompositeDB db("testQueryValuesB");

    const std::vector<std::string> namesA({ "three", "one" });
    db.addDB(_dbA, namesA);

    const std::vector<std::string> namesB({ "five" });
    db.addDB(_dbB, namesB);

    db.open();

    const std::vector<std::string> queryValues({ "three" });
    const size_t querySize = queryValues.size();

    const size_t spaceDim = 2;
    spatialdata::geocoords::CSCart cs;
    cs.setSpaceDim(spaceDim);
    const double coordinates[2] = { 2.3, 5.6 };
    std::vector<double> values(querySize);
    const double valuesE[2] = { 3.3 };

    db.open();
    db.setQueryValues(queryValues);
    db.query(values.data(), querySize, coordinates, &cs);
    db.close();

    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < querySize; ++i) {
        const double toleranceV = fabs(valuesE[i]) > 0.0 ? tolerance*valuesE[i] : tolerance;
        CHECK_THAT(values[i], Catch::Matchers::WithinAbs(valuesE[i], toleranceV));
    } // for
} // testQueryA


// ----------------------------------------------------------------------
// Test query().
void
spatialdata::spatialdb::TestCompositeDB::testQueryB(void) {
    CompositeDB db("testQueryValuesB");

    const std::vector<std::string> namesA({ "three", "one" });
    db.addDB(_dbA, namesA);

    const std::vector<std::string> namesB({ "five" });
    db.addDB(_dbB, namesB);

    db.open();

    const std::vector<std::string> queryValues({ "five" });
    const size_t querySize = queryValues.size();

    const size_t spaceDim = 2;
    spatialdata::geocoords::CSCart cs;
    cs.setSpaceDim(spaceDim);
    const double coordinates[2] = { 2.3, 5.6 };
    std::vector<double> values(querySize);
    const double valuesE[2] = { 5.5 };

    db.open();
    db.setQueryValues(queryValues);
    db.query(values.data(), querySize, coordinates, &cs);
    db.close();

    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < querySize; ++i) {
        const double toleranceV = fabs(valuesE[i]) > 0.0 ? tolerance*valuesE[i] : tolerance;
        CHECK_THAT(values[i], Catch::Matchers::WithinAbs(valuesE[i], toleranceV));
    } // for
} // testQueryB


// End of file
