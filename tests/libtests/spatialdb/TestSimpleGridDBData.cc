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

#include "spatialdata/spatialdb/SimpleGridDBData.hh" // Test subject

#include "spatialdata/geocoords/CSCart.hh" // USES CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ------------------------------------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class TestSimpleGridDBData;
    } // spatialdb
} // spatialdata

class spatialdata::spatialdb::TestSimpleGridDBData {
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

}; // class TestSimpleGridDBData

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestSimpleGridDBData::testConstructor", "[TestSimpleGridDBData]") {
    spatialdata::spatialdb::TestSimpleGridDBData::testConstructor();
}
TEST_CASE("TestSimpleGridDBData::testAllocate", "[TestSimpleGridDBData]") {
    spatialdata::spatialdb::TestSimpleGridDBData::testAllocate();
}
TEST_CASE("TestSimpleGridDBData::testData", "[TestSimpleGridDBData]") {
    spatialdata::spatialdb::TestSimpleGridDBData::testData();
}
TEST_CASE("TestSimpleGridDBData::testCoordinates", "[TestSimpleGridDBData]") {
    spatialdata::spatialdb::TestSimpleGridDBData::testCoordinates();
}
TEST_CASE("TestSimpleGridDBData::testNames", "[TestSimpleGridDBData]") {
    spatialdata::spatialdb::TestSimpleGridDBData::testNames();
}
TEST_CASE("TestSimpleGridDBData::testUnits", "[TestSimpleGridDBData]") {
    spatialdata::spatialdb::TestSimpleGridDBData::testUnits();
}

// ------------------------------------------------------------------------------------------------
// Test constructor.
void
spatialdata::spatialdb::TestSimpleGridDBData::testConstructor(void) {
    SimpleGridDBData data;
} // testConstructor


// ------------------------------------------------------------------------------------------------
// Test allocate(), getNumLocs(), numValues(), spaceDim().
void
spatialdata::spatialdb::TestSimpleGridDBData::testAllocate(void) {
    const size_t numX = 2;
    const size_t numY = 4;
    const size_t numZ = 5;
    const size_t numValues = 5;
    const size_t dataDim = 1;
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());REQUIRE(cs);
    const size_t spaceDim = cs->getSpaceDim();

    SimpleGridDBData data;
    data.setCoordSys(cs);
    data.allocate(numX, numY, numZ, numValues, spaceDim, dataDim);

    CHECK(numX == data.getNumX());
    CHECK(numY == data.getNumY());
    CHECK(numZ == data.getNumZ());
    CHECK(numValues == data.getNumValues());
    CHECK(spaceDim == data.getSpaceDim());
    CHECK(dataDim == data.getDataDim());

    CHECK(numX*numY*numZ*numValues == data._data.size());
    CHECK(numX == data._x.size());
    CHECK(numY == data._y.size());
    CHECK(numZ == data._z.size());
    CHECK(0 == data._names.size());
    CHECK(0 == data._units.size());
} // testAllocate


// ------------------------------------------------------------------------------------------------
// Test coordinates()
void
spatialdata::spatialdb::TestSimpleGridDBData::testCoordinates(void) {
    const size_t numX = 2;
    const size_t numY = 3;
    const size_t numZ = 4;
    const size_t numValues = 2;
    const size_t dataDim = 2;
    const size_t spaceDim = 3;
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());REQUIRE(cs);
    cs->setSpaceDim(spaceDim);

    const double xE[numX] = { 1.0, 2.0, };
    const double yE[numY] = { 0.1, 0.2, 0.3, };
    const double zE[numZ] = { 10., 11.0, 12.0, 13.0, };

    SimpleGridDBData data;
    data.setCoordSys(cs);
    data.allocate(numX, numY, numZ, numValues, spaceDim, dataDim);
    std::copy(xE, xE+numX, data.getX().data());
    std::copy(yE, yE+numY, data.getY().data());
    std::copy(zE, zE+numZ, data.getZ().data());

    // Check values
    const std::vector<double> x = data.getX();
    for (size_t iX = 0; iX < numX; ++iX) {
        CHECK(xE[iX] == x[iX]);
    } // for

    const std::vector<double> y = data.getY();
    for (size_t iY = 0; iY < numY; ++iY) {
        CHECK(yE[iY] == y[iY]);
    } // for

    const std::vector<double> z = data.getZ();
    for (size_t iZ = 0; iZ < numZ; ++iZ) {
        CHECK(zE[iZ] == z[iZ]);
    } // for
} // testCoordinates


// ------------------------------------------------------------------------------------------------
// Test data()
void
spatialdata::spatialdb::TestSimpleGridDBData::testData(void) {
    const size_t numX = 2;
    const size_t numY = 3;
    const size_t numZ = 0;
    const size_t numValues = 2;
    const size_t dataDim = 2;
    const size_t spaceDim = 2;
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());REQUIRE(cs);
    cs->setSpaceDim(spaceDim);

    const double valuesE[numX*numY*numValues] = {
        0.11, 0.21,
        0.12, 0.22,
        0.13, 0.23,
        0.14, 0.24,
        0.15, 0.25,
        0.16, 0.36,
    };

    SimpleGridDBData data;
    data.setCoordSys(cs);
    data.allocate(numX, numY, numZ, numValues, spaceDim, dataDim);

    for (size_t iLoc = 0, iData = 0; iLoc < numX*numY; ++iLoc) {
        double* const values = data.getData(iLoc*numValues);REQUIRE(values);
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            values[iValue] = valuesE[iData++];
        } // for
    } // for

    // Check values
    for (size_t iLoc = 0, i = 0; iLoc < numX*numY; ++iLoc) {
        const double* values = data.getData(iLoc*numValues);
        REQUIRE(values);
        for (size_t iValue = 0; iValue < numValues; ++iValue) {
            CHECK(valuesE[i++] == values[iValue]);
        } // for
    } // for
} // testData


// ------------------------------------------------------------------------------------------------
// Test names()
void
spatialdata::spatialdb::TestSimpleGridDBData::testNames(void) {
    const size_t numX = 4;
    const size_t numY = 2;
    const size_t numZ = 2;
    const size_t numValues = 2;
    const size_t dataDim = 1;
    const std::vector<std::string> names({ "one", "two" });
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());REQUIRE(cs);
    const size_t spaceDim = cs->getSpaceDim();

    SimpleGridDBData data;
    data.allocate(numX, numY, numZ, numValues, spaceDim, dataDim);
    data.setNames(names);

    for (size_t i = 0; i < numValues; ++i) {
        CHECK(names[i] == std::string(data.getName(i)));
    } // for
} // testNames


// ------------------------------------------------------------------------------------------------
// Test units()
void
spatialdata::spatialdb::TestSimpleGridDBData::testUnits(void) {
    const size_t numX = 4;
    const size_t numY = 1;
    const size_t numZ = 2;
    const size_t numValues = 2;
    const size_t dataDim = 1;
    const std::vector<std::string> units({ "m", "m/s" });
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());REQUIRE(cs);
    const size_t spaceDim = cs->getSpaceDim();

    SimpleGridDBData data;
    data.allocate(numX, numY, numZ, numValues, spaceDim, dataDim);
    data.setUnits(units);

    for (size_t i = 0; i < numValues; ++i) {
        CHECK(units[i] == std::string(data.getUnits(i)));
    } // for
} // testUnits


// End of file
