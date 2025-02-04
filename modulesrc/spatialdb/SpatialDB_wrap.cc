// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "spatialdata/spatialdb/SpatialDB.hh"

#include "spatialdata/geocoords/CoordSys.hh" // USES CoordSys

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"

#include <memory>

namespace py = pybind11;

namespace spatialdata {
    namespace spatialdb {
        class WrapSpatialDB;
    }
}

// Helper class providing implementations of pure virtual functions
class spatialdata::spatialdb::WrapSpatialDB : public spatialdata::spatialdb::SpatialDB {
public:

    using spatialdata::spatialdb::SpatialDB::SpatialDB;

    // Override pure virtual functions so we can call the constructor.
    void open(void) override {
        PYBIND11_OVERRIDE_PURE(void, spatialdata::spatialdb::SpatialDB, open, );
    }

    void close(void) override {
        PYBIND11_OVERRIDE_PURE(void, spatialdata::spatialdb::SpatialDB, close, );
    }

    const std::vector<std::string>& getNamesDBValues(void) const override {
        PYBIND11_OVERRIDE_PURE(const std::vector<std::string>&, spatialdata::spatialdb::SpatialDB, getNamesDBValues, );
    }

    void setQueryValues(const std::vector<std::string>& names) override {
        PYBIND11_OVERRIDE_PURE(void, spatialdata::spatialdb::SpatialDB, setQueryValues, names);
    }

    int query(double* values,
              const size_t numValues,
              const double* coordinates,
              const spatialdata::geocoords::CoordSys* csCoordinates) override {
        PYBIND11_OVERRIDE_PURE(int, spatialdata::spatialdb::SpatialDB, query, values, numValues, coordinates, csCoordinates);
    }

};

// Provide Python interface for non-virtual functions.
void
init_SpatialDB(py::module_ &m) {
    py::class_<spatialdata::spatialdb::SpatialDB, spatialdata::spatialdb::WrapSpatialDB, std::shared_ptr<spatialdata::spatialdb::SpatialDB> >(m, "SpatialDB")
    .def(py::init<const char*>())
    .def("setDescription", &spatialdata::spatialdb::SpatialDB::setDescription)
    .def("getDescription", &spatialdata::spatialdb::SpatialDB::getDescription)
    ;
}
