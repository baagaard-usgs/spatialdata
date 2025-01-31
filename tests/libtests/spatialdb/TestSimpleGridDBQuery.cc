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

#include "TestSimpleGridDBQuery.hh" // Implementation of class methods

#include "spatialdata/spatialdb/SimpleGridDBQuery.hh" // USES SimpleGridDBQuery
#include "spatialdata/spatialdb/SimpleGridDBData.hh" // USES SimpleGridDBData
#include "spatialdata/spatialdb/Exception.hh" // USES OutOfBounds

#include "spatialdata/geocoords/CSCart.hh" // USE CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ----------------------------------------------------------------------
// Constructor.
spatialdata::spatialdb::TestSimpleGridDBQuery::TestSimpleGridDBQuery(TestSimpleGridDBQuery_Data* data) :
    _data(new SimpleGridDBData),
    _testData(data) {
    REQUIRE(_data);
    REQUIRE(_testData);
    REQUIRE(_testData->description);
    _query = std::make_unique<SimpleGridDBQuery>(*_data.get(), _testData->description);REQUIRE(_query);
} // setUp


// ----------------------------------------------------------------------
// Tear down testing data.
spatialdata::spatialdb::TestSimpleGridDBQuery::~TestSimpleGridDBQuery(void) {}


// ----------------------------------------------------------------------
// Test constructor
void
spatialdata::spatialdb::TestSimpleGridDBQuery::testConstructor(void) {
    const std::string& description = "ABC";
    SimpleGridDBData data;
    SimpleGridDBQuery query(data, description.c_str());

    CHECK(description == query._description);
    CHECK(query._converter);
} // testConstructor


// ----------------------------------------------------------------------
// Test setDescription(), setQueryType()
void
spatialdata::spatialdb::TestSimpleGridDBQuery::testAccessors(void) {
    const std::string& description = "ABC";
    SimpleGridDBData data;
    SimpleGridDBQuery query(data, description.c_str());

    CHECK(description == query._description);
    CHECK(query._converter);
    const SimpleGridDB::QueryEnum queryTypeDefault = SimpleGridDB::NEAREST;
    query.setQueryType(queryTypeDefault);
    CHECK(queryTypeDefault == query._queryType);

    const SimpleGridDB::QueryEnum queryTypeUser = SimpleGridDB::LINEAR;
    query.setQueryType(queryTypeUser);
    CHECK(queryTypeUser == query._queryType);
} // testAccessors


// ----------------------------------------------------------------------
// Test setQueryValues()
void
spatialdata::spatialdb::TestSimpleGridDBQuery::testQueryValues(void) {
    _initializeDB();
    REQUIRE(_query);

    std::vector<std::string> names({ "two", "one", });
    const size_t querySize = 2;
    const size_t queryIndices[querySize] = { 1, 0, };

    REQUIRE(_query);
    _query->setQueryValues(names);
    REQUIRE(querySize == _query->_queryIndices.size());
    for (size_t i = 0; i < querySize; ++i) {
        CHECK(queryIndices[i] == _query->_queryIndices[i]);
    } // for
} // testQueryValues


// ----------------------------------------------------------------------
// Test query() using nearest neighbor.
void
spatialdata::spatialdb::TestSimpleGridDBQuery::testQueryNearest(void) {
    _initializeDB();
    REQUIRE(_query);
    REQUIRE(_testData);

    _query->setQueryType(SimpleGridDB::NEAREST);
    _checkQuery(_testData->queryNearest, NULL);
} // _testQueryNearest


// ----------------------------------------------------------------------
// Test query() using linear interpolation.
void
spatialdata::spatialdb::TestSimpleGridDBQuery::testQueryLinear(void) {
    _initializeDB();
    REQUIRE(_query);
    REQUIRE(_testData);

    _query->setQueryType(SimpleGridDB::LINEAR);
    _checkQuery(_testData->queryLinear, _testData->errFlags);
} // _testQueryLinear


// ----------------------------------------------------------------------
// Populate database with data.
void
spatialdata::spatialdb::TestSimpleGridDBQuery::_initializeDB(void) {
    REQUIRE(_testData);
    REQUIRE(_data);

    _data->allocate(_testData->numX, _testData->numY, _testData->numZ, _testData->numValues, _testData->spaceDim, _testData->dataDim);

    std::vector<std::string> names(_testData->names, _testData->names+_testData->numValues);
    _data->setNames(names);

    std::vector<std::string> units(_testData->units, _testData->units+_testData->numValues);
    _data->setUnits(units);

    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());REQUIRE(cs);
    cs->setSpaceDim(_testData->spaceDim);
    _data->setCoordSys(cs);

    std::copy(_testData->dbX, _testData->dbX+_testData->numX, _data->getX().data());
    std::copy(_testData->dbY, _testData->dbY+_testData->numY, _data->getY().data());
    std::copy(_testData->dbZ, _testData->dbZ+_testData->numZ, _data->getZ().data());

    const size_t numLocs =
        (_testData->numX > 0 ? _testData->numX : 1) *
        (_testData->numY > 0 ? _testData->numY : 1) *
        (_testData->numZ > 0 ? _testData->numZ : 1);
    const size_t numValues = _testData->numValues;
    for (size_t iLoc = 0, iData = 0; iLoc < numLocs; ++iLoc) {
        double* const data = _data->getData(iLoc*numValues);
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            data[iValue] = _testData->dbData[iData++];
        } // for
    } // for
} // _initializeDB


// ----------------------------------------------------------------------
// Test query method by doing query and checking values returned.
void
spatialdata::spatialdb::TestSimpleGridDBQuery::_checkQuery(const double* queryData,
                                                           const int* flagsE) {
    REQUIRE(queryData);
    REQUIRE(_testData);
    REQUIRE(_query);

    // Query values in reverse order for nontrivial test.
    const size_t numValues = _testData->numValues;
    std::vector<std::string> queryNames(numValues);
    for (size_t i = 0; i < numValues; ++i) {
        queryNames[numValues-i-1] = _testData->names[i];
    } // for
    _query->setQueryValues(queryNames);

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
        try {
            _query->query(values.data(), numValues, coordinates, &csCart);
        } catch (const OutOfBounds& err) {
            CHECK(flagsE[iQuery] == 1);
            continue;
        } catch (...) {
            throw;
        } // try/catch
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            const double valueE = valuesE[numValues-iValue-1];
            const double toleranceV = fabs(valueE) > 0.0 ? fabs(valueE) * tolerance : tolerance;
            CHECK_THAT(values[iValue], Catch::Matchers::WithinAbs(valueE, toleranceV));
        } // for
    } // for

} // _checkQuery


// ----------------------------------------------------------------------
spatialdata::spatialdb::TestSimpleGridDBQuery_Data::TestSimpleGridDBQuery_Data(void) :
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
    numQueries(0),
    queryNearest(NULL),
    queryLinear(NULL),
    errFlags(NULL) {}


// ----------------------------------------------------------------------
spatialdata::spatialdb::TestSimpleGridDBQuery_Data::~TestSimpleGridDBQuery_Data(void) {
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
    numQueries = 0;
    queryNearest = nullptr;
    queryLinear = nullptr;
    errFlags = nullptr;
} // destructor


// End of file
