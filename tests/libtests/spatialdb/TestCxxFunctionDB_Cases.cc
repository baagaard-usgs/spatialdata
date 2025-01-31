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

#include "TestCxxFunctionDB.hh" // USES TestCxxFunctionDB

#include "spatialdata/geocoords/CSCart.hh" // USES CSCart

#include "catch2/catch_test_macros.hpp"

#include <cassert>

namespace spatialdata {
    namespace spatialdb {
        class TestCxxFunctionDB_1D;
        class TestCxxFunctionDB_2D;
        class TestCxxFunctionDB_3D;
    } // spatialdb
} // spatialdata

// ------------------------------------------------------------------------------------------------
class spatialdata::spatialdb::TestCxxFunctionDB_1D {
public:

    // Factories
    static
    TestCxxFunctionDB_Data* createData(void);

    static
    CxxFunctionDB* createDB(void);

private:

    static double density(const double x) {
        return 100.0*x;
    }

    static const char* density_units(void) {
        return "kg/m**3";
    }

    static double vs(const double x) {
        return 2.0*x;
    }

    static const char* vs_units(void) {
        return "km/s";
    }

    static double vp(const double x) {
        return 3.5*x;
    }

    static const char* vp_units(void) {
        return "km/s";
    }

}; // TestCxxFunctionDB_1D

// ------------------------------------------------------------------------------------------------
spatialdata::spatialdb::TestCxxFunctionDB_Data*
spatialdata::spatialdb::TestCxxFunctionDB_1D::createData(void) {
    std::unique_ptr<spatialdata::spatialdb::TestCxxFunctionDB_Data> data(new TestCxxFunctionDB_Data());assert(data);

    data->numValues = 3;
    static const TestCxxFunctionDB_Data::UserData values[3] = {
        {"density", density_units(), 1.0 },
        {"vs", vs_units(), 1000.0 },
        {"vp", vp_units(), 1000.0 },
    };
    data->values = values;

    data->cs = std::shared_ptr<spatialdata::geocoords::CSCart>(new spatialdata::geocoords::CSCart());assert(data->cs);
    data->cs->setSpaceDim(1);

    data->numQueryPoints = 4;
    static const double queryXYZ[4*1] = {
        0.0,
        -10.0,
        4.0,
        0.5,
    };
    data->queryXYZ = queryXYZ;
    static const double queryValues[4*3] = {
        density(0.0),   vs(0.0),   vp(0.0),
        density(-10.0), vs(-10.0), vp(-10.0),
        density(4.0),   vs(4.0),   vp(4.0),
        density(0.5),   vs(0.5),   vp(0.5),
    };
    data->queryValues = queryValues;

    return data.release();
} // createData


// ------------------------------------------------------------------------------------------------
spatialdata::spatialdb::CxxFunctionDB*
spatialdata::spatialdb::TestCxxFunctionDB_1D::createDB(void) {
    std::unique_ptr<CxxFunctionDB> db(new CxxFunctionDB("TestCxxFunctionDB_1D"));assert(db);

    db->addValue("density", density, density_units());
    db->addValue("vs", vs, vs_units());
    db->addValue("vp", vp, vp_units());

    return db.release();
} // createData


// ------------------------------------------------------------------------------------------------
class spatialdata::spatialdb::TestCxxFunctionDB_2D {
public:

    // Factories
    static
    TestCxxFunctionDB_Data* createData(void);

    static
    CxxFunctionDB* createDB(void);

private:

    static double density(const double x,
                          const double y) {
        return 100.0*x*y;
    }

    static const char* density_units(void) {
        return "kg/m**3";
    }

    static double vs(const double x,
                     const double y) {
        return 2.0*x + 0.3*y;
    }

    static const char* vs_units(void) {
        return "km/s";
    }

    static double vp(const double x,
                     const double y) {
        return 0.2*x + 3.5*y;
    }

    static const char* vp_units(void) {
        return "km/s";
    }

}; // TestCxxFunctionDB_2D

