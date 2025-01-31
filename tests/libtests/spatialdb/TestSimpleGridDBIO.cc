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

#include "spatialdata/spatialdb/SimpleGridDBIO.hh" // Test subject

#include "spatialdata/spatialdb/SimpleGridDBData.hh" // USES SimpleGridDBData

#include "spatialdata/geocoords/CSCart.hh" // USE CSCart
#include "spatialdata/geocoords/CSGeo.hh" // USE CSGeo

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include <cmath> // USES fabs()

// ------------------------------------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class TestSimpleGridDBIO;
    } // spatialdb
} // spatialdata

class spatialdata::spatialdb::TestSimpleGridDBIO {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Test read() and write() with CSCart.
    static
    void testIOCSCart(void);

    /// Test read() and write() with CSGeo.
    static
    void testIOCSGeo(void);

    /// Test read() with comments.
    static
    void testReadComments(void);

}; // class TestSimpleGridDBIO

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestSimpleGridDBIO::testIOCSCart", "[TestSimpleGridDBIO]") {
    spatialdata::spatialdb::TestSimpleGridDBIO::testIOCSCart();
}
TEST_CASE("TestSimpleGridDBIO::testIOCSGeo", "[TestSimpleGridDBIO]") {
    spatialdata::spatialdb::TestSimpleGridDBIO::testIOCSGeo();
}
TEST_CASE("TestSimpleGridDBIO::testReadComments", "[TestSimpleGridDBIO]") {
    spatialdata::spatialdb::TestSimpleGridDBIO::testReadComments();
}

// ----------------------------------------------------------------------
// Test filename(), write(), read().
void
spatialdata::spatialdb::TestSimpleGridDBIO::testIOCSCart(void) {
    const size_t numX = 1;
    const size_t numY = 2;
    const size_t numZ = 3;
    const size_t spaceDim = 3;
    const size_t numValues = 2;
    const size_t dataDim = 2;

    const double x[numX] = { -2.0 };
    const double y[numY] = { 0.0, 1.0 };
    const double z[numZ] = { -2.0, -1.0, 2.0 };

    const double data[numX*numY*numZ*numValues] = {
        6.6,  3.4,
        5.5,  6.7,
        2.3,  4.1,
        5.7,  2.0,
        6.3,  6.9,
        3.4,  6.4,
    };
    const std::vector<std::string> names({ "One", "Two" });
    const std::vector<std::string> units({ "m", "m" });
    std::shared_ptr<spatialdata::geocoords::CoordSys> csOut(new spatialdata::geocoords::CSCart());assert(csOut);
    csOut->setSpaceDim(spaceDim);

    SimpleGridDBData dataOut;
    dataOut.setCoordSys(csOut);
    dataOut.allocate(numX, numY, numZ, numValues, spaceDim, dataDim);
    std::copy(x, x+numX, dataOut.getX().data());
    std::copy(y, y+numY, dataOut.getY().data());
    std::copy(z, z+numZ, dataOut.getZ().data());
    dataOut.setNames(names);
    dataOut.setUnits(units);

    const size_t numLocs = numX * numY * numZ;
    for (size_t iLoc = 0, i = 0; iLoc < numLocs; ++iLoc) {
        double* const valuesOut = dataOut.getData(iLoc*numValues);assert(valuesOut);
        for (size_t iValues = 0; iValues < numValues; ++iValues) {
            valuesOut[iValues] = data[i++];
        } // for
    } // for

    const char* filename = "data/grid_xyz.spatialdb";
    SimpleGridDBIO::write(dataOut, filename);

    SimpleGridDBData dataIn;
    SimpleGridDBIO::read(&dataIn, filename);

    CHECK(numX == dataIn.getNumX());
    CHECK(numY == dataIn.getNumY());
    CHECK(numZ == dataIn.getNumZ());
    CHECK(dataDim == dataIn.getDataDim());
    CHECK(spaceDim == dataIn.getSpaceDim());
    REQUIRE(numValues == dataIn.getNumValues());

    // Check names and units
    for (size_t iValues = 0; iValues < numValues; ++iValues) {
        CHECK(names[iValues] == std::string(dataIn.getName(iValues)));
        CHECK(units[iValues] == std::string(dataIn.getUnits(iValues)));
    } // for

    // Check coordinates
    const double tolerance = 1.0e-06;
    const std::vector<double>& xIn = dataIn.getX();
    REQUIRE(xIn.size() == numX);
    for (size_t i = 0; i < numX; ++i) {
        CHECK_THAT(xIn[i], Catch::Matchers::WithinAbs(x[i], tolerance));
    } // for
    const std::vector<double>& yIn = dataIn.getY();
    REQUIRE(yIn.size() == numY);
    for (size_t i = 0; i < numY; ++i) {
        CHECK_THAT(yIn[i], Catch::Matchers::WithinAbs(y[i], tolerance));
    } // for
    const std::vector<double>& zIn = dataIn.getZ();
    REQUIRE(zIn.size() == numZ);
    for (size_t i = 0; i < numZ; ++i) {
        CHECK_THAT(zIn[i], Catch::Matchers::WithinAbs(z[i], tolerance));
    } // for

    // Check to make sure values were read in correctly
    for (size_t iX = 0, i = 0; iX < numX; ++iX) {
        for (size_t iZ = 0; iZ < numZ; ++iZ) {
            for (size_t iY = 0; iY < numY; ++iY) {
                const size_t indexData = dataIn.getDataIndex(iX, numX, iY, numY, iZ, numZ);
                const double* dataLoc = dataIn.getData(indexData);assert(dataLoc);
                for (size_t iValues = 0; iValues < numValues; ++iValues, ++i) {
                    const double toleranceV = (fabs(data[i]) > 0.0) ? tolerance*data[i] : tolerance;
                    CHECK_THAT(dataLoc[iValues], Catch::Matchers::WithinAbs(data[i], toleranceV));
                } // for
            } // for
        } // for
    } // for

#if 0
    // Perform simple nearest query to ensure consistency of read/query
    dbIn.setQueryValues(names, numValues);
    const size_t numLocs = 3;
    const double points[numLocs*spaceDim] = {
        -2.0, 1.0, -2.0,
        -5.0, 0.0,  2.0,
        +6.0, 1.0, -1.0,
    };
    const double dataE[numLocs*numValues] = {
        5.5, 6.7,
        6.3, 6.9,
        5.7, 2.0,
    };
    const int errE[numLocs] = { 0, 0 };

    for (size_t iLoc = 0; iLoc < numLocs; ++iLoc) {
        double data[numValues];
        int err = dbIn.query(data, numValues, &points[iLoc*spaceDim], spaceDim, &csOut);
        REQUIRE(errE[iLoc] == err);
        for (size_t iValues = 0; iValues < numValues; ++iValues) {
            const double valueE = dataE[iLoc*numValues+iValues];
            const double toleranceV = fabs(valueE) > 0.0 ? tolerance*valueE : tolerance;
            CHECK_THAT(data[iValues], Catch::Matchers::WithinAbs(valueE, toleranceV));
        } // for
    } // for
#endif
} // testIOCSCart


