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

#include "TestSimpleGridDB.hh" // Implementation of class methods

#include "spatialdata/spatialdb/SimpleGridDB.hh" // USES SimpleGridDB

#include "spatialdata/geocoords/CSCart.hh" // USE CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ----------------------------------------------------------------------
// Constructor.
spatialdata::spatialdb::TestSimpleGridDB::TestSimpleGridDB(TestSimpleGridDB_Data* data) :
    _testData(data) {
    REQUIRE(_testData);
    _db = std::make_unique<SimpleGridDB>(_testData->description);
} // setUp


// ----------------------------------------------------------------------
// Tear down testing data.
spatialdata::spatialdb::TestSimpleGridDB::~TestSimpleGridDB(void) {}


// ----------------------------------------------------------------------
// Test constructor
void
spatialdata::spatialdb::TestSimpleGridDB::testConstructor(void) {
    const char* description = "testConstructor";
    SimpleGridDB db(description);
} // testConstructor


// ----------------------------------------------------------------------
// Test setDescription()
void
spatialdata::spatialdb::TestSimpleGridDB::testAccessors(void) {
    const std::string& description = "testAccessors";
    const std::string& filename = "griddb.spatialdb";

    SimpleGridDB db(description.c_str());
    CHECK(description == std::string(db.getDescription()));

    db.setFilename(filename.c_str());
    CHECK(filename == db._filename);
} // testAccessors


// ----------------------------------------------------------------------
// Test getNamesDBValues().
void
spatialdata::spatialdb::TestSimpleGridDB::testGetNamesDBValues(void) {
    REQUIRE(_db);
    REQUIRE(_testData);
    _db->setFilename(_testData->filename);
    _db->open();

    const std::vector<std::string>& names = _db->getNamesDBValues();
    REQUIRE(_testData->numValues == names.size());

    const size_t numValues = names.size();
    for (size_t i = 0; i < numValues; ++i) {
        CHECK(std::string(_testData->names[i]) == names[i]);
    } // for
} // testGetDBValues


// ----------------------------------------------------------------------
// Test query() using nearest neighbor.
void
spatialdata::spatialdb::TestSimpleGridDB::testQueryNearest(void) {
    REQUIRE(_db);
    REQUIRE(_testData);
    _db->setFilename(_testData->filename);
    _db->open();

    _db->setQueryType(SimpleGridDB::NEAREST);
    _checkQuery(_testData->queryNearest, nullptr);
} // _testQueryNearest


// ----------------------------------------------------------------------
// Test query() using linear interpolation.
void
spatialdata::spatialdb::TestSimpleGridDB::testQueryLinear(void) {
    REQUIRE(_db);
    REQUIRE(_testData);
    _db->setFilename(_testData->filename);
    _db->open();

    _db->setQueryType(SimpleGridDB::LINEAR);
    _checkQuery(_testData->queryLinear, _testData->errFlags);
} // _testQueryLinear


// ----------------------------------------------------------------------
// Test query method by doing query and checking values returned.
void
spatialdata::spatialdb::TestSimpleGridDB::_checkQuery(const double* queryData,
                                                      const int* flagsE) {
    REQUIRE(queryData);
    REQUIRE(_testData);

    // Query values in reverse order for nontrivial test.
    const size_t numValues = _testData->numValues;
    std::vector<std::string> queryNames(numValues);
    for (size_t i = 0; i < numValues; ++i) {
        queryNames[numValues-i-1] = _testData->names[i];
    } // for
    _db->setQueryValues(queryNames);

    const size_t spaceDim = _testData->spaceDim;
    spatialdata::geocoords::CSCart csCart;
    csCart.setSpaceDim(spaceDim);

    const size_t numQueries = _testData->numQueries;
    const size_t locSize = spaceDim + numValues;
    std::vector<double> values(numValues);
    const double tolerance = 1.0e-06;
    for (size_t iQuery = 0; iQuery < numQueries; ++iQuery) {
        const double* coordinates = &queryData[iQuery*locSize];
        const double* valuesE = &queryData[iQuery*locSize+spaceDim];
        const int err = _db->query(values.data(), numValues, coordinates, &csCart);
        if (flagsE) {
            CHECK(flagsE[iQuery] == err);
        } else {
            CHECK(0 == err);
        } // if/else
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            const double valueE = valuesE[numValues-iValue-1];
            const double toleranceV = fabs(valueE) > 0.0 ? fabs(valueE) * tolerance : tolerance;
            CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valueE, toleranceV));
        } // for
    } // for
} // _checkQuery


// ----------------------------------------------------------------------
spatialdata::spatialdb::TestSimpleGridDB_Data::TestSimpleGridDB_Data(void) :
    numX(0),
    numY(0),
    numZ(0),
    spaceDim(0),
    numValues(0),
    dataDim(0),
    dbX(nullptr),
    dbY(nullptr),
    dbZ(nullptr),
    dbData(nullptr),
    names(nullptr),
    units(nullptr),
    description(nullptr),
    filename(nullptr),
    numQueries(0),
    queryNearest(nullptr),
    queryLinear(nullptr),
    errFlags(nullptr) {}


// ----------------------------------------------------------------------
spatialdata::spatialdb::TestSimpleGridDB_Data::~TestSimpleGridDB_Data(void) {
    numX = 0;
    numY = 0;
    numZ = 0;
    spaceDim = 0;
    numValues = 0;
    dataDim = 0;
    dbX = nullptr;
    dbY = nullptr;
    dbZ = nullptr;
    dbData = nullptr;
    names = nullptr;
    units = nullptr;
    description = nullptr;
    filename = nullptr;
    numQueries = 0;
    queryNearest = nullptr;
    queryLinear = nullptr;
    errFlags = nullptr;
} // destructor


// End of file
