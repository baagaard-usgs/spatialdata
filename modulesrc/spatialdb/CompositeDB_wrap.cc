// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/CompositeDB.hh"

#include "SpatialDB_pyapi.hh" // Python interface for query()

#include "pybind11/pybind11.h"

namespace py = pybind11;

void
init_CompositeDB(py::module_ &m) {
    py::class_<spatialdata::spatialdb::CompositeDB, spatialdata::spatialdb::SpatialDB, std::shared_ptr<spatialdata::spatialdb::CompositeDB> >(m, "CompositeDB")
    .def(py::init<const char*>())
    .def("addDB", &spatialdata::spatialdb::CompositeDB::addDB)
    .def("open", &spatialdata::spatialdb::CompositeDB::open)
    .def("close", &spatialdata::spatialdb::CompositeDB::close)
    .def("getNamesDBValues", &spatialdata::spatialdb::CompositeDB::getNamesDBValues)
    .def("setQueryValues", &spatialdata::spatialdb::CompositeDB::setQueryValues)
    .def("query", &spatialdata::spatialdb::PySpatialDB::query)
    ;
}
