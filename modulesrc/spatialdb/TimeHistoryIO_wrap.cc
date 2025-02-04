// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/TimeHistoryIO.hh"

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace py = pybind11;

void
init_TimeHistoryIO(py::module_ &m) {
    py::class_<spatialdata::spatialdb::TimeHistoryIO> (m, "TimeHistoryIO")
    .def_static("write", &spatialdata::spatialdb::TimeHistoryIO::write)
    ;
}
