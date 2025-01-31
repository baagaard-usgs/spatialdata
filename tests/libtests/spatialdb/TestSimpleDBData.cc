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

#include "spatialdata/spatialdb/SimpleDBData.hh" // Test subject

#include "spatialdata/geocoords/CSCart.hh" // USES CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ------------------------------------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class TestSimpleDBData;
    } // spatialdb
} // spatialdata

class spatialdata::spatialdb::TestSimpleDBData {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Test constructor.
    static
    void testConstructor(void);

    /// Test allocate(), getNumLocs(), numValues(), spaceDim().
    static
    void testAllocate(void);

    /// Test data() and dataDim().
    static
    void testData(void);

    /// Test coordinates()
    static
    void testCoordinates(void);

    /// Test names()
    static
    void testNames(void);

    /// Test units()
    static
    void testUnits(void);

}; // class TestSimpleDBData

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestSimpleDBData::testConstructor", "[TestSimpleDBData]") {
    spatialdata::spatialdb::TestSimpleDBData::testConstructor();
}
TEST_CASE("TestSimpleDBData::testAllocate", "[TestSimpleDBData]") {
    spatialdata::spatialdb::TestSimpleDBData::testAllocate();
}
TEST_CASE("TestSimpleDBData::testData", "[TestSimpleDBData]") {
    spatialdata::spatialdb::TestSimpleDBData::testData();
}
TEST_CASE("TestSimpleDBData::testCoordinates", "[TestSimpleDBData]") {
    spatialdata::spatialdb::TestSimpleDBData::testCoordinates();
}
TEST_CASE("TestSimpleDBData::testNames", "[TestSimpleDBData]") {
    spatialdata::spatialdb::TestSimpleDBData::testNames();
}
TEST_CASE("TestSimpleDBData::testUnits", "[TestSimpleDBData]") {
    spatialdata::spatialdb::TestSimpleDBData::testUnits();
}

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
spatialdata::spatialdb::TestSimpleDBData::testConstructor(void) {
    SimpleDBData data;
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test allocate(), getNumLocs(), numValues(), spaceDim().
void
spatialdata::spatialdb::TestSimpleDBData::testAllocate(void) {
    const size_t numLocs = 4;
    const size_t numValues = 5;
    const size_t dataDim = 1;
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());assert(cs);
    const size_t spaceDim = cs->getSpaceDim();

    SimpleDBData data;
    data.setCoordSys(cs);
    data.allocate(numLocs, numValues, spaceDim, dataDim);

    CHECK(numLocs == data.getNumLocs());
    CHECK(numValues == data.getNumValues());
    CHECK(spaceDim == data.getSpaceDim());
    CHECK(dataDim == data.getDataDim());

    assert(numLocs*numValues == data._data.size());
    assert(numLocs*spaceDim == data._coordinates.size());
    assert(numValues == data._names.size());
    assert(numValues == data._units.size());
} // testAllocate


// ------------------------------------------------------------------------------------------------
// Test data()
void
spatialdata::spatialdb::TestSimpleDBData::testData(void) {
    const size_t numLocs = 4;
    const size_t numValues = 2;
    const size_t dataDim = 1;
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());assert(cs);
    const size_t spaceDim = cs->getSpaceDim();

    const double valuesE[numLocs*numValues] = {
        0.11, 0.21,
        0.12, 0.22,
        0.13, 0.23,
        0.14, 0.24,
    };

    SimpleDBData data;
    data.setCoordSys(cs);
    data.allocate(numLocs, numValues, spaceDim, dataDim);
    for (size_t iLoc = 0, i = 0; iLoc < numLocs; ++iLoc) {
        double* const values = data.getData(iLoc);
        assert(values);
        for (size_t iVal = 0; iVal < numValues; ++iVal) {
            values[iVal] = valuesE[i++];
        } // for
    } // for

    for (size_t iLoc = 0, i = 0; iLoc < numLocs; ++iLoc) {
        const double* values = data.getData(iLoc);
        assert(values);
        for (size_t iVal = 0; iVal < numValues; ++iVal) {
            CHECK(valuesE[i++] == values[iVal]);
        } // for
    } // for
} // testData


// ------------------------------------------------------------------------------------------------
// Test coordinates()
void
spatialdata::spatialdb::TestSimpleDBData::testCoordinates(void) {
    const size_t numLocs = 4;
    const size_t numValues = 2;
    const size_t dataDim = 1;
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());assert(cs);
    const size_t spaceDim = cs->getSpaceDim();

    const double coordinatesE[numLocs*3] = {
        1.1, 2.1, 3.1,
        1.2, 2.2, 3.2,
        1.3, 2.3, 3.3,
        1.4, 2.4, 3.4,
    };

    SimpleDBData data;
    data.setCoordSys(cs);
    data.allocate(numLocs, numValues, spaceDim, dataDim);
    for (size_t iLoc = 0, i = 0; iLoc < numLocs; ++iLoc) {
        double* const coordinates = data.getCoordinates(iLoc);
        assert(coordinates);
        for (size_t iDim = 0; iDim < spaceDim; ++iDim) {
            coordinates[iDim] = coordinatesE[i++];
        } // for
    } // for

    for (size_t iLoc = 0, i = 0; iLoc < numLocs; ++iLoc) {
        const double* coordinates = data.getCoordinates(iLoc);
        assert(coordinates);
        for (size_t iDim = 0; iDim < spaceDim; ++iDim) {
            CHECK(coordinatesE[i++] == coordinates[iDim]);
        } // for
    } // for
} // testCoordinates


// ------------------------------------------------------------------------------------------------
// Test names()
void
spatialdata::spatialdb::TestSimpleDBData::testNames(void) {
    const size_t numLocs = 4;
    const size_t numValues = 2;
    const size_t dataDim = 1;
    const std::vector<std::string> names({ "one", "two" });
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());assert(cs);
    const size_t spaceDim = cs->getSpaceDim();

    SimpleDBData data;
    data.allocate(numLocs, numValues, spaceDim, dataDim);
    data.setNames(names);

    for (size_t i = 0; i < numValues; ++i) {
        CHECK(names[i] == std::string(data.getName(i)));
    } // for
} // testNames


// ------------------------------------------------------------------------------------------------
// Test units()
void
spatialdata::spatialdb::TestSimpleDBData::testUnits(void) {
    const size_t numLocs = 4;
    const size_t numValues = 2;
    const size_t dataDim = 1;
    const std::vector<std::string> units({ "m", "m/s" });
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());assert(cs);
    const size_t spaceDim = cs->getSpaceDim();

    SimpleDBData data;
    data.allocate(numLocs, numValues, spaceDim, dataDim);
    data.setUnits(units);

    for (size_t i = 0; i < numValues; ++i) {
        CHECK(units[i] == std::string(data.getUnits(i)));
    } // for
} // testUnits


// End of file
