// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/geocoords/CSGeoLocal.hh"

#include "pybind11/pybind11.h"

#include <sstream> // USES std::istream, std::ostream

namespace py = pybind11;

void
init_CSGeoLocal(py::module_ &m) {
    py::class_<spatialdata::geocoords::CSGeoLocal, spatialdata::geocoords::CSGeo, std::shared_ptr<spatialdata::geocoords::CSGeoLocal> >(m, "CSGeoLocal")
    .def(py::init<>())
    .def("setLocal", &spatialdata::geocoords::CSGeoLocal::setLocal)
    .def("getLocal", [](spatialdata::geocoords::CSGeoLocal& self) {
        double originX, originY, yAzimuth;
        self.getLocal(&originX, &originY, &yAzimuth);
        return std::make_tuple(originX, originY, yAzimuth);
    })
    .def("pickle", &spatialdata::geocoords::CSGeoLocal::pickle)
    .def("unpickle", &spatialdata::geocoords::CSGeoLocal::unpickle)
    ;
}
