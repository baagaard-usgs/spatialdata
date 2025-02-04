// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/geocoords/CSCart.hh"

#include "pybind11/pybind11.h"

#include <sstream> // USES std::istream, std::ostream

namespace py = pybind11;

void
init_CSCart(py::module_ &m) {
    py::class_<spatialdata::geocoords::CSCart, spatialdata::geocoords::CoordSys, std::shared_ptr<spatialdata::geocoords::CSCart> >(m, "CSCart")
    .def(py::init<>())
    .def("setToMeters", &spatialdata::geocoords::CSCart::setToMeters)
    .def("getToMeters", &spatialdata::geocoords::CSCart::getToMeters)
    .def("pickle", &spatialdata::geocoords::CSCart::pickle)
    .def("unpickle", &spatialdata::geocoords::CSCart::unpickle)
    ;
}