// ----------------------------------------------------------------------
// Test filename(), write(), read().
void
spatialdata::spatialdb::TestSimpleGridDBIO::testIOCSGeo(void) {
    const size_t numX = 1;
    const size_t numY = 2;
    const size_t numZ = 3;
    const size_t spaceDim = 3;
    const size_t numValues = 10;
    const size_t dataDim = 2;

    const double x[numX] = { -2.0 };
    const double y[numY] = { 0.0, 1.0 };
    const double z[numZ] = { -2.0, -1.0, 2.0 };

    const double data[numX*numY*numZ*numValues] = {
        6.6,  3.4, 2.3, 8.3, 3.6, 9.4, 8.4, 3.7, 7.3, 1.5,
        5.5,  6.7, 3.4, 9.4, 4.7, 1.5, 9.5, 4.8, 8.4, 2.6,
        2.3,  4.1, 4.5, 0.5, 5.8, 2.6, 0.6, 5.9, 9.5, 3.7,
        5.7,  2.0, 5.6, 1.6, 6.9, 3.7, 1.7, 6.0, 0.6, 4.8,
        6.3,  6.9, 6.7, 2.7, 7.0, 4.8, 2.8, 7.0, 0.7, 5.9,
        3.4,  6.4, 7.8, 3.8, 8.1, 5.9, 3.9, 8.0, 0.8, 6.0,
    };
    const std::vector<std::string> names({ "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten" });
    const std::vector<std::string> units({ "m", "none", "m", "Pa", "Pa", "m", "kg", "m", "none", "none" });
    std::shared_ptr<spatialdata::geocoords::CSGeo> cs(new spatialdata::geocoords::CSGeo());assert(cs);
    cs->setString("+proj=tmerc +datum=WGS84 +lon_0=-122.6765 +lat_0=45.5231 +k=0.9996 +units=m +vunits=m");
    std::shared_ptr<spatialdata::geocoords::CoordSys> csOut = std::move(cs);

    SimpleGridDBData dataOut;
    dataOut.setCoordSys(csOut);
    dataOut.allocate(numX, numY, numZ, numValues, spaceDim, dataDim);
    std::copy(x, x+numX, dataOut.getX().data());
    std::copy(y, y+numY, dataOut.getY().data());
    std::copy(z, z+numZ, dataOut.getZ().data());
    dataOut.setNames(names);
    dataOut.setUnits(units);

    const size_t numLocs = numX * numY * numZ;
    for (size_t iLoc = 0, i = 0; iLoc < numLocs; ++iLoc) {
        double* const valuesOut = dataOut.getData(iLoc*numValues);assert(valuesOut);
        for (size_t iValues = 0; iValues < numValues; ++iValues) {
            valuesOut[iValues] = data[i++];
        } // for
    } // for

    const char* filename = "data/grid_geo.spatialdb";
    SimpleGridDBIO::write(dataOut, filename);

    SimpleGridDBData dataIn;
    SimpleGridDBIO::read(&dataIn, filename);

    CHECK(numX == dataIn.getNumX());
    CHECK(numY == dataIn.getNumY());
    CHECK(numZ == dataIn.getNumZ());
    CHECK(dataDim == dataIn.getDataDim());
    CHECK(spaceDim == dataIn.getSpaceDim());
    REQUIRE(numValues == dataIn.getNumValues());

    // Check names and units
    for (size_t iValues = 0; iValues < numValues; ++iValues) {
        CHECK(names[iValues] == std::string(dataIn.getName(iValues)));
        CHECK(units[iValues] == std::string(dataIn.getUnits(iValues)));
    } // for

    // Check coordinates
    const double tolerance = 1.0e-06;
    const std::vector<double>& xIn = dataIn.getX();
    REQUIRE(xIn.size() == numX);
    for (size_t i = 0; i < numX; ++i) {
        CHECK_THAT(xIn[i], Catch::Matchers::WithinAbs(x[i], tolerance));
    } // for
    const std::vector<double>& yIn = dataIn.getY();
    REQUIRE(yIn.size() == numY);
    for (size_t i = 0; i < numY; ++i) {
        CHECK_THAT(yIn[i], Catch::Matchers::WithinAbs(y[i], tolerance));
    } // for
    const std::vector<double>& zIn = dataIn.getZ();
    REQUIRE(zIn.size() == numZ);
    for (size_t i = 0; i < numZ; ++i) {
        CHECK_THAT(zIn[i], Catch::Matchers::WithinAbs(z[i], tolerance));
    } // for

    // Check to make sure values were read in correctly
    for (size_t iX = 0, i = 0; iX < numX; ++iX) {
        for (size_t iZ = 0; iZ < numZ; ++iZ) {
            for (size_t iY = 0; iY < numY; ++iY) {
                const size_t indexData = dataIn.getDataIndex(iX, numX, iY, numY, iZ, numZ);
                const double* dataLoc = dataIn.getData(indexData);assert(dataLoc);
                for (size_t iValues = 0; iValues < numValues; ++iValues, ++i) {
                    const double toleranceV = (fabs(data[i]) > 0.0) ? tolerance*data[i] : tolerance;
                    CHECK_THAT(dataLoc[iValues], Catch::Matchers::WithinAbs(data[i], toleranceV));
                } // for
            } // for
        } // for
    } // for

#if 0
    // Perform simple nearest query to ensure consistency of read/query
    dbIn.setQueryValues(names, numValues);
    const size_t numLocs = 3;
    const double points[numLocs*spaceDim] = {
        -2.0, 1.0, -2.0,
        -5.0, 0.0,  2.0,
        +6.0, 1.0, -1.0,
    };
    const double dataE[numLocs*numValues] = {
        5.5, 6.7, 3.4, 9.4, 4.7, 1.5, 9.5, 4.8, 8.4, 2.6,
        6.3, 6.9, 6.7, 2.7, 7.0, 4.8, 2.8, 7.0, 0.7, 5.9,
        5.7, 2.0, 5.6, 1.6, 6.9, 3.7, 1.7, 6.0, 0.6, 4.8,
    };
    const int errE[numLocs] = { 0, 0, 0 };

    for (size_t iLoc = 0; iLoc < numLocs; ++iLoc) {
        double data[numValues];
        int err = dbIn.query(data, numValues, &points[iLoc*spaceDim], spaceDim, &csOut);
        REQUIRE(errE[iLoc] == err);
        for (size_t iValues = 0; iValues < numValues; ++iValues) {
            const double valueE = dataE[iLoc*numValues+iValues];
            const double toleranceV = fabs(valueE) > 0.0 ? tolerance*valueE : tolerance;
            CHECK_THAT(data[iValues], Catch::Matchers::WithinAbs(valueE, toleranceV));
        } // for
    } // for
#endif
} // testIOCSGeo


