// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/SimpleDB.hh"

#include "SpatialDB_pyapi.hh" // Python interface for query()

#include "pybind11/pybind11.h"

namespace py = pybind11;

void
init_SimpleDB(py::module_ &m) {
    py::class_<spatialdata::spatialdb::SimpleDB, spatialdata::spatialdb::SpatialDB, std::shared_ptr<spatialdata::spatialdb::SimpleDB> > db(m, "SimpleDB");

    py::enum_<spatialdata::spatialdb::SimpleDB::QueryEnum>(db, "QueryEnum")
    .value("NEAREST", spatialdata::spatialdb::SimpleDB::NEAREST)
    .value("LINEAR", spatialdata::spatialdb::SimpleDB::LINEAR)
    .export_values();

    db
    .def(py::init<const char*>())
    .def("setQueryType", &spatialdata::spatialdb::SimpleDB::setQueryType)
    .def("setFilename", &spatialdata::spatialdb::SimpleDB::setFilename)
    .def("open", &spatialdata::spatialdb::SimpleDB::open)
    .def("close", &spatialdata::spatialdb::SimpleDB::close)
    .def("getNamesDBValues", &spatialdata::spatialdb::SimpleDB::getNamesDBValues)
    .def("setQueryValues", &spatialdata::spatialdb::SimpleDB::setQueryValues)
    .def("query", &spatialdata::spatialdb::PySpatialDB::query)
    ;
}
