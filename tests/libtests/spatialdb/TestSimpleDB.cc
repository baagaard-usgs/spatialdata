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

#include "TestSimpleDB.hh" // Implementation of class methods

#include "spatialdata/spatialdb/SimpleDB.hh" // USES SimpleDB
#include "spatialdata/spatialdb/SimpleDBData.hh" // USES SimpleDBData
#include "spatialdata/spatialdb/SimpleDBQuery.hh" // USES SimpleDBQuery
#include "spatialdata/spatialdb/SimpleDBIO.hh" // USES SimpleDBIO

#include "spatialdata/geocoords/CSCart.hh" // USE CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ------------------------------------------------------------------------------------------------
// Constructor.
spatialdata::spatialdb::TestSimpleDB::TestSimpleDB(TestSimpleDB_Data* data) :
    _db(new SimpleDB(data->description)),
    _data(data) {
    REQUIRE(_db);
    REQUIRE(_data);
} // constructor


// ------------------------------------------------------------------------------------------------
// Deallocate test data;
spatialdata::spatialdb::TestSimpleDB::~TestSimpleDB(void) {}


// ------------------------------------------------------------------------------------------------
// Test constructor
void
spatialdata::spatialdb::TestSimpleDB::testConstructors(void) {
    const std::string& description = "TestSimpleDB::testConstructors";
    SimpleDB db(description.c_str());

    CHECK(description == std::string(db.getDescription()));
} // testConstructors


// ------------------------------------------------------------------------------------------------
// Test accessors.
void
spatialdata::spatialdb::TestSimpleDB::testAccessors(void) {
    const std::string& description = "TestSimpleDB::testAccessors";
    SimpleDB db(description.c_str());

    CHECK(description == std::string(db.getDescription()));
} // testAccessors


// ------------------------------------------------------------------------------------------------
// Test getNamesDBValues().
void
spatialdata::spatialdb::TestSimpleDB::testGetNamesDBValues(void) {
    _initializeDB();
    REQUIRE(_db);
    REQUIRE(_data);

    const std::vector<std::string>& names = _db->getNamesDBValues();
    REQUIRE(_data->numValues == names.size());

    for (size_t i = 0; i < _data->numValues; ++i) {
        CHECK(std::string(_data->names[i]) == names[i]);
    } // for
} // testGetDBValues


// ------------------------------------------------------------------------------------------------
// Test query() using nearest neighbor
void
spatialdata::spatialdb::TestSimpleDB::testQueryNearest(void) {
    _initializeDB();
    REQUIRE(_db);
    REQUIRE(_data);

    _db->setQueryType(SimpleDB::NEAREST);
    _checkQuery(_data->queryNearest, nullptr);
} // testQueryNearest


// ------------------------------------------------------------------------------------------------
// Test query() using linear interpolation
void
spatialdata::spatialdb::TestSimpleDB::testQueryLinear(void) {
    _initializeDB();
    REQUIRE(_db);
    REQUIRE(_data);

    _db->setQueryType(SimpleDB::LINEAR);
    _checkQuery(_data->queryLinear, _data->errFlags);
} // _testQueryLinear


// ------------------------------------------------------------------------------------------------
// Populate database with data.
void
spatialdata::spatialdb::TestSimpleDB::_initializeDB(void) {
    REQUIRE(_data);
    REQUIRE(_db);
    REQUIRE(_db->_data);

    _db->_data->allocate(_data->numLocs, _data->numValues, _data->spaceDim, _data->dataDim);

    std::vector<std::string> names(_data->names, _data->names+_data->numValues);
    _db->_data->setNames(names);

    std::vector<std::string> units(_data->units, _data->units+_data->numValues);
    _db->_data->setUnits(units);

    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());REQUIRE(cs);
    _db->_data->setCoordSys(cs);

    _db->_query = std::make_unique<SimpleDBQuery>(*_db->_data.get(), _data->description);

    for (size_t iLoc = 0, iData = 0, iCoords = 0; iLoc < _data->numLocs; ++iLoc) {
        // data
        double* const data = _db->_data->getData(iLoc);
        for (size_t iValue = 0; iValue < _data->numValues; ++iValue) {
            data[iValue] = _data->dbValues[iData++];
        } // for

        // coordinates
        double* const coordinates = _db->_data->getCoordinates(iLoc);
        for (size_t iDim = 0; iDim < _data->spaceDim; ++iDim) {
            coordinates[iDim] = _data->dbCoordinates[iCoords++];
        } // for
    } // for
} // _setupDB


// ------------------------------------------------------------------------------------------------
// Test query method by doing query and checking values returned.
void
spatialdata::spatialdb::TestSimpleDB::_checkQuery(const double* queryData,
                                                  const int* flagsE) {
    REQUIRE(queryData);
    REQUIRE(_data);
    REQUIRE(_db);

    // Query values in reverse order for nontrivial test.
    const size_t numValues = _data->numValues;
    std::vector<std::string> queryNames(numValues);
    for (size_t i = 0; i < numValues; ++i) {
        queryNames[numValues-i-1] = _data->names[i];
    } // for
    _db->setQueryValues(queryNames);

    std::vector<double> values(numValues);
    const double tolerance = 1.0e-06;

    const size_t spaceDim = _data->spaceDim;
    const size_t numQueries = _data->numQueries;
    const size_t locSize = spaceDim + numValues;
    spatialdata::geocoords::CSCart csCart;
    for (size_t iQuery = 0; iQuery < numQueries; ++iQuery) {
        const double* coordinates = &queryData[iQuery*locSize];
        const double* valuesE = &queryData[iQuery*locSize+spaceDim];
        const int err = _db->query(values.data(), numValues, coordinates, &csCart);
        if (flagsE) {
            CHECK(flagsE[iQuery] == err);
        } else {
            CHECK(0 == err);
        } // if/else
        for (size_t iVal = 0; iVal < numValues; ++iVal) {
            const double valueE = valuesE[numValues-iVal-1];
            const double toleranceV = fabs(valueE) > 0.0 ? fabs(valueE) * tolerance : tolerance;
            CHECK_THAT(values[iVal], Catch::Matchers::WithinAbs(valueE, toleranceV));
        } // for
    } // for
} // _checkQuery


// ------------------------------------------------------------------------------------------------
// Constructor
spatialdata::spatialdb::TestSimpleDB_Data::TestSimpleDB_Data(void) :
    numLocs(0),
    spaceDim(0),
    numValues(0),
    dataDim(0),
    numQueries(0),
    description(nullptr),
    dbCoordinates(nullptr),
    dbValues(nullptr),
    names(nullptr),
    units(nullptr),
    queryNearest(nullptr),
    queryLinear(nullptr),
    errFlags(nullptr) {}


// ------------------------------------------------------------------------------------------------
spatialdata::spatialdb::TestSimpleDB_Data::~TestSimpleDB_Data(void) {
    // Set members holding static const data to NULL (no deallocation).
    description = nullptr;
    dbCoordinates = nullptr;
    dbValues = nullptr;
    names = nullptr;
    units = nullptr;
    queryNearest = nullptr;
    queryLinear = nullptr;
    errFlags = nullptr;
} // destructor


// End of file
