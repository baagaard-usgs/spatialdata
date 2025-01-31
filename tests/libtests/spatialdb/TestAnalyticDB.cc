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

#include "spatialdata/spatialdb/AnalyticDB.hh" // Test subject

#include "spatialdata/geocoords/CSCart.hh" // USES CSCart
#include "spatialdata/geocoords/CSGeo.hh" // USES CSGeo

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ------------------------------------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class TestAnalyticDB;
    }
} // spatialdata

class spatialdata::spatialdb::TestAnalyticDB {
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

    /// Test query() WGS84 -> UTM.
    static
    void testQueryUTM(void);

}; // class TestAnalyticDB

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestAnalyticDB::testConstructors", "[TestAnalyticDB]") {
    spatialdata::spatialdb::TestAnalyticDB::testConstructors();
}
TEST_CASE("TestAnalyticDB::testAccessors", "[TestAnalyticDB]") {
    spatialdata::spatialdb::TestAnalyticDB::testAccessors();
}
TEST_CASE("TestAnalyticDB::testSetData", "[TestAnalyticDB]") {
    spatialdata::spatialdb::TestAnalyticDB::testSetData();
}
TEST_CASE("TestAnalyticDB::testGetNamesDBValues", "[TestAnalyticDB]") {
    spatialdata::spatialdb::TestAnalyticDB::testGetNamesDBValues();
}
TEST_CASE("TestAnalyticDB::testQueryValues", "[TestAnalyticDB]") {
    spatialdata::spatialdb::TestAnalyticDB::testQueryValues();
}
TEST_CASE("TestAnalyticDB::testQuery", "[TestAnalyticDB]") {
    spatialdata::spatialdb::TestAnalyticDB::testQuery();
}
TEST_CASE("TestAnalyticDB::testQueryUTM", "[TestAnalyticDB]") {
    spatialdata::spatialdb::TestAnalyticDB::testQueryUTM();
}

// ----------------------------------------------------------------------
// Test constructor.
void
spatialdata::spatialdb::TestAnalyticDB::testConstructors(void) {
    const std::string& description = "TestAnalyticDB::testConstructors";

    AnalyticDB db(description.c_str());
    CHECK(description == std::string(db.getDescription()));
} // testConstructors


// ----------------------------------------------------------------------
// Test accessors().
void
spatialdata::spatialdb::TestAnalyticDB::testAccessors(void) {
    const std::string& description = "TestAnalyticDB::testAccessors";

    AnalyticDB db(description.c_str());
    CHECK(description == std::string(db.getDescription()));
} // testAccessors


// ----------------------------------------------------------------------
// Test setData().
void
spatialdata::spatialdb::TestAnalyticDB::testSetData(void) {
    AnalyticDB db("TestAnalyticDB::testSetData");

    const size_t numValuesE = 3;
    const std::vector<std::string> names({ "one", "two", "three" });
    const std::vector<std::string> units({ "m", "km", "cm" });
    const std::vector<std::string> expressions({ "x^2 + y^2", "x/z", "x + y + z" });

    db.setData(names, units, expressions);

    REQUIRE(numValuesE == db._names.size());
    for (size_t i = 0; i < numValuesE; ++i) {
        CHECK(std::string(names[i]) == db._names[i]);
    } // for

    REQUIRE(numValuesE == db._scales.size());
    const std::vector<double> scalesE({1.0, 1000.0, 0.01});
    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < numValuesE; ++i) {
        CHECK_THAT(scalesE[i]/db._scales[i], Catch::Matchers::WithinAbs(1.0, tolerance));
    } // for

    REQUIRE(numValuesE == db._expressions.size());
    for (size_t i = 0; i < numValuesE; ++i) {
        CHECK(std::string(expressions[i]) == db._expressions[i]);
    } // for

    CHECK(numValuesE == db._queryIndices.size());
} // testSetData


// ----------------------------------------------------------------------
// Test getNamesDBValues().
void
spatialdata::spatialdb::TestAnalyticDB::testGetNamesDBValues(void) {
    const size_t numValuesE = 3;
    const std::vector<std::string> namesE({ "one", "two", "three" });
    const std::vector<std::string> units({ "none", "none", "none" });
    const std::vector<std::string> expressions({ "x^2 + y^2", "x/z", "x + y + z" });

    AnalyticDB db("TestAnalyticDB::testGetNamesDBValues");
    db.setData(namesE, units, expressions);

    const std::vector<std::string>& names = db.getNamesDBValues();
    REQUIRE(numValuesE == names.size());

    for (size_t i = 0; i < numValuesE; ++i) {
        CHECK(std::string(namesE[i]) == std::string(names[i]));
    } // for
} // testGetNamesDBValues


