// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/SimpleGridDB.hh"

#include "SpatialDB_pyapi.hh" // Python interface for query()

#include "pybind11/pybind11.h"

namespace py = pybind11;

void
init_SimpleGridDB(py::module_ &m) {
    py::class_<spatialdata::spatialdb::SimpleGridDB, spatialdata::spatialdb::SpatialDB, std::shared_ptr<spatialdata::spatialdb::SimpleGridDB> > db(m, "SimpleGridDB");

    py::enum_<spatialdata::spatialdb::SimpleGridDB::QueryEnum>(db, "QueryEnum")
    .value("NEAREST", spatialdata::spatialdb::SimpleGridDB::NEAREST)
    .value("LINEAR", spatialdata::spatialdb::SimpleGridDB::LINEAR)
    .export_values();

    db
    .def(py::init<const char*>())
    .def("setQueryType", &spatialdata::spatialdb::SimpleGridDB::setQueryType)
    .def("setFilename", &spatialdata::spatialdb::SimpleGridDB::setFilename)
    .def("open", &spatialdata::spatialdb::SimpleGridDB::open)
    .def("close", &spatialdata::spatialdb::SimpleGridDB::close)
    .def("getNamesDBValues", &spatialdata::spatialdb::SimpleGridDB::getNamesDBValues)
    .def("setQueryValues", &spatialdata::spatialdb::SimpleGridDB::setQueryValues)
    .def("query", &spatialdata::spatialdb::PySpatialDB::query)
    ;
}