// ------------------------------------------------------------------------------------------------
spatialdata::spatialdb::TestCxxFunctionDB_Data*
spatialdata::spatialdb::TestCxxFunctionDB_2D::createData(void) {
    std::unique_ptr<spatialdata::spatialdb::TestCxxFunctionDB_Data> data(new TestCxxFunctionDB_Data());assert(data);

    data->numValues = 3;
    static const TestCxxFunctionDB_Data::UserData values[3] = {
        { "vs", vs_units(), 1000.0, },
        { "vp", vp_units(), 1000.0, },
        { "density", density_units(), 1.0, },
    };
    data->values = values;

    data->cs = std::shared_ptr<spatialdata::geocoords::CSCart>(new spatialdata::geocoords::CSCart());assert(data->cs);
    data->cs->setSpaceDim(2);

    data->numQueryPoints = 4;
    static const double queryXYZ[4*2] = {
        0.0, 0.0,
        -10.0, 4.0,
        4.0, 2.0,
        0.5, 0.1,
    };
    data->queryXYZ = queryXYZ;
    static const double queryValues[4*3] = {
        vs(0.0, 0.0),  vp(0.0, 0.0),  density(0.0, 0.0),
        vs(-10.0, 4.0),  vp(-10.0, 4.0),  density(-10.0, 4.0),
        vs(4.0, 2.0),  vp(4.0, 2.0),  density(4.0, 2.0),
        vs(0.5, 0.1),  vp(0.5, 0.1),  density(0.5, 0.1),
    };
    data->queryValues = queryValues;

    return data.release();
} // createData


// ------------------------------------------------------------------------------------------------
spatialdata::spatialdb::CxxFunctionDB*
spatialdata::spatialdb::TestCxxFunctionDB_2D::createDB(void) {
    std::unique_ptr<CxxFunctionDB> db(new CxxFunctionDB("TestCxxFunctionDB_2D"));assert(db);

    db->addValue("vs", vs, vs_units());
    db->addValue("vp", vp, vp_units());
    db->addValue("density", density, density_units());

    return db.release();
} // _addValues


// ------------------------------------------------------------------------------------------------
class spatialdata::spatialdb::TestCxxFunctionDB_3D {
public:

    // Factories
    static
    TestCxxFunctionDB_Data* createData(void);

    static
    CxxFunctionDB* createDB(void);

private:

    static double density(const double x,
                          const double y,
                          const double z) {
        return 2.3 + 1.0*x*y + 0.3*z*x;
    }

    static const char* density_units(void) {
        return "kg/m**3";
    }

    static double vs(const double x,
                     const double y,
                     const double z) {
        return 0.34 + 2.0*x + 0.2*y + 4.0*z;
    }

    static const char* vs_units(void) {
        return "km/s";
    }

}; // TestCxxFunctionDB_3D

// ------------------------------------------------------------------------------------------------
spatialdata::spatialdb::TestCxxFunctionDB_Data*
spatialdata::spatialdb::TestCxxFunctionDB_3D::createData(void) {
    std::unique_ptr<spatialdata::spatialdb::TestCxxFunctionDB_Data> data(new TestCxxFunctionDB_Data());assert(data);

    data->numValues = 2;
    static const TestCxxFunctionDB_Data::UserData values[2] = {
        { "density", density_units(), 1.0, },
        { "vs", vs_units(), 1000.0, },
    };
    data->values = values;

    data->cs = std::shared_ptr<spatialdata::geocoords::CSCart>(new spatialdata::geocoords::CSCart());assert(data->cs);
    data->cs->setSpaceDim(3);

    data->numQueryPoints = 2;
    static const double queryXYZ[2*3] = {
        0.0, 0.0, 0.0,
        1.0, 4.3, -3.6,
    };
    data->queryXYZ = queryXYZ;
    static const double queryValues[2*2] = {
        density(0.0, 0.0, 0.0),  vs(0.0, 0.0, 0.0),
        density(1.0, 4.3, -3.6), vs(1.0, 4.3, -3.6),
    };
    data->queryValues = queryValues;

    return data.release();
} // setUp