// ----------------------------------------------------------------------
// Test read() with comments.
void
spatialdata::spatialdb::TestSimpleGridDBIO::testReadComments(void) {
    const size_t numX = 1;
    const size_t numY = 1;
    const size_t numZ = 5;
    const size_t spaceDim = 3;
    const size_t numValues = 13;
    const size_t dataDim = 1;

    const double x[numX] = { 0.0 };
    const double y[numY] = { 0.0 };
    const double z[numZ] = { -400.01, -100.0, -30.0, -20.0, 0.0  };

    const double data[numX*numY*numZ*numValues] = {
        1.0e+20, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5,
        2.0e+20, 0.4, 1.4, 2.4, 3.4, 4.4, 5.4, 6.4, 7.4, 8.4, 9.4, 10.4, 11.4,
        4.0e+20, 0.3, 1.3, 2.3, 3.3, 4.3, 5.3, 6.3, 7.3, 8.3, 9.3, 10.3, 11.3,
        1.0e+21, 0.2, 1.2, 2.2, 3.2, 4.2, 5.2, 6.2, 7.2, 8.2, 9.2, 10.2, 11.2,
        1.0e+22, 0.1, 1.1, 2.1, 3.1, 4.1, 5.1, 6.1, 7.1, 8.1, 9.1, 10.1, 11.1,
    };
    const std::vector<std::string> names({
        "viscosity",
        "viscous_strain_xx",
        "viscous_strain_yy",
        "viscous_strain_zz",
        "viscous_strain_xy",
        "viscous_strain_yz",
        "viscous_strain_xz",
        "total_strain_xx",
        "total_strain_yy",
        "total_strain_zz",
        "total_strain_xy",
        "total_strain_yz",
        "total_strain_xz",
    });
    const std::vector<std::string> units({
        "Pa*s",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
        "none",
    });

    const char* filename = "data/grid_comments.spatialdb";
    SimpleGridDBData dataIn;
    SimpleGridDBIO::read(&dataIn, filename);

    CHECK(numX == dataIn.getNumX());
    CHECK(numY == dataIn.getNumY());
    CHECK(numZ == dataIn.getNumZ());
    CHECK(dataDim == dataIn.getDataDim());
    CHECK(spaceDim == dataIn.getSpaceDim());
    REQUIRE(numValues == dataIn.getNumValues());

    // Check names and units
    for (size_t iValues = 0; iValues < numValues; ++iValues) {
        CHECK(names[iValues] == std::string(dataIn.getName(iValues)));
        CHECK(units[iValues] == std::string(dataIn.getUnits(iValues)));
    } // for

    // Check coordinates
    const double tolerance = 1.0e-06;
    const std::vector<double>& xIn = dataIn.getX();
    REQUIRE(xIn.size() == numX);
    for (size_t i = 0; i < numX; ++i) {
        CHECK_THAT(xIn[i], Catch::Matchers::WithinAbs(x[i], tolerance));
    } // for
    const std::vector<double>& yIn = dataIn.getY();
    REQUIRE(yIn.size() == numY);
    for (size_t i = 0; i < numY; ++i) {
        CHECK_THAT(yIn[i], Catch::Matchers::WithinAbs(y[i], tolerance));
    } // for
    const std::vector<double>& zIn = dataIn.getZ();
    REQUIRE(zIn.size() == numZ);
    for (size_t i = 0; i < numZ; ++i) {
        CHECK_THAT(zIn[i], Catch::Matchers::WithinAbs(z[i], tolerance));
    } // for

    // Check to make sure values were read in correctly
    for (size_t iX = 0, i = 0; iX < numX; ++iX) {
        for (size_t iZ = 0; iZ < numZ; ++iZ) {
            for (size_t iY = 0; iY < numY; ++iY) {
                const size_t indexData = dataIn.getDataIndex(iX, numX, iY, numY, iZ, numZ);
                const double* dataLoc = dataIn.getData(indexData);assert(dataLoc);
                for (size_t iValues = 0; iValues < numValues; ++iValues, ++i) {
                    const double toleranceV = (fabs(data[i]) > 0.0) ? tolerance*data[i] : tolerance;
                    CHECK_THAT(dataLoc[iValues], Catch::Matchers::WithinAbs(data[i], toleranceV));
                } // for
            } // for
        } // for
    } // for

} // testReadComments


// End of file
