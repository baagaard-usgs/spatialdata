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

#include "TestCxxFunctionDB.hh" // Implementation of class methods

#include "spatialdata/spatialdb/CxxFunctionDB.hh" // USES CxxFunctionDB
#include "spatialdata/geocoords/CSCart.hh" // USE CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()
#include <stdexcept> // USES std::runtime_error

namespace spatialdata {
    namespace spatialdb {
        namespace _TestCxxFunctionDB {
            double
            fn1D(double x) {
                return x * x;
            }


        }
    }
}

// ----------------------------------------------------------------------
// Constructor
spatialdata::spatialdb::TestCxxFunctionDB::TestCxxFunctionDB(TestCxxFunctionDB_Data* data,
                                                             CxxFunctionDB* db) :
    _db(db),
    _data(data) {
    assert(_data);
    assert(_db);

    _db->setCoordSys(_data->cs);
} // setUp


// ----------------------------------------------------------------------
// Tear down testing data.
spatialdata::spatialdb::TestCxxFunctionDB::~TestCxxFunctionDB(void) {}


// ----------------------------------------------------------------------
// Test constructor
void
spatialdata::spatialdb::TestCxxFunctionDB::testConstructor(void) {
    CxxFunctionDB db("TestCxxFunctionDB::testConstructor");
} // testConstructor


// ----------------------------------------------------------------------
// Test label()
void
spatialdata::spatialdb::TestCxxFunctionDB::testDescription(void) {
    const std::string& description = "TestCxxFunctionDB::testDescription";
    CxxFunctionDB db(description.c_str());

    CHECK(description == std::string(db.getDescription()));
} // testDescription


// ----------------------------------------------------------------------
// Test coordsys()
void
spatialdata::spatialdb::TestCxxFunctionDB::testCoordsys(void) {
    CxxFunctionDB db("TestCxxFunctionDB::testCoordsys");

    std::shared_ptr<spatialdata::geocoords::CSCart> cs(new spatialdata::geocoords::CSCart);
    const size_t spaceDim = 2;
    cs->setSpaceDim(spaceDim);
    db.setCoordSys(cs);
    CHECK(spaceDim == db._cs->getSpaceDim());
} // testCoordsys


// ----------------------------------------------------------------------
// Test addValue()
void
spatialdata::spatialdb::TestCxxFunctionDB::testAddValue(void) {
    const size_t numValues = _data->numValues;
    assert(numValues == _db->_functions.size());
    for (size_t i = 0; i < numValues; ++i) {
        assert(_db->_functions[i].fn);
        CHECK(_data->values[i].units == _db->_functions[i].units);
    } // for

    // Test duplicate add
    CxxFunctionDB db("TestCxxFunctionDB::testAddValue");
    db.addValue("one", _TestCxxFunctionDB::fn1D, "m");
    REQUIRE_THROWS_AS(db.addValue("two", _TestCxxFunctionDB::fn1D, nullptr), std::logic_error);
    REQUIRE_THROWS_AS(db.addValue("one", _TestCxxFunctionDB::fn1D, "m"), std::logic_error);
} // testAddValue


// ----------------------------------------------------------------------
// Test open() and close()
void
spatialdata::spatialdb::TestCxxFunctionDB::testOpenClose(void) {
    assert(_data);
    assert(_db);

    // Test open() and close() with valid data.
    _db->open();

    // Verify scales
    const size_t numValues = _data->numValues;
    const double tolerance = 1.0e-6;
    for (size_t i = 0; i < numValues; ++i) {
        CHECK_THAT(_db->_functions[i].scale, Catch::Matchers::WithinAbs(_data->values[i].scale, tolerance));
    } // for

    REQUIRE(numValues == _db->_queryIndices.size());

    _db->close();
    assert(0 == _db->_queryIndices.size());

    // Verify open() fails with spatial dimension mismatch.
    const int spaceDim = _data->cs->getSpaceDim();
    const int spaceDimBad = (spaceDim == 3) ? 2 : 3;
    _db->_cs->setSpaceDim(spaceDimBad);
    CHECK_THROWS_AS(_db->open(), std::runtime_error);
    _db->_cs->setSpaceDim(spaceDim); // Reset space dimension.

    // Verify open() fails with bad units.
    _db->_functions[0].units = "abcd";
    CHECK_THROWS_AS(_db->open(), std::runtime_error);
} // testOpenClose


