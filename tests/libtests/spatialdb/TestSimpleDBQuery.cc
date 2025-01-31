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

#include "TestSimpleDBQuery.hh" // Implementation of class methods

#include "spatialdata/spatialdb/SimpleDBQuery.hh" // USES SimpleDBQuery
#include "spatialdata/spatialdb/SimpleDBData.hh" // USES SimpleDBData

#include "spatialdata/geocoords/CSCart.hh" // USE CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ----------------------------------------------------------------------
// Constructor.
spatialdata::spatialdb::TestSimpleDBQuery::TestSimpleDBQuery(TestSimpleDBQuery_Data* data) :
    _data(new SimpleDBData),
    _testData(data) {
    REQUIRE(_data);
    REQUIRE(_testData);
    REQUIRE(_testData->description);
    _query = std::make_unique<SimpleDBQuery>(*_data.get(), _testData->description);REQUIRE(_query);
} // constructor


// ----------------------------------------------------------------------
// Destructor.
spatialdata::spatialdb::TestSimpleDBQuery::~TestSimpleDBQuery(void) {}


// ----------------------------------------------------------------------
// Test Constructor()
void
spatialdata::spatialdb::TestSimpleDBQuery::testConstructor(void) {
    REQUIRE(_query);
    CHECK(std::string(_testData->description) == _query->_description);
} // testConstructor


// ----------------------------------------------------------------------
// Test accessors.
void
spatialdata::spatialdb::TestSimpleDBQuery::testAccessors(void) {
    REQUIRE(_query);

    const SimpleDB::QueryEnum queryTypeDefault = SimpleDB::NEAREST;
    _query->setQueryType(queryTypeDefault);
    CHECK(queryTypeDefault == _query->_queryType);

    const SimpleDB::QueryEnum queryTypeUser = SimpleDB::LINEAR;
    _query->setQueryType(queryTypeUser);
    CHECK(queryTypeUser == _query->_queryType);
} // testAccessors


// ----------------------------------------------------------------------
// Test setQueryValues()
void
spatialdata::spatialdb::TestSimpleDBQuery::testQueryVals(void) {
    _initializeDB();

    std::vector<std::string> names({ "two", "one", "three" });
    const size_t querySize = 3;
    const size_t queryIndices[3] = { 1, 0, 2 };

    REQUIRE(_query);
    _query->setQueryValues(names);
    REQUIRE(querySize == _query->_queryIndices.size());
    for (size_t i = 0; i < querySize; ++i) {
        CHECK(queryIndices[i] == _query->_queryIndices[i]);
    } // for
} // testQueryVals


// ----------------------------------------------------------------------
// Test distSquared()
void
spatialdata::spatialdb::TestSimpleDBQuery::testDistSquared(void) {
    _initializeDB();
    REQUIRE(_testData);

    REQUIRE(_testData->numPoints >= 2);
    const size_t spaceDim = _testData->spaceDim;
    const double dist2 = SimpleDBQuery::_distSquared(&_testData->coordinates[0*spaceDim], &_testData->coordinates[1*spaceDim]);
    const double tolerance = 1.0e-06;
    const double toleranceV = fabs(_testData->dist2) > 0.0 ? fabs(_testData->dist2) * tolerance : tolerance;
    CHECK_THAT(dist2, Catch::Matchers::WithinAbs(_testData->dist2, toleranceV));
} // _testDistSquared


