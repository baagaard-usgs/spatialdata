// =================================================================================================
// This code is part of SpatialData, developed through the Computational Infrastructure
// for Geodynamics (https://github.com/geodynamics/spatialdata).
//
// Copyright (c) 2010-2025, University of California, Davis and the SpatialData Development Team.
// All rights reserved.
//
// See https://mit-license.org/ and LICENSE.md and for license information.
// =================================================================================================

#include "UniformVelModel.hh"

#include "spatialdata/geocoords/CoordSys.hh" // USES CoordSys

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/numpy.h"

namespace py = pybind11;

void
init_UniformVelModel(py::module_ &m) {
    py::class_<contrib::spatialdb::UniformVelModel, spatialdata::spatialdb::SpatialDB, std::shared_ptr<contrib::spatialdb::UniformVelModel> > (m, "UniformVelModel")

    .def(py::init<const char*>())
    .def("open", &contrib::spatialdb::UniformVelModel::open)
    .def("close", &contrib::spatialdb::UniformVelModel::close)
    .def("getNamesDBValues", &contrib::spatialdb::UniformVelModel::getNamesDBValues)
    .def("setQueryValues", &contrib::spatialdb::UniformVelModel::setQueryValues)
    .def("setVp", &contrib::spatialdb::UniformVelModel::setVp)
    .def("setVs", &contrib::spatialdb::UniformVelModel::setVs)
    .def("setDensity", &contrib::spatialdb::UniformVelModel::setDensity)

    .def("query", [](spatialdata::spatialdb::SpatialDB& db,
                     py::array_t<double, py::array::c_style | py::array::forcecast > coordinatesArray,
                     const spatialdata::geocoords::CoordSys* csCoordinates,
                     const std::vector<std::string>& queryValues) {
        py::buffer_info coordinatesInfo = coordinatesArray.request();
        const double* const coordinates = static_cast<const double*>(coordinatesInfo.ptr);
        if (2 != coordinatesInfo.ndim) {
            throw std::range_error(
                      "Error in 'coordinates' argument to SpatialDB.query(). Dimensions must be 2 [numLocs, spaceDim]."
                      );
        } // if
        const size_t numLocs = coordinatesInfo.shape[0];
        const size_t spaceDim = coordinatesInfo.shape[1];
        if ((spaceDim < 2) || (spaceDim > 3)) {
            throw std::range_error(
                      "Error in 'coordinates' argument to SpatialDB.query(). Second dimension (spatial dimension) must be 2 or 3."
                      );
        } // if

        const size_t querySize = queryValues.size();
        db.setQueryValues(queryValues);

        py::array_t<double> resultArray({numLocs, querySize});
        py::buffer_info resultInfo = resultArray.request();
        double* result = static_cast<double*>(resultInfo.ptr);

        py::array_t<int> errorArray(numLocs);
        py::buffer_info errorInfo = errorArray.request();
        int* error = static_cast<int*>(errorInfo.ptr);

        for (size_t iLoc = 0; iLoc < numLocs; ++iLoc) {
            const double* coordsLoc = &coordinates[iLoc*spaceDim];
            error[iLoc] = db.query(&result[iLoc*querySize], querySize, coordsLoc, csCoordinates);
        } // for

        return std::make_tuple(resultArray, errorArray);
    })
    ;
}
