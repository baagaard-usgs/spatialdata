// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/AnalyticDB.hh"

#include "SpatialDB_pyapi.hh" // Python interface for query()

#include "pybind11/pybind11.h"

namespace py = pybind11;

void
init_AnalyticDB(py::module_ &m) {
    py::class_<spatialdata::spatialdb::AnalyticDB, spatialdata::spatialdb::SpatialDB, std::shared_ptr<spatialdata::spatialdb::AnalyticDB> >(m, "AnalyticDB")
    .def(py::init<const char*>())
    .def("clear", &spatialdata::spatialdb::AnalyticDB::clear)
    .def("setData", &spatialdata::spatialdb::AnalyticDB::setData)
    .def("setCoordSys", &spatialdata::spatialdb::AnalyticDB::setCoordSys)
    .def("open", &spatialdata::spatialdb::AnalyticDB::open)
    .def("close", &spatialdata::spatialdb::AnalyticDB::close)
    .def("getNamesDBValues", &spatialdata::spatialdb::AnalyticDB::getNamesDBValues)
    .def("setQueryValues", &spatialdata::spatialdb::AnalyticDB::setQueryValues)
    .def("query", &spatialdata::spatialdb::PySpatialDB::query)
    ;
}