// ----------------------------------------------------------------------
// Test getNamesDBValues().
void
spatialdata::spatialdb::TestCxxFunctionDB::testGetNamesDBValues(void) {
    assert(_data);
    assert(_db);

    const std::vector<std::string>& names = _db->getNamesDBValues();
    const size_t numNames = names.size();
    REQUIRE(_data->numValues == numNames);

    for (size_t i = 0; i < numNames; ++i) {
        bool found = false;
        for (size_t iE = 0; iE < numNames; ++iE) {
            if (_data->values[iE].name == std::string(names[i])) {
                found = true;
                break;
            } // if
        } // for
        if (!found) {
            FAIL("Could not find value '" << names[i] << "' in CxxFunctionDB test data.");
        } // if
    } // for
} // testGetDBValues


// ----------------------------------------------------------------------
// Test setQueryValues().
void
spatialdata::spatialdb::TestCxxFunctionDB::testQueryValues(void) {
    assert(_data);
    assert(_db);

    _db->open();

    // Call setQueryValues().
    const size_t querySize = _data->numValues - 1;
    std::vector<std::string> names(querySize);
    for (int i = 0; i < querySize; ++i) {
        names[i] = _data->values[querySize-1-i].name;
    } // for
    _db->setQueryValues(names);

    // Check result.
    CHECK(querySize == _db->_queryIndices.size());
    for (int i = 0; i < querySize; ++i) {
        const int j = querySize - 1 - i;
        CHECK(j == _db->_queryIndices[i]);
    } // for

    // Attempt to create query with no values.
    std::vector<std::string> empty;
    CHECK_THROWS_AS(_db->setQueryValues(empty), std::invalid_argument);

    // Attempt to create query with value not in database (verify failure).
    std::vector<std::string> badname({"lkdfjglkdfjgljsdf"});
    CHECK_THROWS_AS(_db->setQueryValues(badname), std::out_of_range);

    _db->close();
} // testQueryValues


// ----------------------------------------------------------------------
// Test query()
void
spatialdata::spatialdb::TestCxxFunctionDB::testQuery(void) {
    assert(_data);

    assert(_data->cs);
    const int spaceDim = _data->cs->getSpaceDim();
    const size_t numValues = _data->numValues;
    const size_t numQueries = _data->numQueryPoints;
    double* values = (numValues > 0) ? new double[numValues] : NULL;

    _db->open();

    // Call setQueryValues().
    const size_t querySize = numValues;
    std::vector<std::string> names(querySize);
    for (int i = 0; i < querySize; ++i) {
        names[i] = _data->values[i].name;
    } // for
    _db->setQueryValues(names);

    // Call query() and check result.
    const double tolerance = 1.0e-6;
    for (size_t iQuery = 0; iQuery < numQueries; ++iQuery) {
        const int flag = _db->query(values, numValues, &_data->queryXYZ[iQuery*spaceDim], _data->cs.get());
        CHECK(0 == flag);

        for (size_t iVal = 0; iVal < numValues; ++iVal) {
            const double valueE = _data->queryValues[iQuery*numValues+iVal]*_data->values[iVal].scale;
            CHECK_THAT(values[iVal], Catch::Matchers::WithinAbs(valueE, tolerance));
        } // for
    } // for
    delete[] values;values = NULL;

    _db->close();

} // testQuery


// ----------------------------------------------------------------------
// Constructor
spatialdata::spatialdb::TestCxxFunctionDB_Data::TestCxxFunctionDB_Data(void) :
    numValues(0),
    values(nullptr),
    queryXYZ(nullptr),
    queryValues(nullptr),
    numQueryPoints(0) {}


// ----------------------------------------------------------------------
// Destructor
spatialdata::spatialdb::TestCxxFunctionDB_Data::~TestCxxFunctionDB_Data(void) {}


// End of file
