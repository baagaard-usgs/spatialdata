// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/geocoords/CoordSys.hh"

#include "pybind11/pybind11.h"

#include <sstream>

namespace py = pybind11;

namespace spatialdata {
    namespace geocoords {
        class WrapCoordSys;
    }
}

// Helper class providing implementations of pure virtual functions
class spatialdata::geocoords::WrapCoordSys : public spatialdata::geocoords::CoordSys {
public:

    using spatialdata::geocoords::CoordSys::CoordSys;

    // Provide implementations of pure virtual functions so we can call constructor in Python.
    void pickle(std::ostream& s) const override {
        PYBIND11_OVERRIDE_PURE(void, spatialdata::geocoords::CoordSys, pickle);
    }

    void unpickle(std::istream& s) override {
        PYBIND11_OVERRIDE_PURE(void, spatialdata::geocoords::CoordSys, unpickle);
    }

};

void
init_CoordSys(py::module_ &m) {
    py::class_<spatialdata::geocoords::CoordSys, spatialdata::geocoords::WrapCoordSys, std::shared_ptr<spatialdata::geocoords::CoordSys> > coordsys(m, "CoordSys");

    py::enum_<spatialdata::geocoords::CoordSys::CSTypeEnum>(coordsys, "CSTypeEnum")
    .value("CARTESIAN", spatialdata::geocoords::CoordSys::CARTESIAN)
    .value("GEOGRAPHIC", spatialdata::geocoords::CoordSys::GEOGRAPHIC)
    .value("LOCAL_GEOGRAPHIC", spatialdata::geocoords::CoordSys::LOCAL_GEOGRAPHIC)
    .export_values(); // Export the values to Python

    coordsys
    .def(py::init<>())
    .def("setSpaceDim", &spatialdata::geocoords::CoordSys::setSpaceDim)
    .def("getSpaceDim", &spatialdata::geocoords::CoordSys::getSpaceDim)
    .def("getCSType", &spatialdata::geocoords::CoordSys::getCSType)
    ;
}
