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

#include "spatialdata/spatialdb/UniformDB.hh" // Test subject

#include "spatialdata/geocoords/CSCart.hh" // USES CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ------------------------------------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class TestUniformDB;
    }
} // spatialdata

class spatialdata::spatialdb::TestUniformDB {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Test constructors
    static
    void testConstructors(void);

    /// Test accessors.
    static
    void testAccessors(void);

    /// Test setData()
    static
    void testSetData(void);

    /// Test getNamesDBValues().
    static
    void testGetNamesDBValues(void);

    /// Test setQueryValues()
    static
    void testQueryValues(void);

    /// Test query()
    static
    void testQuery(void);

}; // class TestUniformDB

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestUniformDB::testConstructors", "[TestUniformDB]") {
    spatialdata::spatialdb::TestUniformDB::testConstructors();
}
TEST_CASE("TestUniformDB::testAccessors", "[TestUniformDB]") {
    spatialdata::spatialdb::TestUniformDB::testAccessors();
}
TEST_CASE("TestUniformDB::testSetData", "[TestUniformDB]") {
    spatialdata::spatialdb::TestUniformDB::testSetData();
}
TEST_CASE("TestUniformDB::testGetNamesDBValues", "[TestUniformDB]") {
    spatialdata::spatialdb::TestUniformDB::testGetNamesDBValues();
}
TEST_CASE("TestUniformDB::testQueryValues", "[TestUniformDB]") {
    spatialdata::spatialdb::TestUniformDB::testQueryValues();
}
TEST_CASE("TestUniformDB::testQuery", "[TestUniformDB]") {
    spatialdata::spatialdb::TestUniformDB::testQuery();
}

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
spatialdata::spatialdb::TestUniformDB::testConstructors(void) {
    const std::string& description = "TestUniformDB::testConstructors";
    UniformDB db(description.c_str());

    CHECK(description == std::string(db.getDescription()));
} // testConstructors


// ----------------------------------------------------------------------
// Test accessors().
void
spatialdata::spatialdb::TestUniformDB::testAccessors(void) {
    const std::string& description = "TestUniformDB::testAccessors";

    UniformDB db(description.c_str());
    CHECK(description == std::string(db.getDescription()));
} // testAccessors


// ----------------------------------------------------------------------
// Test setData().
void
spatialdata::spatialdb::TestUniformDB::testSetData(void) {
    UniformDB db("TestUniformDB::testSetData");

    const size_t numValues = 3;
    const std::vector<std::string> names({ "one", "two", "three" });
    const std::vector<std::string> units({ "m", "km", "cm" });
    const std::vector<double> values({ 1.1, 2.2, 3.3 });
    const double valuesE[numValues] = { 1.1, 2.2e+3, 3.3e-2 };

    db.setData(names, units, values);

    REQUIRE(numValues == db._names.size());
    for (size_t i = 0; i < numValues; ++i) {
        CHECK(std::string(names[i]) == db._names[i]);
    } // for

    REQUIRE(numValues == db._values.size());
    for (size_t i = 0; i < numValues; ++i) {
        CHECK(valuesE[i] == db._values[i]);
    } // for

    REQUIRE(numValues == db._queryIndices.size());
    for (size_t i = 0; i < numValues; ++i) {
        CHECK(i == db._queryIndices[i]);
    } // for
} // testSetData


// ----------------------------------------------------------------------
// Test getNamesDBValues().
void
spatialdata::spatialdb::TestUniformDB::testGetNamesDBValues(void) {
    const size_t numValuesE = 3;
    const std::vector<std::string> namesE({ "one", "two", "three" });
    const std::vector<std::string> units({ "none", "none", "none" });
    const std::vector<double> values({ 1.1, 2.2, 3.3 });

    UniformDB db("TestUniformDB::testGetNamesDBValues");
    db.setData(namesE, units, values);

    const std::vector<std::string>& names = db.getNamesDBValues();
    REQUIRE(numValuesE == names.size());
    for (size_t i = 0; i < numValuesE; ++i) {
        CHECK(std::string(namesE[i]) == std::string(names[i]));
    } // for
} // testGetDBValues


// ----------------------------------------------------------------------
// Test setQueryValues().
void
spatialdata::spatialdb::TestUniformDB::testQueryValues(void) {
    UniformDB db("TestUniformDB::testQueryValues");

    const std::vector<std::string> names({ "one", "two", "three" });
    const std::vector<std::string> units({ "none", "none", "none" });
    const std::vector<double> values({ 1.1, 2.2, 3.3 });

    const std::vector<std::string> queryNames({ "three", "two" });
    const size_t querySize = 2;
    const size_t queryVals[querySize] = { 2, 1 };

    db.setData(names, units, values);
    db.setQueryValues(queryNames);

    REQUIRE(querySize == db._queryIndices.size());
    for (size_t i = 0; i < querySize; ++i) {
        CHECK(queryVals[i] == db._queryIndices[i]);
    } // for
} // testQueryValues


// ----------------------------------------------------------------------
// Test query().
void
spatialdata::spatialdb::TestUniformDB::testQuery(void) {
    UniformDB db("TestUniformDB::testQuery");

    const std::vector<std::string> names({ "one", "two", "three" });
    const std::vector<std::string> units({ "none", "none", "none" });
    const std::vector<double> values({ 1.1, 2.2, 3.3 });

    const std::vector<std::string> queryNames({ "three", "two" });
    const size_t querySize = 2;
    const size_t queryVals[querySize] = { 2, 1 };

    db.setData(names, units, values);
    db.setQueryValues(queryNames);

    const size_t spaceDim = 2;
    spatialdata::geocoords::CSCart cs;
    cs.setSpaceDim(spaceDim);
    const double coords[spaceDim] = { 2.3, 5.6 };
    double data[querySize];

    db.query(data, querySize, coords, &cs);

    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < querySize; ++i) {
        const double valE = values[queryVals[i]];
        const double toleranceV = fabs(valE) > 0.0 ? fabs(valE) * tolerance : tolerance;
        CHECK_THAT(data[i], Catch::Matchers::WithinAbs(valE, toleranceV));
    } // for
} // testQuery


// End of file
