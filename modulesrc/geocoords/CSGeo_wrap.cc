// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/geocoords/CSGeo.hh"

#include "pybind11/pybind11.h"

#include <sstream> // USES std::istream, std::ostream

namespace py = pybind11;

void
init_CSGeo(py::module_ &m) {
    py::class_<spatialdata::geocoords::CSGeo, spatialdata::geocoords::CoordSys, std::shared_ptr<spatialdata::geocoords::CSGeo> >(m, "CSGeo")
    .def(py::init<>())
    .def("setString", &spatialdata::geocoords::CSGeo::setString)
    .def("getString", &spatialdata::geocoords::CSGeo::getString)
    .def("setSpaceDim", &spatialdata::geocoords::CSGeo::setSpaceDim)
    .def("pickle", &spatialdata::geocoords::CSGeo::pickle)
    .def("unpickle", &spatialdata::geocoords::CSGeo::unpickle)
    ;
}
