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
void init_UniformVelModel(py::module_ &);

PYBIND11_MODULE(_spatialdbcontrib, m) {
    init_UniformVelModel(m);
}
