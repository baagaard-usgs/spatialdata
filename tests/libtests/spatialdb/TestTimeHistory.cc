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

#include "spatialdata/spatialdb/TimeHistory.hh" // Test subject

#include "spatialdata/spatialdb/TimeHistoryIO.hh" // USES TimeHistoryIO
#include "spatialdata/units/Parser.hh" // USES Parser

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

// ------------------------------------------------------------------------------------------------
namespace spatialdata {
    namespace spatialdb {
        class TestTimeHistory;
    } // spatialdb
} // spatialdata

class spatialdata::spatialdb::TestTimeHistory {
    // PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////
public:

    /// Test accessors.
    static
    void testAccessors(void);

    /// Test open() and close()
    static
    void testOpenClose(void);

    /// Test query().
    static
    void testQuery(void);

}; // class TestTimeHistory

// ------------------------------------------------------------------------------------------------
TEST_CASE("TestTimeHistory::testAccessors", "[TestTimeHistory]") {
    spatialdata::spatialdb::TestTimeHistory::testAccessors();
}
TEST_CASE("TestTimeHistory::testOpenClose", "[TestTimeHistory]") {
    spatialdata::spatialdb::TestTimeHistory::testOpenClose();
}
TEST_CASE("TestTimeHistory::testQuery", "[TestTimeHistory]") {
    spatialdata::spatialdb::TestTimeHistory::testQuery();
}

// ----------------------------------------------------------------------
// Test accessors.
void
spatialdata::spatialdb::TestTimeHistory::testAccessors(void) {
    const std::string& description = "TestTimeHistory::testAccessors";
    const std::string& filename = "file.th";

    TimeHistory th(description.c_str());
    CHECK(description == std::string(th.getDescription()));

    th.setFilename(filename.c_str());
    CHECK(filename == std::string(th.getFilename()));
} // testAccessors


// ----------------------------------------------------------------------
// Test open() and close()
void
spatialdata::spatialdb::TestTimeHistory::testOpenClose(void) {
    const size_t numPoints = 6;
    const std::vector<double> time({ 0.0, 0.2, 0.8, 1.0, 2.0, 10.0 });
    const std::vector<double> amplitude({ 0.0, 0.4, 1.6, 2.0, 4.0, 0.0 });
    const char* timeUnits = "minute";

    const char* filename = "data/timehistory.data";
    TimeHistoryIO::write(time, amplitude, timeUnits, filename);

    TimeHistory th("TestTimeHistory::testOpenClose");
    th.setFilename(filename);

    th.close(); // Test calling close on unopened db

    th.open();

    units::Parser parser;
    const double scale = parser.parse(timeUnits);
    CHECK(scale > 0.0);

    CHECK(size_t(0) == th._ilower);
    REQUIRE(numPoints == th._time.size());
    REQUIRE(numPoints == th._amplitude.size());
    const double tolerance = 1.0e-06;
    for (size_t i = 0; i < numPoints; ++i) {
        CHECK_THAT(th._time[i]/scale, Catch::Matchers::WithinAbs(time[i], tolerance));
        CHECK_THAT(th._amplitude[i], Catch::Matchers::WithinAbs(amplitude[i], tolerance));
    } // for

    th.close();
    CHECK(size_t(0) == th._time.size());
    CHECK(size_t(0) == th._amplitude.size());

    th.close(); // Test calling close when already closed
} // testOpenClose


// ----------------------------------------------------------------------
// Test query().
void
spatialdata::spatialdb::TestTimeHistory::testQuery(void) {
    const char* filename = "data/timehistory.timedb";
    const size_t nqueries = 7;
    const double timeQ[nqueries] = {
        0.5, 0.0, 0.6, 2.0, 5.0, 20.0, 8.0
    };
    const double amplitudeE[nqueries] = {
        1.0, 0.0, 1.2, 4.0, 2.5, 0.0, 1.0
    };
    const int errE[nqueries] = {
        0, 0, 0, 0, 0, 1, 0
    };

    TimeHistory th("TestTimeHistory::testQuery");
    th.setFilename(filename);

    th.open();

    const double tolerance = 1.0e-06;
    double amplitude = 0.0;
    for (size_t i = 0; i < nqueries; ++i) {
        int err = th.query(&amplitude, timeQ[i]);
        REQUIRE(errE[i] == err);
        if (0 == errE[i]) {
            CHECK_THAT(amplitude, Catch::Matchers::WithinAbs(amplitudeE[i], tolerance));
        } // if
    } // for

    th.close();
} // testQuery


// End of file
