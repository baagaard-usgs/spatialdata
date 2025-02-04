// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "pybind11/pybind11.h"
namespace py = pybind11;

// Forward declaration of wrappers
void init_CoordSys(py::module_ &);

void init_CSCart(py::module_ &);

void init_CSGeo(py::module_ &);

void init_CSGeoLocal(py::module_ &);

void init_Converter(py::module_ &);

PYBIND11_MODULE(_geocoords, m) {
    init_CoordSys(m);
    init_CSCart(m);
    init_CSGeo(m);
    init_CSGeoLocal(m);
    init_Converter(m);
}
