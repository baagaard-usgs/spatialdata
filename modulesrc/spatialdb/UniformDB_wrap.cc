// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/UniformDB.hh"

#include "SpatialDB_pyapi.hh" // Python interface for query()

#include "pybind11/pybind11.h"

namespace py = pybind11;

void
init_UniformDB(py::module_ &m) {
    py::class_<spatialdata::spatialdb::UniformDB, spatialdata::spatialdb::SpatialDB, std::shared_ptr<spatialdata::spatialdb::UniformDB> >(m, "UniformDB")
    .def(py::init<const char*>())
    .def("setData", &spatialdata::spatialdb::UniformDB::setData)
    .def("open", &spatialdata::spatialdb::UniformDB::open)
    .def("close", &spatialdata::spatialdb::UniformDB::close)
    .def("getNamesDBValues", &spatialdata::spatialdb::UniformDB::getNamesDBValues)
    .def("setQueryValues", &spatialdata::spatialdb::UniformDB::setQueryValues)
    .def("query", &spatialdata::spatialdb::PySpatialDB::query)
    ;
}