// ------------------------------------------------------------------------------------------------
spatialdata::spatialdb::CxxFunctionDB*
spatialdata::spatialdb::TestCxxFunctionDB_3D::createDB(void) {
    std::unique_ptr<CxxFunctionDB> db(new CxxFunctionDB("TestCxxFunctionDB_3D"));assert(db);

    db->addValue("density", density, density_units());
    db->addValue("vs", vs, vs_units());

    return db.release();
} // createDB


// ------------------------------------------------------------------------------------------------
// Static test cases
TEST_CASE("TestCxxFunctionDB::testConstructor", "[TestCxxFunctionDB]") {
    spatialdata::spatialdb::TestCxxFunctionDB::testConstructor();
}
TEST_CASE("TestCxxFunctionDB::testDescription", "[TestCxxFunctionDB]") {
    spatialdata::spatialdb::TestCxxFunctionDB::testDescription();
}
TEST_CASE("TestCxxFunctionDB::testCoordsys", "[TestCxxFunctionDB]") {
    spatialdata::spatialdb::TestCxxFunctionDB::testCoordsys();
}

// Data test cases
TEST_CASE("TestCxxFunctionDB::1D::testAddValue", "[TestCxxFunctionDB][1D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_1D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_1D::createDB()).testAddValue();
}
TEST_CASE("TestCxxFunctionDB::1D::testOpenClose", "[TestCxxFunctionDB][1D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_1D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_1D::createDB()).testOpenClose();
}
TEST_CASE("TestCxxFunctionDB::1D::testGetNamesDBValues", "[TestCxxFunctionDB][1D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_1D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_1D::createDB()).testGetNamesDBValues();
}
TEST_CASE("TestCxxFunctionDB::1D::testQueryValues", "[TestCxxFunctionDB][1D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_1D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_1D::createDB()).testQueryValues();
}
TEST_CASE("TestCxxFunctionDB::1D::testQuery", "[TestCxxFunctionDB][1D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_1D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_1D::createDB()).testQuery();
}

TEST_CASE("TestCxxFunctionDB::2D::testAddValue", "[TestCxxFunctionDB][2D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_2D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_2D::createDB()).testAddValue();
}
TEST_CASE("TestCxxFunctionDB::2D::testOpenClose", "[TestCxxFunctionDB][2D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_2D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_2D::createDB()).testOpenClose();
}
TEST_CASE("TestCxxFunctionDB::2D::testGetNamesDBValues", "[TestCxxFunctionDB][2D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_2D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_2D::createDB()).testGetNamesDBValues();
}
TEST_CASE("TestCxxFunctionDB::2D::testQueryValues", "[TestCxxFunctionDB][2D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_2D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_2D::createDB()).testQueryValues();
}
TEST_CASE("TestCxxFunctionDB::2D::testQuery", "[TestCxxFunctionDB][2D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_2D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_2D::createDB()).testQuery();
}

TEST_CASE("TestCxxFunctionDB::3D::testAddValue", "[TestCxxFunctionDB][3D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_3D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_3D::createDB()).testAddValue();
}
TEST_CASE("TestCxxFunctionDB::3D::testOpenClose", "[TestCxxFunctionDB][3D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_3D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_3D::createDB()).testOpenClose();
}
TEST_CASE("TestCxxFunctionDB::3D::testGetNamesDBValues", "[TestCxxFunctionDB][3D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_3D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_3D::createDB()).testGetNamesDBValues();
}
TEST_CASE("TestCxxFunctionDB::3D::testQueryValues", "[TestCxxFunctionDB][3D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_3D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_3D::createDB()).testQueryValues();
}
TEST_CASE("TestCxxFunctionDB::3D::testQuery", "[TestCxxFunctionDB][3D]") {
    spatialdata::spatialdb::TestCxxFunctionDB(spatialdata::spatialdb::TestCxxFunctionDB_3D::createData(),
                                              spatialdata::spatialdb::TestCxxFunctionDB_3D::createDB()).testQuery();
}

// End of file