// ----------------------------------------------------------------------
// Test area()
void
spatialdata::spatialdb::TestSimpleDBQuery::testArea(void) {
    _initializeDB();
    REQUIRE(_testData);

    const size_t spaceDim = _testData->spaceDim;
    if (spaceDim < 2) { return; }

    REQUIRE(_testData->numPoints >= 3);
    double area = 0;
    double areaDir[3];
    SimpleDBQuery::_area(&area, areaDir,
                         &_testData->coordinates[0*spaceDim],
                         &_testData->coordinates[1*spaceDim],
                         &_testData->coordinates[2*spaceDim]);
    const double tolerance = 1.0e-06;
    double toleranceV = fabs(_testData->area) > 0.0 ? fabs(_testData->area) * tolerance : tolerance;
    CHECK_THAT(area, Catch::Matchers::WithinAbs(_testData->area, toleranceV));
    for (size_t i = 0; i < spaceDim; ++i) {
        toleranceV = fabs(_testData->areaDir[i]) > 0.0 ? fabs(_testData->areaDir[i]) * tolerance : tolerance;
        CHECK_THAT(areaDir[i], Catch::Matchers::WithinAbs(_testData->areaDir[i], toleranceV));
    } // for
} // _testArea


// ----------------------------------------------------------------------
// Test volume()
void
spatialdata::spatialdb::TestSimpleDBQuery::testVolume(void) {
    _initializeDB();
    REQUIRE(_testData);

    const size_t spaceDim = _testData->spaceDim;
    if (spaceDim < 3) { return; }

    REQUIRE(_testData->numPoints >= 4);
    const double volume =
        SimpleDBQuery::_volume(&_testData->coordinates[0*spaceDim],
                               &_testData->coordinates[1*spaceDim],
                               &_testData->coordinates[2*spaceDim],
                               &_testData->coordinates[3*spaceDim]);
    const double tolerance = 1.0e-06;
    const double toleranceV = fabs(_testData->volume) > 0.0 ? fabs(_testData->volume) * tolerance : tolerance;
    CHECK_THAT(volume, Catch::Matchers::WithinAbs(_testData->volume, toleranceV));
} // testVolume


// ----------------------------------------------------------------------
// Populate database with data.
void
spatialdata::spatialdb::TestSimpleDBQuery::_initializeDB(void) {
    REQUIRE(_testData);
    REQUIRE(_data);

    _data->allocate(_testData->numLocs, _testData->numValues, _testData->spaceDim, _testData->dataDim);

    std::vector<std::string> names(_testData->names, _testData->names+_testData->numValues);
    _data->setNames(names);

    std::vector<std::string> units(_testData->units, _testData->units+_testData->numValues);
    _data->setUnits(units);

    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());REQUIRE(cs);
    _data->setCoordSys(cs);

    for (size_t iLoc = 0, iData = 0, iCoords = 0; iLoc < _testData->numLocs; ++iLoc) {
        // data
        double* const data = _data->getData(iLoc);
        for (size_t iValue = 0; iValue < _testData->numValues; ++iValue) {
            data[iValue] = _testData->dbValues[iData++];
        } // for

        // coordinates
        double* const coordinates = _data->getCoordinates(iLoc);
        for (size_t iDim = 0; iDim < _testData->spaceDim; ++iDim) {
            coordinates[iDim] = _testData->dbCoordinates[iCoords++];
        } // for
    } // for
} // _initializeDB


// ----------------------------------------------------------------------
// Constructor.
spatialdata::spatialdb::TestSimpleDBQuery_Data::TestSimpleDBQuery_Data(void) :
    numLocs(0),
    spaceDim(0),
    numValues(0),
    dataDim(0),
    dbCoordinates(nullptr),
    dbValues(nullptr),
    names(nullptr),
    units(nullptr),
    description(nullptr),
    numPoints(0),
    coordinates(nullptr),
    dist2(0.0),
    areaDir(nullptr),
    volume(0.0) {}


// ----------------------------------------------------------------------
// Constructor.
spatialdata::spatialdb::TestSimpleDBQuery_Data::~TestSimpleDBQuery_Data(void) {
    // Set static const data in derived classes to NULL (don't deallocate).
    dbCoordinates = nullptr;
    dbValues = nullptr;
    names = nullptr;
    units = nullptr;
    description = nullptr;
    coordinates = nullptr;
    areaDir = nullptr;
} // destructor


// End of file