// ----------------------------------------------------------------------
// Test setQueryValues().
void
spatialdata::spatialdb::TestAnalyticDB::testQueryValues(void) {
    AnalyticDB db("TestAnalyticDB::testQueryValues");

    const std::vector<std::string> names({ "one", "two", "three" });
    const std::vector<std::string> units({ "none", "none", "none" });
    const std::vector<std::string> expressions({ "x^2 + y^2", "x/z", "x + y + z" });

    const std::vector<std::string> queryNames({ "three", "two" });
    const size_t querySize = 2;
    const size_t queryVals[querySize] = { 2, 1 };

    db.setData(names, units, expressions);
    db.setQueryValues(queryNames);

    REQUIRE(querySize == db._queryIndices.size());
    for (size_t i = 0; i < querySize; ++i) {
        CHECK(queryVals[i] == db._queryIndices[i]);
    } // for
} // testQueryValues


// ----------------------------------------------------------------------
// Test query().
void
spatialdata::spatialdb::TestAnalyticDB::testQuery(void) {
    AnalyticDB db("TestAnalyticDB::testQuery");

    const size_t spaceDim = 3;
    spatialdata::geocoords::CSCart cs;
    cs.setSpaceDim(spaceDim);
    const double coords[spaceDim] = { 1.1, 2.3, 5.6 };

    const size_t numValues = 3;
    const std::vector<std::string> names({ "one", "two", "three" });
    const std::vector<std::string> units({ "none", "km", "cm" });
    const std::vector<std::string> expressions({ "x^2 + y^2", "x/z", "x + y + z" });
    const double scales[numValues] = { 1.0, 1000.0, 0.01 };
    const double values[numValues] = {
        coords[0]*coords[0],
        coords[0]/coords[2],
        coords[0]+coords[1]+coords[2],
    };

    const std::vector<std::string> queryNames({ "three", "two" });
    const size_t querySize = 2;
    const size_t queryVals[querySize] = { 2, 1 };

    db.setData(names, units, expressions);
    db.setQueryValues(queryNames);

    double data[querySize];
    db.query(data, querySize, coords, &cs);

    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < querySize; ++i) {
        const size_t index = queryVals[i];
        const double valE = scales[index] * values[index];
        const double toleranceV = fabs(valE) > 0.0 ? fabs(valE) * tolerance : tolerance;
        CHECK_THAT(data[i], Catch::Matchers::WithinAbs(valE, toleranceV));
    } // for
} // testQuery


// ----------------------------------------------------------------------
// Test query() with WGS84 -> UTM.
void
spatialdata::spatialdb::TestAnalyticDB::testQueryUTM(void) {
    AnalyticDB db("TestAnalyticDB::testQueryUTM");
    const size_t spaceDim = 3;

    spatialdata::geocoords::CSGeo csWGS84;
    csWGS84.setString("EPSG:4326");
    csWGS84.setSpaceDim(spaceDim);
    const double coordsLL[spaceDim] = { 37.50, -122.30, 5.6 };

    std::shared_ptr<spatialdata::geocoords::CSGeo> csUTM(new spatialdata::geocoords::CSGeo());assert(csUTM);
    csUTM->setString("EPSG:32610");
    csUTM->setSpaceDim(spaceDim);
    const double coordsUTM[spaceDim] = { 561873.454241, 4150571.437855, 5.6 };

    const size_t numValues = 3;
    const std::vector<std::string> names({ "one", "two", "three" });
    const std::vector<std::string> units({ "none", "km", "cm" });
    const std::vector<std::string> expressions({ "x^2 + y^2", "x/z", "x + y + z" });
    const double scales[numValues] = { 1.0, 1000.0, 0.01 };
    const double values[numValues] = {
        coordsUTM[0]*coordsUTM[0],
        coordsUTM[0]/coordsUTM[2],
        coordsUTM[0]+coordsUTM[1]+coordsUTM[2],
    };

    const size_t querySize = 2;
    const std::vector<std::string> queryNames({ "three", "two" });
    const size_t queryVals[querySize] = { 2, 1 };

    db.setCoordSys(csUTM);
    db.setData(names, units, expressions);
    db.setQueryValues(queryNames);

    double data[querySize];
    db.query(data, querySize, coordsLL, &csWGS84);

    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < querySize; ++i) {
        const size_t index = queryVals[i];
        const double valE = scales[index] * values[index];
        const double toleranceV = fabs(valE) > 0.0 ? fabs(valE) * tolerance : tolerance;
        CHECK_THAT(data[i], Catch::Matchers::WithinAbs(valE, toleranceV));
    } // for
} // testQueryUTM


// End of file
