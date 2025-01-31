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

#include "spatialdata/spatialdb/SimpleDBIO.hh" // Test subject

#include "spatialdata/spatialdb/SimpleDBData.hh" // USES SimpleDBData
#include "spatialdata/geocoords/CSCart.hh" // USES CSCart

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ----------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class TestSimpleDBIO;
    } // spatialdb
} // spatialdata

class spatialdata::spatialdb::TestSimpleDBIO {
    // PUBLIC METHODS /////////////////////////////////////////////////////
public:

    /// Test filename(), read(), write().
    static
    void testWriteRead(void);

    /// Test filename(), read() with spatial database file that contains
    /// comments.
    static
    void testReadComments(void);

}; // class TestSimpleDBIO

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestSimpleDBIO::testWriteRead", "[TestSimpleDBIO]") {
    spatialdata::spatialdb::TestSimpleDBIO::testWriteRead();
}
TEST_CASE("TestSimpleDBIO::testReadComments", "[TestSimpleDBIO]") {
    spatialdata::spatialdb::TestSimpleDBIO::testReadComments();
}

// ----------------------------------------------------------------------
// Test filename(), write(), read().
void
spatialdata::spatialdb::TestSimpleDBIO::testWriteRead(void) {
    const size_t spaceDimE = 3;
    const size_t numLocsE = 5;
    const size_t numValuesE = 2;
    const size_t dataDimE = 3;
    const std::vector<std::string> names({ "One", "Two" });
    const std::vector<std::string> units({ "m", "m" });
    const double coordinatesE[numLocsE*spaceDimE] = {
        0.6, 0.1, 0.2,
        1.0, 1.1, 1.2,
        4.7, 9.5, 8.7,
        3.4, 0.7, 9.8,
        3.4, 9.8, 5.7,
    };
    const double dataE[numLocsE*numValuesE] = {
        6.6, 3.4,
        5.5, 6.7,
        2.3, 4.1,
        5.7, 2.0,
        6.3, 6.7,
    };
    std::shared_ptr<spatialdata::geocoords::CoordSys> cs(new spatialdata::geocoords::CSCart());

    SimpleDBData dataOut;
    dataOut.allocate(numLocsE, numValuesE, spaceDimE, dataDimE);
    dataOut.setNames(names);
    dataOut.setUnits(units);
    dataOut.setCoordSys(cs);
    for (size_t iLoc = 0, iData = 0, iCoords = 0; iLoc < numLocsE; ++iLoc) {
        // data
        double* const data = dataOut.getData(iLoc);
        for (size_t iValue = 0; iValue < numValuesE; ++iValue) {
            data[iValue] = dataE[iData++];
        } // for

        // coordinates
        double* const coordinates = dataOut.getCoordinates(iLoc);
        for (size_t iDim = 0; iDim < spaceDimE; ++iDim) {
            coordinates[iDim] = coordinatesE[iCoords++];
        } // for
    } // for

    const char* filename = "spatialdb_ascii.dat";
    SimpleDBIO dbIO;
    dbIO.write(dataOut, filename);

    SimpleDBData dataIn;
    dbIO.read(&dataIn, filename);

    CHECK(numLocsE == dataIn.getNumLocs());
    CHECK(dataDimE == dataIn.getDataDim());
    CHECK(spaceDimE == dataIn.getSpaceDim());
    REQUIRE(numValuesE == dataIn.getNumValues());
    for (size_t iVal = 0; iVal < numValuesE; ++iVal) {
        CHECK(std::string(names[iVal]) == std::string(dataIn.getName(iVal)));
        CHECK(std::string(units[iVal]) == std::string(dataIn.getUnits(iVal)));
    } // for

    const double tolerance = 1.0e-06;
    for (size_t iLoc = 0, i = 0; iLoc < numLocsE; ++iLoc) {
        const double* coordinates = dataIn.getCoordinates(iLoc);
        for (size_t iDim = 0; iDim < spaceDimE; ++iDim, ++i) {
            const double toleranceV = fabs(coordinatesE[i]) > 0.0 ? fabs(coordinatesE[i]) * tolerance : tolerance;
            CHECK_THAT(coordinates[iDim], Catch::Matchers::WithinAbs(coordinatesE[i], toleranceV));
        } // for
    } // for

    for (size_t iLoc = 0, i = 0; iLoc < numLocsE; ++iLoc) {
        const double* values = dataIn.getData(iLoc);
        for (size_t iVal = 0; iVal < numValuesE; ++iVal, ++i) {
            const double toleranceV = fabs(dataE[i]) > 0.0 ? fabs(dataE[i]) * tolerance : tolerance;
            CHECK_THAT(values[iVal], Catch::Matchers::WithinAbs(dataE[i], toleranceV));
        } // for
    } // for
} // testWriteRead


// ----------------------------------------------------------------------
// Test filename(), read() with spatial database file that contains
// comments.
void
spatialdata::spatialdb::TestSimpleDBIO::testReadComments(void) {
    const size_t spaceDimE = 3;
    const size_t numLocsE = 5;
    const size_t numValuesE = 2;
    const size_t dataDimE = 3;
    const char* names[numValuesE] = { "One", "Two" };
    const char* units[numValuesE] = { "m", "m" };
    const double coordinatesE[numLocsE*spaceDimE] = {
        0.6, 0.1, 0.2,
        1.0, 1.1, 1.2,
        4.7, 9.5, 8.7,
        3.4, 0.7, 9.8,
        3.4, 9.8, 5.7,
    };
    const double dataE[numLocsE*numValuesE] = {
        6.6, 3.4,
        5.5, 6.7,
        2.3, 4.1,
        5.7, 2.0,
        6.3, 6.7,
    };

    const char* filename = "data/spatial_comments.dat";
    SimpleDBIO dbIO;
    SimpleDBData dataIn;
    dbIO.read(&dataIn, filename);

    CHECK(numLocsE == dataIn.getNumLocs());
    CHECK(numValuesE == dataIn.getNumValues());
    CHECK(dataDimE == dataIn.getDataDim());
    CHECK(spaceDimE == dataIn.getSpaceDim());
    for (size_t iVal = 0; iVal < numValuesE; ++iVal) {
        CHECK(std::string(names[iVal]) == std::string(dataIn.getName(iVal)));
        CHECK(std::string(units[iVal]) == std::string(dataIn.getUnits(iVal)));
    } // for

    const double tolerance = 1.0e-06;
    for (size_t iLoc = 0, i = 0; iLoc < numLocsE; ++iLoc) {
        const double* coordinates = dataIn.getCoordinates(iLoc);
        for (size_t iDim = 0; iDim < spaceDimE; ++iDim, ++i) {
            const double toleranceV = fabs(coordinatesE[i]) > 0.0 ? fabs(coordinatesE[i]) * tolerance : tolerance;
            CHECK_THAT(coordinates[iDim], Catch::Matchers::WithinAbs(coordinatesE[i], toleranceV));
        } // for
    } // for

    for (size_t iLoc = 0, i = 0; iLoc < numLocsE; ++iLoc) {
        const double* values = dataIn.getData(iLoc);
        for (size_t iVal = 0; iVal < numValuesE; ++iVal, ++i) {
            const double toleranceV = fabs(dataE[i]) > 0.0 ? fabs(dataE[i]) * tolerance : tolerance;
            CHECK_THAT(values[iVal], Catch::Matchers::WithinAbs(dataE[i], toleranceV));
        } // for
    } // for
} // testReadComments


// End of file
