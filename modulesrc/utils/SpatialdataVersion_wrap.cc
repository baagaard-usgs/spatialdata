// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/utils/SpatialdataVersion.hh"

#include "pybind11/pybind11.h"

namespace py = pybind11;

void
init_SpatialdataVersion(py::module_ &m) {
    py::class_<spatialdata::utils::SpatialdataVersion>(m, "SpatialdataVersion")
    .def_static("isRelease", &spatialdata::utils::SpatialdataVersion::isRelease)
    .def_static("version", &spatialdata::utils::SpatialdataVersion::version)
    .def_static("gitRevision", &spatialdata::utils::SpatialdataVersion::gitRevision)
    .def_static("gitHash", &spatialdata::utils::SpatialdataVersion::gitHash)
    .def_static("gitDate", &spatialdata::utils::SpatialdataVersion::gitDate)
    .def_static("gitBranch", &spatialdata::utils::SpatialdataVersion::gitBranch)
    .def_static("projVersion", &spatialdata::utils::SpatialdataVersion::projVersion);
}
