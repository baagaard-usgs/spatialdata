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
void init_SpatialDB(py::module_ &);

void init_AnalyticDB(py::module_ &);

void init_UniformDB(py::module_ &);

void init_SimpleDB(py::module_ &);

void init_SimpleDBData(py::module_ &);

void init_SimpleDBIO(py::module_ &);

void init_SimpleGridDB(py::module_ &);

void init_SimpleGridDBData(py::module_ &);

void init_SimpleGridDBIO(py::module_ &);

void init_CompositeDB(py::module_ &);

void init_GravityField(py::module_ &);

void init_TimeHistory(py::module_ &);

void init_TimeHistoryIO(py::module_ &);

PYBIND11_MODULE(_spatialdb, m) {
    init_SpatialDB(m);
    init_AnalyticDB(m);
    init_UniformDB(m);
    init_SimpleDB(m);
    init_SimpleDBData(m);
    init_SimpleDBIO(m);
    init_SimpleGridDB(m);
    init_SimpleGridDBData(m);
    init_SimpleGridDBIO(m);
    init_CompositeDB(m);
    init_GravityField(m);
    init_TimeHistory(m);
    init_TimeHistoryIO(m);
}
