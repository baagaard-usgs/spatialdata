// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/GravityField.hh"

#include "SpatialDB_pyapi.hh" // Python interface for query()

#include "pybind11/pybind11.h"

namespace py = pybind11;

void
init_GravityField(py::module_ &m) {
    py::class_<spatialdata::spatialdb::GravityField, spatialdata::spatialdb::SpatialDB, std::shared_ptr<spatialdata::spatialdb::GravityField> >(m, "GravityField")
    .def(py::init<>())
    .def("setGravityDir", &spatialdata::spatialdb::GravityField::setGravityDir)
    .def("setGravityAcc", &spatialdata::spatialdb::GravityField::setGravityAcc)
    .def("open", &spatialdata::spatialdb::GravityField::open)
    .def("close", &spatialdata::spatialdb::GravityField::close)
    .def("getNamesDBValues", &spatialdata::spatialdb::GravityField::getNamesDBValues)
    .def("setQueryValues", &spatialdata::spatialdb::GravityField::setQueryValues)
    .def("query", &spatialdata::spatialdb::PySpatialDB::query)
    ;
}
